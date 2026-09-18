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
             std::span<const Campaign::Cat> cats, Level::Bounds bounds) noexcept
    : placed_decor_{
          [](std::span<const Campaign::Decorum> decor_to_place) noexcept
              -> std::vector<Level::PlacedDecorum> {
              try {
                  std::vector<Level::PlacedDecorum> placed{};
                  placed.reserve(decor_to_place.size());

                  for (Campaign::Decorum const& decorum : decor_to_place) {
                      placed.emplace_back(decorum.pos, decorum.id);
                  }

                  return placed;
              } catch (...) {
                  return {};
              }
          }(decor)},
      placed_cats_{[](std::span<const Campaign::Cat> cats_to_place,
                      std::mt19937& rng, Level::Bounds level_bounds) noexcept
                       -> std::vector<Level::PlacedCat> {
          try {
              std::vector<Level::PlacedCat> placed{};
              placed.reserve(cats_to_place.size());

              std::uniform_int_distribution<int> dist_x{0, level_bounds.width};
              std::uniform_int_distribution<int> dist_y{0, level_bounds.height};

              for (Campaign::Cat const& cat : cats_to_place) {
                  placed.emplace_back(
                      Level::Rectangle{
                          .pos{
                              .x = dist_x(rng),
                              .y = dist_y(rng),
                          },
                          .bounds{
                              .width = 1,
                              .height = 1,
                          },
                      },
                      cat.id);
              }

              return placed;
          } catch (...) {
              return {};
          }
      }(cats, twister, bounds)},
      customers_left_{[](std::mt19937& rng) noexcept -> std::uint8_t {
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

Level::Level(std::span<const Campaign::Decorum> decor,
             std::span<const Campaign::Cat> cats, Level::Bounds bounds) noexcept
    : Level(std::mt19937{std::random_device{}()}, decor, cats, bounds) {}

Level::Level(std::span<const Campaign::Decorum> decor,
             std::span<const Campaign::Cat> cats, Level::Bounds bounds,
             std::uint8_t seed) noexcept
    : Level(std::mt19937{seed}, decor, cats, bounds) {}

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
    if (action.served && customers_left_ > 0) {
        --customers_left_;
    }
}
}  // namespace arigato::core
