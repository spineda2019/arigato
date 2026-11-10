// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// PhysicsSystem.cpp - Central system for physics
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <algorithm>
#include <cstddef>
#include <span>
#include <utility>
#include <vector>
//
#include "ECS.hpp"
#include "components/PhysicsComponent.hpp"
#include "systems/Event.hpp"
#include "systems/PhysicsSystem.hpp"
#include "zig.hpp"

namespace arigato::system::physics {
namespace {
inline constexpr auto g_required_mask{[]() noexcept -> core::ECS::BitMask {
    using Bit = core::ECS::BitSetIndex;

    core::ECS::BitMask mask{};
    mask.set(std::to_underlying(Bit::PhysicsBodyComponent));
    mask.set(std::to_underlying(Bit::PhysicsSpeedComponent));
    mask.set(std::to_underlying(Bit::PhysicsIntentComponent));
    return mask;
}()};

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
          std::span<component::PhysicsFaceableComponent> faceables,
          float dt) noexcept {
    for (std::size_t row{0}; row < masks.size(); ++row) {
        if ((masks[row] & g_required_mask).none()) {
            continue;
        }

        const arigato::physics::Intent intent{intents[row].intent};
        const float speed{speeds[row].speed};
        bodies[row].position.pos.x +=
            IntentToDistance(intent.move_x, speed, dt);
        bodies[row].position.pos.y +=
            IntentToDistance(intent.move_y, speed, dt);
        if (const core::ECS::BitMask bit{std::to_underlying(
                core::ECS::BitSetIndex::PhysicsFaceableComponent)};
            (masks[row] & bit).any()) {
            faceables[row].direction = intent.move_x;
        }
    }
}

void Collide(std::span<Body> dynamic_bodies, std::span<Body> static_bodies,
             std::vector<core::Event>& evs) noexcept {
    // TODO(SEP): add collision events to the vec
    constexpr auto clamp_body = [](Body& free_body, Body const& committed_body,
                                   std::vector<core::Event>& events) noexcept {
        arigato::core::zig::zig_assert((free_body.rect != nullptr) &&
                                       (committed_body.rect != nullptr));
        Rectangle& free_rect{*free_body.rect};
        Rectangle const& committed_rect{*committed_body.rect};
        if (free_rect.Overlaps(committed_rect)) [[unlikely]] {
            events.emplace_back(arigato::core::BodyCollision{
                .a = free_body.id,
                .b = committed_body.id,
            });
            const Rectangle overlap{
                OverlapRectangle(free_rect, committed_rect)};
            if (overlap.bounds.width < overlap.bounds.height) {
                // clamp x-axis
                if (free_rect.pos.x <= committed_rect.pos.x) {
                    free_rect.pos.x -= overlap.bounds.width;
                } else {
                    free_rect.pos.x += overlap.bounds.width;
                }
            } else {
                // clamp y-axis
                if (free_rect.pos.y <= committed_rect.pos.y) {
                    free_rect.pos.y -= overlap.bounds.height;
                } else {
                    free_rect.pos.y += overlap.bounds.height;
                }
            }
        }
    };

    // Marks the next dynamic body we have _start at_ for checking the
    // current body.
    for (auto it{dynamic_bodies.begin()}; Body& to_clamp : dynamic_bodies) {
        for (const Body& committed_body : static_bodies) {
            clamp_body(to_clamp, committed_body, evs);
        }
        for (const Body& committed_body :
             decltype(dynamic_bodies){dynamic_bodies.begin(), it}) {
            clamp_body(to_clamp, committed_body, evs);
        }

        ++it;
    }
}
}  // namespace arigato::system::physics
