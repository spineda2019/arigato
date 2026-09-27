// \file
// \brief Physics handling
//
// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/PhysicsManager.hpp"

#include <algorithm>
#include <span>

namespace arigato::core {
void PhysicsManager::Collide(
    std::span<PhysicsManager::Rectangle*> dynamic_bodies,
    std::span<PhysicsManager::Rectangle> static_bodies) const noexcept {
    // Marks the next dynamic body we have _start at_ for checking the
    // current body.
    auto it{dynamic_bodies.begin()};

    for (PhysicsManager::Rectangle* to_clamp : dynamic_bodies) {
        std::ranges::for_each(
            static_bodies,
            [to_clamp](PhysicsManager::Rectangle committed_body) noexcept {
                if (to_clamp->Overlaps(committed_body)) [[unlikely]] {
                    to_clamp->pos.x = committed_body.pos.x;
                }

                if (to_clamp->Overlaps(committed_body)) [[unlikely]] {
                    to_clamp->pos.y = committed_body.pos.y;
                }
            });
        std::for_each(
            dynamic_bodies.begin(), it,
            [to_clamp](PhysicsManager::Rectangle* committed_body) noexcept {
                if (to_clamp->Overlaps(*committed_body)) [[unlikely]] {
                    to_clamp->pos.x = committed_body->pos.x;
                }

                if (to_clamp->Overlaps(*committed_body)) [[unlikely]] {
                    to_clamp->pos.y = committed_body->pos.y;
                }
            });

        ++it;
    }
}
}  // namespace arigato::core
