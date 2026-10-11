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
    constexpr auto Dir = [](bool neg,
                            bool pos) static noexcept -> MoveDirection {
        auto dir{MoveDirection::Zero};

        if (neg && !pos) {
            dir = MoveDirection::Negative;
        } else if (pos && !neg) {
            dir = MoveDirection::Positive;
        }

        return dir;
    };

    return {
        .move_x = Dir(input.held.left, input.held.right),
        .move_y = Dir(input.held.up, input.held.down),
    };
}
}  // namespace arigato::system::input
