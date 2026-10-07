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

#include <cstdint>
#include <type_traits>
#include <utility>

namespace arigato::types {
template <class T>
    requires std::is_arithmetic_v<T> && (!std::is_reference_v<T>)
struct Vec2D final {
    T x;
    T y;

    template <class OtherT>
        requires std::is_nothrow_constructible_v<OtherT, T> &&
                 (std::is_trivially_constructible_v<OtherT, T>)
    inline constexpr Vec2D<OtherT> Convert() const noexcept {
        return {
            .x = static_cast<OtherT>(x),
            .y = static_cast<OtherT>(y),
        };
    }

    inline constexpr void Translate(T dx, T dy) noexcept {
        x += dx;
        y += dy;
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
static_assert(std::is_trivially_constructible_v<Vec2D<int>>);
static_assert(std::is_trivially_copy_constructible_v<Vec2D<int>>);
static_assert(std::is_trivially_constructible_v<Vec2D<int>, int, int>);
static_assert(std::is_nothrow_constructible_v<Vec2D<int>, int, int>);
static_assert(std::is_trivially_destructible_v<Vec2D<float>>);
static_assert(std::is_nothrow_destructible_v<Vec2D<float>>);
static_assert(std::is_trivially_constructible_v<Vec2D<float>, float, float>);
static_assert(std::is_nothrow_constructible_v<Vec2D<float>, float, float>);

template <class T>
    requires std::is_arithmetic_v<T> && (!std::is_reference_v<T>)
struct Bounds final {
    T width;
    T height;

    template <class OtherT>
        requires std::is_nothrow_constructible_v<OtherT, T> &&
                 (std::is_trivially_constructible_v<OtherT, T>)
    inline constexpr Bounds<OtherT> Convert() const noexcept {
        return {
            .width = static_cast<OtherT>(width),
            .height = static_cast<OtherT>(height),
        };
    }
};

template <class T>
struct Bounds<const T> final {
    static_assert(
        false,
        "Do not apply const on the type. Make your object const instead");
};

static_assert(std::is_trivially_destructible_v<Bounds<int>>);
static_assert(std::is_nothrow_destructible_v<Bounds<int>>);
static_assert(std::is_trivially_constructible_v<Bounds<int>>);
static_assert(std::is_trivially_copy_constructible_v<Bounds<int>>);
static_assert(std::is_trivially_constructible_v<Bounds<int>, int, int>);
static_assert(std::is_nothrow_constructible_v<Bounds<int>, int, int>);
static_assert(std::is_trivially_destructible_v<Bounds<float>>);
static_assert(std::is_nothrow_destructible_v<Bounds<float>>);
static_assert(std::is_trivially_constructible_v<Bounds<float>, float, float>);
static_assert(std::is_nothrow_constructible_v<Bounds<float>, float, float>);

template <class T>
    requires std::is_arithmetic_v<T> && (!std::is_reference_v<T>)
struct Rectangle final {
    /// TODO(SEP) make this not needed, and include in a future "PositionedRect"
    Vec2D<T> pos;
    Bounds<T> bounds;

    template <class OtherT>
        requires(std::is_trivially_constructible_v<OtherT, T>)
    inline constexpr Rectangle<OtherT> Convert() const noexcept {
        return {
            .pos{pos.template Convert<OtherT>()},
            .bounds{bounds.template Convert<OtherT>()},
        };
    }

    inline constexpr decltype(std::declval<T>() + std::declval<T>()) RightEdge()
        const noexcept {
        return pos.x + bounds.width;
    }

    inline constexpr decltype(std::declval<T>() + std::declval<T>())
    BottomEdge() const noexcept {
        return pos.y + bounds.height;
    }

    inline constexpr bool OverlapsX(Rectangle<T> other) const noexcept {
        return pos.x > other.pos.x && pos.x < other.RightEdge();
    }

    inline constexpr bool OverlapsY(Rectangle<T> other) const noexcept {
        return pos.y < other.pos.y && pos.y > other.BottomEdge();
    }

    inline constexpr bool Overlaps(Rectangle<T> other) const noexcept {
        return pos.x < other.RightEdge() && other.pos.x < this->RightEdge() &&
               pos.y < other.BottomEdge() && other.pos.y < this->BottomEdge();
    }

    template <class O>
        requires std::is_convertible_v<T, O>
    inline constexpr bool Overlaps(Rectangle<O> other_raw) const noexcept {
        return Overlaps(other_raw.template Convert<T>());
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
static_assert(std::is_trivially_constructible_v<Rectangle<int>>);
static_assert(std::is_trivially_copy_constructible_v<Rectangle<int>>);
static_assert(
    std::is_trivially_constructible_v<Rectangle<int>, Vec2D<int>, Bounds<int>>);
static_assert(
    std::is_nothrow_constructible_v<Rectangle<int>, Vec2D<int>, Bounds<int>>);
static_assert(std::is_trivially_destructible_v<Rectangle<float>>);
static_assert(std::is_nothrow_destructible_v<Rectangle<float>>);
static_assert(std::is_trivially_constructible_v<Rectangle<float>>);
static_assert(std::is_trivially_constructible_v<Rectangle<float>, Vec2D<float>,
                                                Bounds<float>>);
static_assert(std::is_nothrow_constructible_v<Rectangle<float>, Vec2D<float>,
                                              Bounds<float>>);

}  // namespace arigato::types

namespace arigato::physics {
/// \brief Where the owning entity desires to move
///
/// Representation from _before_ collision/physics is calculated and
/// enforced
struct Intent final {
    enum class MoveDirection : std::int8_t {
        Positive = 1,
        Zero = 0,
        Negative = -1,
    };

    MoveDirection move_x;
    MoveDirection move_y;
};

static_assert(std::is_trivially_constructible_v<Intent>);
static_assert(std::is_trivially_copy_constructible_v<Intent>);
static_assert(std::is_trivially_destructible_v<Intent>);
}  // namespace arigato::physics

#endif  // SRC_COMMON_TYPES_ARIGATO_PHYSICS_HPP_
