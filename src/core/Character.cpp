// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Character.cpp - Implementation for the Character class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Character.hpp"

namespace arigato::core {

Character::Character(Rectangle bounds) noexcept : bounds_{bounds} {}

void Character::Apply(float dx, float dy) noexcept {
    bounds_.pos.x += dx;
    bounds_.pos.y += dy;
};

Character::Rectangle Character::GetPosition() const noexcept { return bounds_; }
}  // namespace arigato::core
