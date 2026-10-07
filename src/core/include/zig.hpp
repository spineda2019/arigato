//! Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//!
//! zig.hpp - Symbol/func declarations to call into the zig compilation unit
//!
//! This Source Code Form is subject to the terms of the Mozilla Public
//! License, v. 2.0. If a copy of the MPL was not distributed with this
//! file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_ZIG_HPP_
#define SRC_CORE_INCLUDE_ZIG_HPP_

#include <utility>
namespace arigato::core::zig {
namespace detail {
extern "C" {
void zig_debug_assert(bool cond) noexcept;
}

inline static constexpr bool asserts_enabled{
#ifdef ARIGATO_ASSERT
    true
#else
    false
#endif
};

template <bool Assert>
inline constexpr void RunAssert(bool&&) noexcept {
    static_assert(false, "Unknown specialization");
}

template <>
inline constexpr void RunAssert<true>(bool&& cond) noexcept {
    // This should cause materialization of the expression.
    zig_debug_assert(cond);
}

template <>
inline constexpr void RunAssert<false>(bool&&) noexcept {
    // This should cause AVOID materialization of the expression, as its
    // an rvalue (ideally a prvalue, a move is a code-smell here) that is
    // never used.
}
}  // namespace detail

/// \brief Invoke the zig assertion function
///
/// Assertions in zig are different in C++, and can take affect even in release
/// builds. In non-safety builds (e.g. ReleaseFast and ReleaseSmall), asserts
/// can even be used as optimization heuristics by the compiler. However, that
/// behavior from C++ is plain UB, so this should only hold affect in Debug and
/// ReleaseSafe builds.
///
/// As of C++17, PR values only need to be materialized when they are needed
/// (mostly for copy elision). To encourage the `cond` argument to be a no-op in
/// builds without asserts, we enforce r-value expressions. Since this are not
/// used in non-assert builds, ideally the compiler will completely elide its
/// evaluation.
inline constexpr void zig_assert(bool&& cond) noexcept {
    detail::RunAssert<detail::asserts_enabled>(std::forward<bool&&>(cond));
}

}  // namespace arigato::core::zig

#endif  // SRC_CORE_INCLUDE_ZIG_HPP_
