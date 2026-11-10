// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Event.hpp - Event definitions that systems can raise
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_SYSTEMS_EVENT_HPP_
#define SRC_CORE_INCLUDE_SYSTEMS_EVENT_HPP_

#include <variant>

#include "entities/Id.hpp"

namespace arigato::core {

struct BodyCollision final {
    arigato::entities::Id a;
    arigato::entities::Id b;
};

using Event = std::variant<BodyCollision>;
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_SYSTEMS_EVENT_HPP_
