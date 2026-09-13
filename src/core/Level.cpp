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
#include <span>
#include <vector>
//
#include <arigato/meta.hpp>

#include "include/Campaign.hpp"

namespace arigato::core {

namespace {
inline constexpr std::uint8_t min_cust_count{3};
inline constexpr std::uint8_t max_cust_count{32};
static_assert(min_cust_count < max_cust_count);

}  // anonymous namespace

Level::Level(std::mt19937 twister, std::span<const Campaign::Decorum> decor,
             std::span<const Campaign::Cat>) noexcept
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
      }(twister)},
      placed_decor_{
          [](std::span<const Campaign::Decorum> decor_to_place,
             std::mt19937& rng) noexcept -> std::vector<Level::PlacedDecorum> {
              try {
                  std::vector<Level::PlacedDecorum> placed{};
                  placed.reserve(decor_to_place.size());

                  std::uniform_int_distribution<std::uint16_t> dist{
                      0, std::numeric_limits<std::uint8_t>::max()};

                  for (Campaign::Decorum const& decorum : decor_to_place) {
                      placed.emplace_back(
                          Level::Rectangle{
                              .pos{
                                  .x = dist(rng),
                                  .y = dist(rng),
                              },
                              .width = 1,
                              .height = 1,
                          },
                          decorum.id);
                  }

                  return placed;
              } catch (...) {
                  return {};
              }
          }(decor, twister)} {}

Level::Level(std::span<const Campaign::Decorum> decor,
             std::span<const Campaign::Cat> cats) noexcept
    : Level(std::mt19937{std::random_device{}()}, decor, cats) {}

Level::Level(std::span<const Campaign::Decorum> decor,
             std::span<const Campaign::Cat> cats, std::uint8_t seed) noexcept
    : Level(std::mt19937{seed}, decor, cats) {}

std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}

std::span<const Level::PlacedCat> Level::GetPlacedCats() const noexcept {
    return {placed_cats_.cbegin(), placed_cats_.size()};
}

std::span<const Level::PlacedDecorum> Level::GetPlacedDecor() const noexcept {
    return {placed_decor_.cbegin(), placed_decor_.size()};
}

void Level::Apply(Level::Action action) noexcept {
    customers_left_ = (customers_left_ >= action.amount_served)
                          ? customers_left_ - action.amount_served
                          : 0;
}
}  // namespace arigato::core
