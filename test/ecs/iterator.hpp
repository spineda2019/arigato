// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// test_ecs.hpp - Tests for the ECS
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ECS_ITERATOR_HPP_
#define TEST_ECS_ITERATOR_HPP_

#include <cstdint>
//
#include <ECS.hpp>
#include <arigato/physics.hpp>

#include "entities/Id.hpp"

namespace arigato::test {
inline constexpr bool TestEntityIterator() noexcept {
    using Rect = types::Rectangle<float>;
    using ECS = arigato::core::ECS;

    ECS ecs{};

    constexpr std::uint8_t original_body_count{16};

    entities::Id last_added{};
    for (std::uint8_t x{0}; x < original_body_count; ++x) {
        last_added =
            ecs.SpawnStatic(Rect{.pos{.x = 10.0f, .y = 20.0f},
                                 .bounds{.width = 1.0f, .height = 1.0f}},
                            arigato::id::SpriteId::CafeBar);
    }

    constexpr auto iter_and_count = [](ECS& live_ecs) noexcept -> std::uint8_t {
        std::uint8_t body_count{0};
        for (const ECS::Drawable _ : live_ecs.GetDrawables()) {
            ++body_count;
        }
        return body_count;
    };

    if (iter_and_count(ecs) != original_body_count) {
        return false;
    }

    ecs.DestroyEntity(last_added);

    if (iter_and_count(ecs) != original_body_count - 1) {
        return false;
    }

    return true;
}
}  // namespace arigato::test

#endif  // TEST_ECS_ITERATOR_HPP_
