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
}  // namespace arigato::meta

#endif  // SRC_COMMON_TYPES_ARIGATO_META_HPP_
