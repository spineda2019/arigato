// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Level.hpp - Independent representation of the Level entity
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_LEVEL_HPP_
#define SRC_CORE_INCLUDE_LEVEL_HPP_

#include <cstddef>
#include <cstdint>
#include <random>
#include <type_traits>
//
#include <arigato/id.hpp>
#include <arigato/physics.hpp>
//

namespace arigato::core {
class Level final {
 public:  // types
    struct Action final {
        bool served;
    };
    static_assert(std::is_trivially_destructible_v<Action>);
    static_assert(std::is_trivially_constructible_v<Action>);
    static_assert(std::is_trivially_copy_constructible_v<Action>);
    static_assert(std::is_trivially_move_constructible_v<Action>);

    using Rectangle = types::Rectangle<int>;
    using Bounds = types::Bounds<int>;

 public:  // APIs
    explicit Level() noexcept;

    std::uint8_t GetCustomersLeft() const noexcept;

    void Apply(Action) noexcept;

 private:  // helper ctors
    explicit Level(std::mt19937 rng) noexcept;

 private:
    std::uint8_t customers_left_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_LEVEL_HPP_
