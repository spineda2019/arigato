// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// spawning.hpp - Tests for ECS entity spawning helpers
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ECS_SPAWNING_HPP_
#define TEST_ECS_SPAWNING_HPP_

#include <ECS.hpp>
#include <arigato/id.hpp>
#include <arigato/physics.hpp>

namespace arigato::test {
namespace detail {
inline constexpr bool TestPlayerSpawning() noexcept {
    using ECS = arigato::core::ECS;
    using Rect = types::Rectangle<float>;

    ECS ecs{};

    // SpawnPlayer + GetPlayerBody round-trip
    constexpr Rect expected_player{
        .pos{.x = 3.0f, .y = 4.0f},
        .bounds{.width = 2.0f, .height = 2.0f},
    };
    (void)ecs.SpawnPlayer(expected_player, 7.0f);

    const Rect player_body{ecs.GetPlayerBody()};
    if (player_body.pos.x != expected_player.pos.x ||
        player_body.pos.y != expected_player.pos.y ||
        player_body.bounds.width != expected_player.bounds.width ||
        player_body.bounds.height != expected_player.bounds.height) {
        return false;
    } else {
        return true;
    }
}

inline constexpr bool TestStaticSpawning() noexcept {
    using ECS = arigato::core::ECS;
    using Rect = types::Rectangle<float>;

    ECS ecs{};
    // SpawnStatic with an explicit rectangle
    constexpr Rect expected_static{
        .pos{.x = 13.0f, .y = 7.0f},
        .bounds{.width = 8.0f, .height = 4.0f},
    };
    (void)ecs.SpawnStatic(expected_static, arigato::id::SpriteId::CafeBar);

    for (const ECS::Drawable drawable : ecs.GetDrawables()) {
        if (drawable.sprite_id != arigato::id::SpriteId::CafeBar ||
            drawable.rect.pos.x != expected_static.pos.x ||
            drawable.rect.pos.y != expected_static.pos.y ||
            drawable.rect.bounds.width != expected_static.bounds.width ||
            drawable.rect.bounds.height != expected_static.bounds.height) {
            return false;
        }
    }

    return true;
}
}  // namespace detail
inline constexpr bool TestEntitySpawning() noexcept {
    return detail::TestPlayerSpawning() && detail::TestStaticSpawning();
}
}  // namespace arigato::test

#endif  // TEST_ECS_SPAWNING_HPP_
