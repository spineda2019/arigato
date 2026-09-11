/// \file
/// \brief Basic common types for use across all modules
///
/// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
///
/// This Source Code Form is subject to the terms of the Mozilla Public
/// License, v. 2.0. If a copy of the MPL was not distributed with this
/// file, You can obtain one at https://mozilla.org/MPL/2.init/.

#ifndef SRC_COMMON_TYPES_ARIGATO_PHYSICS_HPP_
#define SRC_COMMON_TYPES_ARIGATO_PHYSICS_HPP_

#include <type_traits>

namespace arigato::types {
template <class T>
    requires std::is_arithmetic_v<T> && (!std::is_reference_v<T>)
struct Vec2D final {
    T x{};
    T y{};

    template <class OtherT>
        requires std::is_nothrow_constructible_v<OtherT, T> &&
                 (std::is_trivially_constructible_v<OtherT, T>)
    constexpr Vec2D<OtherT> Convert() const noexcept {
        return {
            .x = static_cast<OtherT>(x),
            .y = static_cast<OtherT>(y),
        };
    }
};

template <class T>
struct Vec2D<const T> final {
    static_assert(
        false,
        "Do not apply const on the type. Make your object const instead");
};

static_assert(std::is_trivially_destructible_v<Vec2D<int>>);
static_assert(std::is_nothrow_destructible_v<Vec2D<int>>);
static_assert(std::is_trivially_constructible_v<Vec2D<int>, int, int>);
static_assert(std::is_nothrow_constructible_v<Vec2D<int>, int, int>);
static_assert(std::is_trivially_destructible_v<Vec2D<float>>);
static_assert(std::is_nothrow_destructible_v<Vec2D<float>>);
static_assert(std::is_trivially_constructible_v<Vec2D<float>, float, float>);
static_assert(std::is_nothrow_constructible_v<Vec2D<float>, float, float>);

template <class T>
    requires std::is_arithmetic_v<T> && (!std::is_reference_v<T>)
struct Rectangle final {
    Vec2D<T> pos{};
    T width{};
    T height{};

    template <class OtherT>
        requires std::is_nothrow_constructible_v<OtherT, T> &&
                 (std::is_trivially_constructible_v<OtherT, T>)
    constexpr Rectangle<OtherT> Convert() const noexcept {
        return {
            .pos{pos.template Convert<OtherT>()},
            .width = static_cast<OtherT>(width),
            .height = static_cast<OtherT>(height),
        };
    }
};

template <class T>
struct Rectangle<const T> final {
    static_assert(
        false,
        "Do not apply const on the type. Make your object const instead");
};

static_assert(std::is_trivially_destructible_v<Rectangle<int>>);
static_assert(std::is_nothrow_destructible_v<Rectangle<int>>);
static_assert(
    std::is_trivially_constructible_v<Rectangle<int>, Vec2D<int>, int, int>);
static_assert(
    std::is_nothrow_constructible_v<Rectangle<int>, Vec2D<int>, int, int>);
static_assert(std::is_trivially_destructible_v<Rectangle<float>>);
static_assert(std::is_nothrow_destructible_v<Rectangle<float>>);
static_assert(std::is_trivially_constructible_v<Rectangle<float>, Vec2D<float>,
                                                float, float>);
static_assert(std::is_nothrow_constructible_v<Rectangle<float>, Vec2D<float>,
                                              float, float>);

}  // namespace arigato::types
#endif  // SRC_COMMON_TYPES_ARIGATO_PHYSICS_HPP_
