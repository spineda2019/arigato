// \file
// \brief Physics handling
//
// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_PHYSICSMANAGER_HPP_
#define SRC_CORE_INCLUDE_PHYSICSMANAGER_HPP_

#include <span>
//
#include <arigato/physics.hpp>

namespace arigato::core {
class PhysicsManager final {
 public:
    using Rectangle = types::Rectangle<float>;

 public:
    /// \brief Prevent overlap of 2D rectangles.
    void Collide(std::span<Rectangle*> dynamic_bodies,
                 std::span<Rectangle> static_bodies) const noexcept;
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_PHYSICSMANAGER_HPP_
