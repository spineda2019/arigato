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
#include <span>
#include <type_traits>
#include <vector>
//
#include <arigato/id.hpp>
#include <arigato/physics.hpp>
//
#include "Campaign.hpp"

namespace arigato::core {
class Level final {
 public:  // types
    struct Action final {
        bool served;
    };
    static_assert(std::is_trivially_destructible_v<Action>);
    static_assert(std::is_nothrow_destructible_v<Action>);
    static_assert(std::is_trivially_constructible_v<Action>);
    static_assert(std::is_trivially_constructible_v<Action, bool>);
    static_assert(std::is_nothrow_constructible_v<Action, bool>);
    static_assert(std::is_trivially_copy_constructible_v<Action>);
    static_assert(std::is_trivially_move_constructible_v<Action>);

    using Rectangle = types::Rectangle<int>;

    struct PlacedDecorum final {
        Rectangle rect;
        id::DecorId decorum;
    };
    static_assert(std::is_trivially_destructible_v<PlacedDecorum>);
    static_assert(std::is_nothrow_destructible_v<PlacedDecorum>);
    static_assert(std::is_trivially_constructible_v<PlacedDecorum>);
    static_assert(std::is_nothrow_constructible_v<PlacedDecorum>);

    struct PlacedCat final {
        Rectangle rect;
        id::CatId cat;
    };
    static_assert(std::is_trivially_destructible_v<PlacedCat>);
    static_assert(std::is_nothrow_destructible_v<PlacedCat>);
    static_assert(std::is_trivially_constructible_v<PlacedCat>);
    static_assert(std::is_nothrow_constructible_v<PlacedCat>);

    /// TODO(SEP)
    struct PlacedCustomers final {};

    using Bounds = types::Bounds<int>;

 public:  // APIs
    explicit Level(std::span<const Campaign::Decorum>,
                   std::span<const Campaign::Cat>, Bounds) noexcept;
    explicit Level(std::span<const Campaign::Decorum>,
                   std::span<const Campaign::Cat>, Bounds,
                   std::uint8_t seed) noexcept;

    std::uint8_t GetCustomersLeft() const noexcept;
    std::span<const PlacedCat> GetPlacedCats() const noexcept;
    std::span<const PlacedDecorum> GetPlacedDecor() const noexcept;

    void Apply(Action) noexcept;

 private:  // helper ctors
    explicit Level(std::mt19937 rng, std::span<const Campaign::Decorum> decor,
                   std::span<const Campaign::Cat>, Bounds) noexcept;

 private:
    std::vector<PlacedDecorum> placed_decor_{};
    std::vector<PlacedCat> placed_cats_{};
    std::uint8_t customers_left_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_LEVEL_HPP_
