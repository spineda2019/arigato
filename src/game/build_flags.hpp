// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// build_flags.cpp - compile-time flags
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_GAME_BUILD_FLAGS_HPP_
#define SRC_GAME_BUILD_FLAGS_HPP_

namespace arigato {
inline constexpr bool debug_build{
#ifdef ARIGATO_DEBUG
    true
#else
    false
#endif
};

inline constexpr bool draw_hotboxes{
#ifdef ARIGATO_DRAW_HITBOXES
    true
#else
    false
#endif
};
}  // namespace arigato

#endif  // SRC_GAME_BUILD_FLAGS_HPP_
