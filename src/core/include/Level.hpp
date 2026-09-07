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
#include <vector>
//
#include "./Vec.hpp"

namespace arigato::core {
class Level final {
 public:  // types
    struct Action final {
        std::uint8_t amount_served{};
    };

    struct Layout final {
        Rectangle cafe_bar{};
        std::vector<Rectangle> cats{};
        std::vector<Rectangle> tables{};
    };

 public:
    explicit Level(Layout) noexcept;
    explicit Level(std::uint8_t seed) noexcept;

    std::size_t GetDay() const noexcept;
    std::uint8_t GetCustomersLeft() const noexcept;

    void Apply(Action) noexcept;
    void NextDay() noexcept;
    void SeededNextDay(std::uint8_t seed) noexcept;

 private:
    Layout object_layout_{};
    std::size_t day_{1};
    std::uint8_t customers_left_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_LEVEL_HPP_
