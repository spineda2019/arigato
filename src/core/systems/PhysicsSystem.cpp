// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// PhysicsSystem.cpp - Central system for physics
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "systems/PhysicsSystem.hpp"

#include <algorithm>
#include <cstddef>
#include <span>
#include <utility>

#include "ECS.hpp"
#include "components/PhysicsComponent.hpp"

namespace arigato::system::physics {
namespace {
constexpr Rectangle OverlapRectangle(Rectangle const& rect,
                                     Rectangle const& other) noexcept {
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

constexpr float IntentToDistance(arigato::physics::Intent::MoveDirection move,
                                 float speed, float dt) noexcept {
    return static_cast<float>(std::to_underlying(move)) * speed * dt;
}
}  // namespace

void Move(std::span<const core::ECS::BitMask> masks,
          std::span<const component::PhysicsIntentComponent> intents,
          std::span<const component::PhysicsSpeedComponent> speeds,
          std::span<component::PhysicsBodyComponent> bodies,
          float dt) noexcept {
    using BitSetIndex = core::ECS::BitSetIndex;

    constexpr core::ECS::BitMask required{[]() noexcept {
        core::ECS::BitMask mask{};
        mask.set(std::to_underlying(BitSetIndex::PhysicsBodyComponent));
        mask.set(std::to_underlying(BitSetIndex::PhysicsSpeedComponent));
        mask.set(std::to_underlying(BitSetIndex::PhysicsIntentComponent));
        return mask;
    }()};

    for (std::size_t row{0}; row < masks.size(); ++row) {
        if ((masks[row] & required) != required) {
            continue;
        }

        const arigato::physics::Intent intent{intents[row].intent};
        const float speed{speeds[row].speed};
        bodies[row].position.pos.x +=
            IntentToDistance(intent.move_x, speed, dt);
        bodies[row].position.pos.y +=
            IntentToDistance(intent.move_y, speed, dt);
    }
}

void Collide(std::span<Rectangle*> dynamic_bodies,
             std::span<Rectangle> static_bodies) noexcept {
    // Marks the next dynamic body we have _start at_ for checking the
    // current body.
    auto it{dynamic_bodies.begin()};

    constexpr auto clamp_body = [](Rectangle* free_body,
                                   Rectangle const& committed_body) noexcept {
        if (free_body->Overlaps(committed_body)) [[unlikely]] {
            const Rectangle overlap{
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

    for (Rectangle* to_clamp : dynamic_bodies) {
        for (const Rectangle committed_body : static_bodies) {
            clamp_body(to_clamp, committed_body);
        }
        for (const Rectangle* committed_body :
             decltype(dynamic_bodies){dynamic_bodies.begin(), it}) {
            clamp_body(to_clamp, *committed_body);
        }

        ++it;
    }
}
}  // namespace arigato::system::physics
