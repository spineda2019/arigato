// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Level.cpp - Implementation for the Level class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Level.hpp"
//
#include <cstddef>
#include <cstdint>
#include <limits>
#include <random>
//
#include <arigato/meta.hpp>

namespace arigato::core {

namespace {
inline constexpr std::uint8_t min_cust_count{3};
inline constexpr std::uint8_t max_cust_count{32};
static_assert(min_cust_count < max_cust_count);
}  // anonymous namespace

Level::Level(std::mt19937 twister) noexcept
    : customers_left_{[](std::mt19937& rng) noexcept -> std::uint8_t {
          try {
              static_assert(min_cust_count >=
                            std::numeric_limits<std::uint8_t>::min());
              static_assert(max_cust_count <=
                            std::numeric_limits<std::uint8_t>::max());
              std::uniform_int_distribution<std::uint16_t> dist{min_cust_count,
                                                                max_cust_count};
              return static_cast<std::uint8_t>(dist(rng));
          } catch (...) {
              return max_cust_count;
          }
      }(twister)} {}

Level::Level() noexcept : Level(std::mt19937{std::random_device{}()}) {}

std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}

void Level::Apply(Level::Action action) noexcept {
    if (action.served && customers_left_ > 0) {
        --customers_left_;
    }
}
}  // namespace arigato::core
