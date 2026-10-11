// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ECS.cpp - Central ECS management
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <cstddef>
#include <limits>
#include <span>
#include <utility>
#include <vector>
//
#include <arigato/input.hpp>
#include <arigato/physics.hpp>
//
#include "include/ECS.hpp"
#include "include/components/PhysicsComponent.hpp"
#include "include/entities/Id.hpp"
#include "include/systems/InputSystem.hpp"
#include "include/systems/PhysicsSystem.hpp"
#include "include/zig.hpp"

namespace arigato::core {

void ECS::Update(input::Input input, float dt) noexcept {
    // First get the physics intent from translating user input
    arigato::component::PhysicsIntentComponent* const player_intent{
        this->GetComponent<component::PhysicsIntentComponent>(player_id_)};
    zig::zig_assert(player_intent != nullptr);
    player_intent->intent = system::input::ToIntent(input);

    // Get the pre-committed "as-if" physics locations as if collision was not
    // enforced
    system::physics::Move(component_masks_, physics_intent_components_,
                          physics_speed_components_, physics_body_components_,
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

        if (mask.test(speed_bit)) {
            dynamic_bodies.push_back(&physics_body_components_[row].position);
        } else {
            static_bodies.emplace_back(physics_body_components_[row].position);
        }
    }

    system::physics::Collide(dynamic_bodies, static_bodies);
}

void ECS::Clear() noexcept {
    interactable_components_.clear();
    physics_body_components_.clear();
    physics_speed_components_.clear();
    physics_intent_components_.clear();
    component_masks_.clear();
    free_ids_.clear();
    entity_count = 0;
    player_id_ = 0;
}

void ECS::AppendEmptyEntity() {
    zig::zig_assert(entity_count <
                    std::numeric_limits<decltype(entity_count)>::max());

    interactable_components_.emplace_back();
    physics_body_components_.emplace_back();
    physics_speed_components_.emplace_back();
    physics_intent_components_.emplace_back();
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
    player_id_ =
        this->CreateEntity(component::PhysicsBodyComponent{.position{body}},
                           component::PhysicsSpeedComponent{.speed = speed},
                           component::PhysicsIntentComponent{});

    return player_id_;
}

arigato::entities::Id ECS::SpawnStatic(types::Rectangle<float> body) {
    return this->CreateEntity(component::PhysicsBodyComponent{.position{body}});
}

types::Rectangle<float> ECS::GetPlayerBody() const noexcept {
    arigato::component::PhysicsBodyComponent const* const body{
        this->GetComponent<component::PhysicsBodyComponent>(player_id_)};
    zig::zig_assert(body != nullptr);

    return body->position;
}

}  // namespace arigato::core
