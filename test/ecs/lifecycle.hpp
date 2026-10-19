// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// lifecycle.hpp - Tests for ECS entity lifecycle (creation, destruction, clear)
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ECS_LIFECYCLE_HPP_
#define TEST_ECS_LIFECYCLE_HPP_

#include <cstdint>
//
#include <ECS.hpp>
#include <arigato/id.hpp>
#include <arigato/physics.hpp>

#include "entities/Id.hpp"

namespace arigato::test {
inline constexpr bool TestEntityLifecycle() noexcept {
    using ECS = arigato::core::ECS;
    using Rect = types::Rectangle<float>;

    ECS ecs{};

    // Sequential creation returns unique IDs
    const entities::Id first{ecs.CreateEmptyEntity()};
    const entities::Id second{ecs.CreateEmptyEntity()};
    if (first == second) {
        return false;
    }

    // Destroying the same entity twice must be a safe no-op on the second call
    ecs.DestroyEntity(first);
    ecs.DestroyEntity(first);

    // Next creation should reuse `first`
    const entities::Id reused{ecs.CreateEmptyEntity()};
    if (reused != first) {
        return false;
    }

    // Clear resets all entities (including drawables)
    (void)ecs.SpawnStatic(Rect{.pos{.x = 1.0f, .y = 1.0f},
                               .bounds{.width = 1.0f, .height = 1.0f}},
                          arigato::id::SpriteId::CafeBar);
    ecs.Clear();

    std::uint8_t drawable_count{0};
    for (const ECS::Drawable _ : ecs.GetDrawables()) {
        ++drawable_count;
    }
    if (drawable_count != 0) {
        return false;
    }

    return true;
}
}  // namespace arigato::test

#endif  // TEST_ECS_LIFECYCLE_HPP_
