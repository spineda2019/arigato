/// \file
/// \brief Common types to represent game input between game-state and rendering
///
/// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
///
/// This Source Code Form is subject to the terms of the Mozilla Public
/// License, v. 2.0. If a copy of the MPL was not distributed with this
/// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_COMMON_TYPES_ARIGATO_INPUT_HPP_
#define SRC_COMMON_TYPES_ARIGATO_INPUT_HPP_

#include <type_traits>
namespace arigato::input {
struct Keys final {
    bool left;
    bool right;
    bool up;
    bool down;

    bool space;
};

static_assert(std::is_trivially_destructible_v<Keys>);
static_assert(std::is_nothrow_destructible_v<Keys>);
static_assert(std::is_nothrow_constructible_v<Keys>);
static_assert(std::is_trivially_constructible_v<Keys>);
static_assert(std::is_default_constructible_v<Keys>);

struct Input final {
    Keys pressed;
    Keys held;
};

static_assert(std::is_trivially_destructible_v<Input>);
static_assert(std::is_nothrow_destructible_v<Input>);
static_assert(std::is_nothrow_constructible_v<Input>);
static_assert(std::is_trivially_constructible_v<Input>);
static_assert(std::is_default_constructible_v<Input>);
}  // namespace arigato::input

#endif  // SRC_COMMON_TYPES_ARIGATO_INPUT_HPP_
