// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// InputSystem.hpp - Translates raw user input into movement intent
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_SYSTEMS_INPUTSYSTEM_HPP_
#define SRC_CORE_INCLUDE_SYSTEMS_INPUTSYSTEM_HPP_

#include <arigato/input.hpp>
#include <arigato/physics.hpp>

namespace arigato::system::input {
/// \brief Convert held directional keys into a movement intent
///
/// Opposing keys held together (or neither held) yield `Zero` on that axis.
physics::Intent ToIntent(arigato::input::Input) noexcept;
}  // namespace arigato::system::input

#endif  // SRC_CORE_INCLUDE_SYSTEMS_INPUTSYSTEM_HPP_
