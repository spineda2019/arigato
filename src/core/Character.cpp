// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Character.cpp - Implementation for the Character class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Character.hpp"

#include <utility>

namespace arigato::core {
namespace {
constexpr int move_speed{120};
constexpr int Direction(Character::Action::Direction dir) {
    return std::to_underlying(dir);
}
}  // namespace
void Character::Apply(Character::Action action) noexcept {
    pos_.x +=
        static_cast<float>(Direction(action.move_x) * move_speed) * action.dt;
    pos_.y +=
        static_cast<float>(Direction(action.move_y) * move_speed) * action.dt;
};

Character::Vec2D Character::GetPosition() const noexcept { return pos_; }
}  // namespace arigato::core
