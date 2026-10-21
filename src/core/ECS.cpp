// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ECS.cpp - Central ECS management
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
//
#include <arigato/input.hpp>
#include <arigato/meta.hpp>
#include <arigato/physics.hpp>
//
#include "arigato/id.hpp"
#include "components/SpriteComponent.hpp"
#include "include/ECS.hpp"
#include "include/components/InteractableComponent.hpp"
#include "include/components/PhysicsComponent.hpp"
#include "include/components/SpriteComponent.hpp"
#include "include/entities/Id.hpp"
#include "include/systems/InputSystem.hpp"
#include "include/systems/PhysicsSystem.hpp"
#include "include/zig.hpp"

namespace arigato::core {
namespace {

// ****************************** Bit Helpers ******************************* //

/// \brief How we're going to map component types to their bit indices until we
/// get some more reflection capabilities.
template <class ComponentT>
struct ComponentTypeToBitSetIndex final {
    static_assert(false, "Unknown component type");
    inline static constexpr ECS::BitSetIndex value{};
};

template <>
struct ComponentTypeToBitSetIndex<arigato::component::InteractableComponent>
    final {
    inline static constexpr auto value{ECS::BitSetIndex::InteractableComponent};
};

template <>
struct ComponentTypeToBitSetIndex<arigato::component::PhysicsBodyComponent>
    final {
    inline static constexpr auto value{ECS::BitSetIndex::PhysicsBodyComponent};
};

template <>
struct ComponentTypeToBitSetIndex<arigato::component::PhysicsSpeedComponent>
    final {
    inline static constexpr auto value{ECS::BitSetIndex::PhysicsSpeedComponent};
};

template <>
struct ComponentTypeToBitSetIndex<arigato::component::PhysicsIntentComponent>
    final {
    inline static constexpr auto value{
        ECS::BitSetIndex::PhysicsIntentComponent};
};

template <>
struct ComponentTypeToBitSetIndex<arigato::component::SpriteComponent> final {
    inline static constexpr auto value{ECS::BitSetIndex::SpriteComponent};
};

template <>
struct ComponentTypeToBitSetIndex<arigato::component::PhysicsFaceableComponent>
    final {
    inline static constexpr auto value{
        ECS::BitSetIndex::PhysicsFaceableComponent};
};
// **************************** End Bit Helpers ***************************** //

template <component::Component C>
inline consteval ECS::BitSetIndex IndexOf() noexcept {
    return ComponentTypeToBitSetIndex<C>::value;
}

static_assert(IndexOf<arigato::component::InteractableComponent>() ==
              ECS::BitSetIndex::InteractableComponent);
static_assert(IndexOf<arigato::component::PhysicsIntentComponent>() ==
              ECS::BitSetIndex::PhysicsIntentComponent);
}  // anonymous namespace

/// \brief helper for `ECS` implementations to keep templates in the CPP file
/// and outside of the header
///
/// Essentially a 'static-class', though that doesn't exist in C++. We do this
/// so we can set this as a friend in `ECS`.
struct ECS::PrivateAccesor final {
    template <component::Component C>
    static inline auto& StorageOf(ECS& ecs) noexcept {
        return std::get<ComponentVec<C>>(ecs.components_);
    }

    template <component::Component C>
    static inline void SetComponent(ECS& ecs, arigato::entities::Id id,
                                    C component) {
        auto& storage{ECS::PrivateAccesor::StorageOf<C>(ecs)};
        zig::zig_assert(storage.size() ==
                        static_cast<std::size_t>(ecs.entity_count));
        zig::zig_assert(ecs.component_masks_.size() ==
                        static_cast<std::size_t>(ecs.entity_count));
        zig::zig_assert(id < ecs.entity_count);
        zig::zig_assert(ecs.component_masks_[id].test(
            std::to_underlying(BitSetIndex::Alive)));

        ecs.component_masks_[id].set(std::to_underlying(IndexOf<C>()));
        storage[id] = component;
    }

    template <component::Component C>
    static inline bool HasComponent(ECS const& ecs, entities::Id id) noexcept {
        return id < ecs.entity_count &&
               ecs.component_masks_[id].test(std::to_underlying(IndexOf<C>()));
    }

