// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// LevelSystem.hpp - Handles level playing semantics
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_SYSTEMS_LEVELSYSTEM_HPP_
#define SRC_CORE_INCLUDE_SYSTEMS_LEVELSYSTEM_HPP_

#include <random>
#include <type_traits>

#include "arigato/physics.hpp"

namespace arigato::core {
class LevelSystem final {
    // types
 public:
    using Rect = types::Rectangle<float>;
    using Bounds = types::Bounds<float>;

    struct CreationBounds final {
        Bounds level;
        Bounds created_body;
    };

    static_assert(std::is_trivially_constructible_v<CreationBounds>);
    static_assert(std::is_trivially_destructible_v<CreationBounds>);

 public:
    Rect GenerateLevelRectangle(CreationBounds) noexcept;

 private:
    std::mt19937 rng_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_SYSTEMS_LEVELSYSTEM_HPP_
