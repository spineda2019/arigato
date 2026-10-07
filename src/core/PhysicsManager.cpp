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
namespace {
PhysicsManager::Rectangle OverlapRectangle(
    PhysicsManager::Rectangle const& rect,
    PhysicsManager::Rectangle const& other) noexcept {
    //
    const float x{std::max(rect.pos.x, other.pos.x)};
    const float y{std::max(rect.pos.y, other.pos.y)};

    const float right_edge{std::min(rect.RightEdge(), other.RightEdge())};
    const float bottom_edge{std::min(rect.BottomEdge(), other.BottomEdge())};

    return {
        .pos{
            .x = x,
            .y = y,
        },
        .bounds{
            .width = right_edge - x,
            .height = bottom_edge - y,
        },
    };
}
}  // namespace

void PhysicsManager::Collide(
    std::span<PhysicsManager::Rectangle*> dynamic_bodies,
    std::span<PhysicsManager::Rectangle> static_bodies) const noexcept {
    // Marks the next dynamic body we have _start at_ for checking the
    // current body.
    auto it{dynamic_bodies.begin()};

    constexpr auto clamp_body =
        [](PhysicsManager::Rectangle* free_body,
           PhysicsManager::Rectangle const& committed_body) noexcept {
            if (free_body->Overlaps(committed_body)) [[unlikely]] {
                const PhysicsManager::Rectangle overlap{
                    OverlapRectangle(*free_body, committed_body)};
                if (overlap.bounds.width < overlap.bounds.height) {
                    // clamp x-axis
                    if (free_body->pos.x <= committed_body.pos.x) {
                        free_body->pos.x -= overlap.bounds.width;
                    } else {
                        free_body->pos.x += overlap.bounds.width;
                    }
                } else {
                    // clamp y-axis
                    if (free_body->pos.y <= committed_body.pos.y) {
                        free_body->pos.y -= overlap.bounds.height;
                    } else {
                        free_body->pos.y += overlap.bounds.height;
                    }
                }
            }
        };

    for (PhysicsManager::Rectangle* to_clamp : dynamic_bodies) {
        for (const PhysicsManager::Rectangle committed_body : static_bodies) {
            clamp_body(to_clamp, committed_body);
        }
        for (const PhysicsManager::Rectangle* committed_body :
             decltype(dynamic_bodies){dynamic_bodies.begin(), it}) {
            clamp_body(to_clamp, *committed_body);
        }

        ++it;
    }
}
}  // namespace arigato::core