    template <component::Component C>
    static inline auto* GetComponent(ECS& ecs, entities::Id id) noexcept {
        auto* const data{ECS::PrivateAccesor::StorageOf<C>(ecs).data()};
        return ECS::PrivateAccesor::HasComponent<C>(ecs, id) ? data + id
                                                             : nullptr;
    }

    template <component::Component C>
    static inline void RemoveComponent(ECS& ecs, entities::Id id) noexcept {
        if (id < ecs.entity_count) {
            ecs.component_masks_[id].reset(std::to_underlying(IndexOf<C>()));
        }
    }

    /// \brief Create a new entity with a known set of components
    template <component::Component... Cs>
    static inline entities::Id CreateEntity(ECS& ecs, Cs... components) {
        const entities::Id id{ecs.CreateEmptyEntity()};
        // elipses on the left is a left-fold, which should be tail recursive
        (..., ECS::PrivateAccesor::SetComponent(ecs, id, components));
        return id;
    }
};
static_assert(std::is_trivially_constructible_v<ECS::PrivateAccesor>);
static_assert(std::is_trivially_destructible_v<ECS::PrivateAccesor>);

void ECS::Update(input::Input input, float dt) noexcept {
    namespace comp = arigato::component;

    // First get the physics intent from translating user input
    comp::PhysicsIntentComponent* const player_intent{
        ECS::PrivateAccesor::GetComponent<comp::PhysicsIntentComponent>(
            *this, player_id_)};
    zig::zig_assert(player_intent != nullptr);
    player_intent->intent = system::input::ToIntent(input);

    // Get the pre-committed "as-if" physics locations as if collision was not
    // enforced
    system::physics::Move(
        component_masks_,
        ECS::PrivateAccesor::StorageOf<comp::PhysicsIntentComponent>(*this),
        ECS::PrivateAccesor::StorageOf<comp::PhysicsSpeedComponent>(*this),
        ECS::PrivateAccesor::StorageOf<comp::PhysicsBodyComponent>(*this),
        ECS::PrivateAccesor::StorageOf<comp::PhysicsFaceableComponent>(*this),
        dt);

    // Clamp the actual body physics and "commit" them
    const auto body_bit{std::to_underlying(BitSetIndex::PhysicsBodyComponent)};
    const auto speed_bit{
        std::to_underlying(BitSetIndex::PhysicsSpeedComponent)};

    std::vector<system::physics::Rectangle*> dynamic_bodies{};
    std::vector<system::physics::Rectangle> static_bodies{};
    for (std::size_t row{0}; row < component_masks_.size(); ++row) {
        const BitMask& mask{component_masks_[row]};
        if (!mask.test(body_bit)) {
            continue;
        }

        auto& storage =
            ECS::PrivateAccesor::StorageOf<component::PhysicsBodyComponent>(
                *this);
        if (mask.test(speed_bit)) {
            dynamic_bodies.push_back(&storage[row].position);
        } else {
            static_bodies.emplace_back(storage[row].position);
        }
    }

    system::physics::Collide(dynamic_bodies, static_bodies);
}

void ECS::Clear() noexcept {
    std::apply(
        [](auto&... component_vecs) noexcept { (component_vecs.clear(), ...); },
        components_);
    component_masks_.clear();
    free_ids_.clear();
    entity_count = 0;
    player_id_ = 0;
}

void ECS::AppendEmptyEntity() {
    zig::zig_assert(entity_count <
                    std::numeric_limits<decltype(entity_count)>::max());

    std::apply(
        [](auto&... component_vecs) { (component_vecs.emplace_back(), ...); },
        components_);
    ECS::BitMask alive{};
    alive.set(std::to_underlying(BitSetIndex::Alive));
    component_masks_.emplace_back(alive);

    ++entity_count;
}

arigato::entities::Id ECS::CreateEmptyEntity() {
    if (free_ids_.empty()) {
        this->AppendEmptyEntity();
        zig::zig_assert(entity_count > 0);
        return static_cast<arigato::entities::Id>(entity_count - 1);
    } else {
        arigato::entities::Id id{free_ids_.back()};
        free_ids_.pop_back();

        zig::zig_assert(id < component_masks_.size());
        component_masks_[id].reset();
        component_masks_[id].set(std::to_underlying(BitSetIndex::Alive));

        return id;
    }
}

void ECS::DestroyEntity(arigato::entities::Id id) {
    zig::zig_assert(id < component_masks_.size());

    if (component_masks_[id].test(std::to_underlying(BitSetIndex::Alive))) {
        component_masks_[id].reset();
        free_ids_.push_back(id);
    }
}

arigato::entities::Id ECS::SpawnPlayer(types::Rectangle<float> body,
                                       float speed) {
    player_id_ = ECS::PrivateAccesor::CreateEntity(
        *this,  //
        component::PhysicsBodyComponent{.position{body}},
        component::PhysicsSpeedComponent{.speed = speed},
        component::PhysicsIntentComponent{},  //
        component::SpriteComponent{.sprite_id = arigato::id::SpriteId::Player},
        component::PhysicsFaceableComponent{});

    return player_id_;
}

arigato::entities::Id ECS::SpawnStatic(types::Rectangle<float> body,
                                       arigato::id::SpriteId id) {
    return ECS::PrivateAccesor::CreateEntity(
        *this,                                             //
        component::PhysicsBodyComponent{.position{body}},  //
        component::SpriteComponent{.sprite_id = id});
}

arigato::entities::Id ECS::SpawnStatic(types::Bounds<float> bounds,
                                       arigato::id::SpriteId id) {
    return ECS::PrivateAccesor::CreateEntity(
        *this,  //
        component::PhysicsBodyComponent{
            .position{level_system_.GenerateLevelRectangle({
                .level{bounds},
                .created_body{.width = 1.0f, .height = 1.0f},
            })}},  //
        component::SpriteComponent{.sprite_id = id});
}

types::Rectangle<float> ECS::GetPlayerBody() noexcept {
    arigato::component::PhysicsBodyComponent const* const body{
        ECS::PrivateAccesor::GetComponent<component::PhysicsBodyComponent>(
            *this, player_id_)};
    zig::zig_assert(body != nullptr);

    return body->position;
}

bool ECS::IsDrawableAt(std::size_t id) noexcept {
    if (id >= component_masks_.size()) [[unlikely]] {
        return false;
    }

    using SpriteComponent = arigato::component::SpriteComponent;
    using BodyComponent = arigato::component::PhysicsBodyComponent;

    const auto* sprite{ECS::PrivateAccesor::GetComponent<SpriteComponent>(
        *this, static_cast<std::uint16_t>(id))};
    const auto* body{ECS::PrivateAccesor::GetComponent<BodyComponent>(
        *this, static_cast<std::uint16_t>(id))};
    const bool alive{
        component_masks_[id].test(std::to_underlying(BitSetIndex::Alive))};

    return sprite && body && alive;
}

ECS::Drawable ECS::GetDrawableAt(std::size_t id) noexcept {
    using SpriteComponent = arigato::component::SpriteComponent;
    using BodyComponent = arigato::component::PhysicsBodyComponent;

    const auto* sprite{ECS::PrivateAccesor::GetComponent<SpriteComponent>(
        *this, static_cast<std::uint16_t>(id))};
    auto* body{ECS::PrivateAccesor::GetComponent<BodyComponent>(
        *this, static_cast<std::uint16_t>(id))};
    const bool alive{
        component_masks_[id].test(std::to_underlying(BitSetIndex::Alive))};

    zig::zig_assert(sprite && body && alive);

    using FaceableComponent = arigato::component::PhysicsFaceableComponent;
    if (const auto* face{ECS::PrivateAccesor::GetComponent<FaceableComponent>(
            *this, static_cast<std::uint16_t>(id))};
        face && face->direction != decltype(face->direction)::Zero) {
        // TODO(SEP): Somehow add support for flipping the sprite
    }

    return {
        .rect{body->position},
        .sprite_id = sprite->sprite_id,
    };
}

ECS::DrawableIterable ECS::GetDrawables() noexcept(
    std::is_nothrow_constructible_v<ECS::DrawableIterable, ECS*>) {
    return ECS::DrawableIterable{this};
}
}  // namespace arigato::core
