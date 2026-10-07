/// \file
/// \brief Common meta-programming facilities
///
/// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
///
/// This Source Code Form is subject to the terms of the Mozilla Public
/// License, v. 2.0. If a copy of the MPL was not distributed with this
/// file, You can obtain one at https://mozilla.org/MPL/2.init/.

#ifndef SRC_COMMON_TYPES_ARIGATO_META_HPP_
#define SRC_COMMON_TYPES_ARIGATO_META_HPP_

#include <cstdint>
#include <new>
#include <type_traits>
namespace arigato::meta {
/// \brief If condition for meta-functions
///
/// Exposes a `type` field set to `T` if Condition is true, `F` otherwise
template <bool Condition, class T, class F>
struct If final {
    static_assert(false, "Invalid use of unspecializd `If` metafunction");
};

template <class T, class F>
struct If<true, T, F> final {
    using type = T;
};

template <class T, class F>
struct If<false, T, F> final {
    using type = F;
};

template <class T>
struct EfficientFuncArgType final {
    static_assert(!std::is_reference_v<T>, "T is already a reference");

    using type =
        typename If<std::is_trivially_copy_constructible_v<T> &&
                        sizeof(T) <=
                            std::hardware_destructive_interference_size,
                    T, T const&>::type;
};

template <class Head, class RestHead, class... Rest>
struct OneOf final {
    consteval static bool Helper() {
        if (std::is_same_v<Head, RestHead>) {
            return true;
        } else {
            return OneOf<Head, Rest...>::value;
        }
    }

    static inline constexpr bool value{Helper()};
};

template <class Head, class Last>
struct OneOf<Head, Last> final {
    static inline constexpr bool value{std::is_same_v<Head, Last>};
};

template <class Head, class RestHead, class... Rest>
static inline constexpr bool OneOf_v{OneOf<Head, RestHead, Rest...>::value};

static_assert(OneOf_v<int, int>);
static_assert(!OneOf_v<int, unsigned int>);
static_assert(!OneOf_v<int, const int>);
static_assert(!OneOf_v<int, int&>);
static_assert(!OneOf_v<int, int const&>);

static_assert(OneOf_v<int, int, float>);
static_assert(OneOf_v<int, bool, float, std::uint64_t, int>);
static_assert(OneOf_v<int, bool, float, int, std::uint64_t>);
}  // namespace arigato::meta

#endif  // SRC_COMMON_TYPES_ARIGATO_META_HPP_
