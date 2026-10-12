// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// SpriuteComponent.hpp - Represents a component that can be drawn to the screen
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_COMPONENTS_SPRITECOMPONENT_HPP_
#define SRC_CORE_INCLUDE_COMPONENTS_SPRITECOMPONENT_HPP_

#include <arigato/id.hpp>
//
#include "Component.hpp"

namespace arigato::component {
struct SpriteComponent final {
    arigato::id::SpriteId sprite_id;
};

static_assert(Component<SpriteComponent>);
}  // namespace arigato::component

#endif  // SRC_CORE_INCLUDE_COMPONENTS_SPRITECOMPONENT_HPP_
