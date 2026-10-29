// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// PhysicsComponent.hpp - Represents an arbitrary collidable object
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_COMPONENTS_PHYSICSCOMPONENT_HPP_
#define SRC_CORE_INCLUDE_COMPONENTS_PHYSICSCOMPONENT_HPP_

#include <arigato/physics.hpp>
#include <cstdint>
//
#include "Component.hpp"

namespace arigato::component {
struct PhysicsBodyComponent final {
    types::Rectangle<float> position;
};
static_assert(Component<PhysicsBodyComponent>);

struct PhysicsSpeedComponent final {
    /// \brief game-cells/second
    float speed;
};
static_assert(Component<PhysicsSpeedComponent>);

struct PhysicsIntentComponent final {
    arigato::physics::Intent intent;
};
static_assert(Component<PhysicsIntentComponent>);

/// \brief Represents the ability for a physics object to face a direction.
struct PhysicsFaceableComponent final {
    arigato::physics::Intent::MoveDirection direction;
    arigato::physics::Intent::MoveDirection last_direction;
};
static_assert(Component<PhysicsFaceableComponent>);
}  // namespace arigato::component

#endif  // SRC_CORE_INCLUDE_COMPONENTS_PHYSICSCOMPONENT_HPP_
