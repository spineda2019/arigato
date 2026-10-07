// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// InteractableComponent.hpp - Represents an arbitrary interactable, like a
// screen transition
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_COMPONENTS_INTERACTABLECOMPONENT_HPP_
#define SRC_CORE_INCLUDE_COMPONENTS_INTERACTABLECOMPONENT_HPP_

#include <cstdint>
//
#include <arigato/physics.hpp>
//
#include "Component.hpp"

namespace arigato::component {

struct InteractableComponent final {
    enum struct InteractionKind : std::uint8_t {
        screen_transition,
    };

    types::Rectangle<float> zone;
    InteractionKind interaction_kind;
};

static_assert(Component<InteractableComponent>);
}  // namespace arigato::component

#endif  // SRC_CORE_INCLUDE_COMPONENTS_INTERACTABLECOMPONENT_HPP_
