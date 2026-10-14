// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// LevelSystem.cpp - Handles level playing semantics
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "systems/LevelSystem.hpp"
//
#include <random>
//
#include <zig.hpp>

namespace arigato::core {
LevelSystem::Rect LevelSystem::GenerateLevelRectangle(
    LevelSystem::CreationBounds bounds) noexcept {
    try {
        const float max_x{bounds.level.width - bounds.created_body.width};
        const float max_y{bounds.level.height - bounds.created_body.height};

        zig::zig_assert(max_x >= 0.0f);
        zig::zig_assert(max_y >= 0.0f);

        std::uniform_real_distribution<float> dist_x{0, max_x};
        std::uniform_real_distribution<float> dist_y{0, max_y};

        return {
            .pos{
                .x = dist_x(rng_),
                .y = dist_y(rng_),
            },
            .bounds{
                .width = bounds.created_body.width,
                .height = bounds.created_body.height,
            },
        };
    } catch (...) {
        return {
            .pos{},
            .bounds{
                .width = bounds.created_body.width,
                .height = bounds.created_body.height,
            },
        };
    }
}
}  // namespace arigato::core
