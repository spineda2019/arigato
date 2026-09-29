//! Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//!
//! zig.hpp - Symbol/func declarations to call into the zig compilation unit
//!
//! This Source Code Form is subject to the terms of the Mozilla Public
//! License, v. 2.0. If a copy of the MPL was not distributed with this
//! file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_ZIG_HPP_
#define SRC_CORE_INCLUDE_ZIG_HPP_

namespace arigato::core::zig {
extern "C" {
/// \brief Invoke the zig assertion function
///
/// Assertions in zig are different in C++, and can take affect even in release
/// builds. In non-safety builds (e.g. ReleaseFast and ReleaseSmall), asserts
/// can even be used as optimization heuristics by the compiler. However, that
/// behavior from C++ is plain UB, so this should only hold affect in Debug and
/// ReleaseSafe builds
void zig_assert(bool cond) noexcept;
}
}  // namespace arigato::core::zig

#endif  // SRC_CORE_INCLUDE_ZIG_HPP_
