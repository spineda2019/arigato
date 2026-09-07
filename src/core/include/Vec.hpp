// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Vec.hpp - Dependency (e.g. raylib) agnostic physics types
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_VEC_HPP_
#define SRC_CORE_INCLUDE_VEC_HPP_

namespace arigato::core {
struct Vec2D final {
    float x{};
    float y{};
};

struct Rectangle final {
    Vec2D top_left{};
    float width{};
    float height{};
};

struct Vec3D final {
    float x{};
    float y{};
    float z{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_VEC_HPP_
