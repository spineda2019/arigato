// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// InputSystem.cpp - Translates raw user input into movement intent
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <arigato/input.hpp>
#include <arigato/physics.hpp>
#include <systems/InputSystem.hpp>

namespace arigato::system::input {
physics::Intent ToIntent(arigato::input::Input input) noexcept {
    using MoveDirection = physics::Intent::MoveDirection;

    const auto x_direction{[](bool left, bool right) noexcept -> MoveDirection {
        auto dir{MoveDirection::Zero};

        if (left && !right) {
            dir = MoveDirection::Negative;
        } else if (right && !left) {
            dir = MoveDirection::Positive;
        }

        return dir;
    }(input.held.left, input.held.right)};

    const auto y_direction{[](bool up, bool down) noexcept -> MoveDirection {
        auto dir{MoveDirection::Zero};

        if (up && !down) {
            dir = MoveDirection::Negative;
        } else if (down && !up) {
            dir = MoveDirection::Positive;
        }

        return dir;
    }(input.held.up, input.held.down)};

    return {
        .move_x = x_direction,
        .move_y = y_direction,
    };
}
}  // namespace arigato::system::input
