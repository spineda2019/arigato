// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// PhysicsSystem.hpp - Central system for physics
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_SYSTEMS_PHYSICSSYSTEM_HPP_
#define SRC_CORE_INCLUDE_SYSTEMS_PHYSICSSYSTEM_HPP_

#include <span>
//
#include <arigato/physics.hpp>
//
#include "ECS.hpp"
#include "components/PhysicsComponent.hpp"

namespace arigato::system::physics {
using Rectangle = types::Rectangle<float>;

/// \brief Integrate movement intent into body positions
///
/// Only rows whose mask has the body, speed, and intent bits set are moved.
void Move(std::span<const core::ECS::BitMask> masks,
          std::span<const component::PhysicsIntentComponent> intents,
          std::span<const component::PhysicsSpeedComponent> speeds,
          std::span<component::PhysicsBodyComponent> bodies,
          float dt) noexcept;

/// \brief Prevent overlap of 2D rectangles.
void Collide(std::span<Rectangle*> dynamic_bodies,
             std::span<Rectangle> static_bodies) noexcept;
}  // namespace arigato::system::physics

#endif  // SRC_CORE_INCLUDE_SYSTEMS_PHYSICSSYSTEM_HPP_
