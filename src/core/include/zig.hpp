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
[[noreturn]] void zig_assert_fail() noexcept;
}

inline constexpr bool asserts_enabled{
#ifdef ARIGATO_ASSERT
    true
#else
    false
#endif
};

}  // namespace detail

/// \brief Invoke the zig assertion function and potentially encourage
/// optimization
///
/// Assertions in zig are different in C++, and can take affect even in release
/// builds. In non-safety builds (e.g. ReleaseFast and ReleaseSmall), asserts
/// can even be used as optimization heuristics by the compiler. However, that
/// behavior from C++ isn't quite as powerful from zig, as that requires
/// crossing the compilation unit boundary, whereas zig compiles all zig files
/// in a single compilation unit.
///
/// Instead, this conducts a hybrid approach:
/// * In constant evaluation, this assertion simply reaches std::unreachable,
///   failing the compilation and (hopefully) providing an understandable
///   diagnostic
/// * In Runtime evaluation _only_ when assertions are enabled, zig's assertion
///   is first called. This gives us sensible panics and stacktraces when we
///   fail the assertion (rather than hard-UB). Then, if-and-only-if the
///   assertion passes, the std::unreachable correctly allows the compiler
///   _in the compilation unit of the caller_ to use the assertion for
///   optimization. This is not allowed when assertions are turned off, as that
///   can allow UB with no chance of panics.
inline constexpr void zig_assert(bool cond) noexcept {
    if consteval {
        if (!cond) {
            std::unreachable();
        }
    } else {
        if constexpr (detail::asserts_enabled) {
            if (!cond) [[unlikely]] {
                detail::zig_assert_fail();
                std::unreachable();
            }
        }
    }
}

}  // namespace arigato::core::zig

#endif  // SRC_CORE_INCLUDE_ZIG_HPP_
