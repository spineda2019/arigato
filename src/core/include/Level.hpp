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

namespace arigato::core {
class Level final {
 public:  // types
    struct Action final {
        std::uint8_t amount_served{};
    };

 public:  // APIs
    explicit Level() noexcept;
    explicit Level(std::uint8_t seed) noexcept;

    std::uint8_t GetCustomersLeft() const noexcept;

    void Apply(Action) noexcept;

 private:
    std::uint8_t customers_left_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_LEVEL_HPP_
