// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Level.cpp - Implementation for the Level class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Level.hpp"

#include <cstddef>
#include <cstdint>
#include <random>
#include <utility>

namespace arigato::core {

namespace {
inline constexpr std::uint8_t min_cust_count{3};
inline constexpr std::uint8_t max_cust_count{32};
static_assert(min_cust_count < max_cust_count);
std::uint8_t UnseededRandomCustAmount() noexcept {
    try {
        std::mt19937 twister{std::random_device{}()};
        std::uniform_int_distribution<std::uint8_t> dist{min_cust_count,
                                                         max_cust_count};
        return dist(twister);
    } catch (...) {
        return 42;
    }
}
std::uint8_t SeededRandomCustAmount(std::uint8_t seed) noexcept {
    try {
        std::mt19937 seeded_twister{seed};
        std::uniform_int_distribution<std::uint8_t> dist{min_cust_count,
                                                         max_cust_count};
        return dist(seeded_twister);
    } catch (...) {
        return 42;
    }
}
}  // anonymous namespace

Level::Level(Level::Layout layout) noexcept
    : object_layout_{std::move(layout)},
      day_{1},
      customers_left_{UnseededRandomCustAmount()} {}

Level::Level(std::uint8_t seed) noexcept
    : day_{1}, customers_left_{SeededRandomCustAmount(seed)} {}

std::size_t Level::GetDay() const noexcept { return day_; }
std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}
void Level::Apply(Level::Action action) noexcept {
    customers_left_ = (customers_left_ >= action.amount_served)
                          ? customers_left_ - action.amount_served
                          : 0;
}

void Level::NextDay() noexcept {
    ++day_;
    customers_left_ = UnseededRandomCustAmount();
}

void Level::SeededNextDay(std::uint8_t seed) noexcept {
    ++day_;
    customers_left_ = SeededRandomCustAmount(seed);
}
}  // namespace arigato::core
