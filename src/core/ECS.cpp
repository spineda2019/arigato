// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ECS.cpp - Central ECS management
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/ECS.hpp"

#include <limits>
#include <utility>

#include "include/entities/Id.hpp"
#include "include/zig.hpp"

namespace arigato::core {

void ECS::Update() {}

void ECS::AppendEmptyEntity() {
    interactable_components_.emplace_back();
    physics_body_components_.emplace_back();
    physics_speed_components_.emplace_back();
    physics_intent_components_.emplace_back();
    ECS::BitMask alive{};
    alive.set(std::to_underlying(BitSetIndex::Alive));
    component_masks_.emplace_back(alive);

    zig::zig_assert(entity_count <
                    std::numeric_limits<decltype(entity_count)>::max());

    ++entity_count;
}

arigato::entities::Id ECS::CreateEmptyEntity() {
    if (free_ids_.empty()) {
        this->AppendEmptyEntity();
        zig::zig_assert(entity_count > 0);
        return entity_count - 1;
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

}  // namespace arigato::core
