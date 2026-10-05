// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ECS.hpp - Central ECS management
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_ECS_HPP_
#define SRC_CORE_INCLUDE_ECS_HPP_

#include <bitset>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
//
#include "components/Component.hpp"
#include "components/InteractableComponent.hpp"
#include "components/PhysicsComponent.hpp"
//
#include "entities/Id.hpp"
#include "zig.hpp"

namespace arigato::core {
class ECS final {
 public:
    enum struct BitSetIndex : std::uint8_t {
        InteractableComponent,
        PhysicsBodyComponent,
        PhysicsSpeedComponent,
        PhysicsIntentComponent,
        /// \brief just used to check if an entity in our component arrays is in
        /// use
        Alive,
        /// \brief I dislike this. Used for `component_masks_` without C++26
        Count,
    };

    using BitMask = std::bitset<std::to_underlying(BitSetIndex::Count)>;

 public:
    explicit ECS() = default;

    void Update();

    /// \brief Create a new entity with no components
    arigato::entities::Id CreateEmptyEntity();
    /// \brief Create a new entity with a known set of components
    template <component::Component... Cs>
    entities::Id CreateEntity(Cs... components) {
        const entities::Id id{this->CreateEmptyEntity()};
        (this->SetComponent(id, components), ...);
        return id;
    }
    /// \brief Unregister the entity from the ECS and recycle the ID for future
    /// entities
    ///
    /// Double destructions should be memory-safe
    void DestroyEntity(arigato::entities::Id);

 private:
    /// \brief sugar-helper to enfore component concept on container elements
    template <component::Component C>
    using ComponentVec = std::vector<C>;

 private:
    ComponentVec<component::InteractableComponent> interactable_components_;
    ComponentVec<component::PhysicsBodyComponent> physics_body_components_;
    ComponentVec<component::PhysicsSpeedComponent> physics_speed_components_;
    ComponentVec<component::PhysicsIntentComponent> physics_intent_components_;
    /// \brief the bitmask indicating if the component index by id is in use by
    /// the entity of valud id.
    ///
    /// Until C++26, this is maintained by the enum sentinel. I would ideally
    /// like the number of bits to be calculated with
    /// std::meta::enumerators_of(^^BitSetIndex).size()
    std::vector<BitMask> component_masks_;
    /// \brief candidate set for reusable ids that have been destroyed after
    /// newer IDs were created
    std::vector<arigato::entities::Id> free_ids_;
    arigato::entities::Id entity_count{};
    arigato::entities::Id player_id_{};

 private:
    /// \brief Create a new entity at the tail end of the arrays with the alive
    /// bit set
    void AppendEmptyEntity();

    template <component::Component C>
    inline void SetComponent(arigato::entities::Id id, C component) {
        auto& storage{this->StorageOf<C>()};
        zig::zig_assert(storage.size() == entity_count);
        zig::zig_assert(component_masks_.size() == entity_count);
        zig::zig_assert(id < entity_count);
        zig::zig_assert(
            component_masks_[id].test(std::to_underlying(BitSetIndex::Alive)));

        component_masks_[id].set(std::to_underlying(IndexOf<C>()));
        storage[id] = component;
    }

    template <component::Component C>
    inline static consteval BitSetIndex IndexOf() noexcept {
        namespace c = arigato::component;

        if constexpr (std::same_as<C, c::InteractableComponent>) {
            return BitSetIndex::InteractableComponent;
        } else if constexpr (std::same_as<C, c::PhysicsBodyComponent>) {
            return BitSetIndex::PhysicsBodyComponent;
        } else if constexpr (std::same_as<C, c::PhysicsSpeedComponent>) {
            return BitSetIndex::PhysicsSpeedComponent;
        } else if constexpr (std::same_as<C, c::PhysicsIntentComponent>) {
            return BitSetIndex::PhysicsIntentComponent;
        } else {
            static_assert(false, "Unrecognized Component type");
        }
    }

    template <component::Component C>
    inline auto& StorageOf(this auto& self) noexcept {
        namespace c = arigato::component;

        if constexpr (std::same_as<C, c::InteractableComponent>) {
            return self.interactable_components_;
        } else if constexpr (std::same_as<C, c::PhysicsBodyComponent>) {
            return self.physics_body_components_;
        } else if constexpr (std::same_as<C, c::PhysicsSpeedComponent>) {
            return self.physics_speed_components_;
        } else if constexpr (std::same_as<C, c::PhysicsIntentComponent>) {
            return self.physics_intent_components_;
        } else {
            static_assert(false, "Unrecognized Component type");
        }
    }
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_ECS_HPP_
