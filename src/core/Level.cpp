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

Level::Level() noexcept
    : customers_left_{[]() noexcept -> std::uint8_t {
          try {
              std::mt19937 twister{std::random_device{}()};
              static_assert(min_cust_count >=
                            std::numeric_limits<std::uint8_t>::min());
              static_assert(max_cust_count <=
                            std::numeric_limits<std::uint8_t>::max());
              std::uniform_int_distribution<std::uint16_t> dist{min_cust_count,
                                                                max_cust_count};
              return static_cast<std::uint8_t>(dist(twister));
          } catch (...) {
              return max_cust_count;
          }
      }()} {}

Level::Level(std::uint8_t s) noexcept
    : customers_left_{[](std::uint8_t seed) noexcept -> std::uint8_t {
          try {
              std::mt19937 seeded_twister{seed};
              static_assert(min_cust_count >=
                            std::numeric_limits<std::uint8_t>::min());
              static_assert(max_cust_count <=
                            std::numeric_limits<std::uint8_t>::max());
              std::uniform_int_distribution<std::uint16_t> dist{min_cust_count,
                                                                max_cust_count};
              return static_cast<std::uint8_t>(dist(seeded_twister));
          } catch (...) {
              return max_cust_count;
          }
      }(s)} {}

std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}
void Level::Apply(Level::Action action) noexcept {
    customers_left_ = (customers_left_ >= action.amount_served)
                          ? customers_left_ - action.amount_served
                          : 0;
}
}  // namespace arigato::core
