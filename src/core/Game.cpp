// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Game.hpp"

#include "include/Level.hpp"
#include "include/Vec.hpp"

namespace arigato::core {
Game::Game() noexcept : level_{Level::Layout{}}, character_{} {}

Vec2D Game::GetPlayerPosition() const noexcept {
    return character_.GetPosition();
}
std::size_t Game::GetCurrentDay() const noexcept { return level_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    return level_.GetCustomersLeft();
}

void Game::Update(Character::Action character_action,
                  Level::Action level_action) noexcept {
    character_.Apply(character_action);
    level_.Apply(level_action);
}

void Game::NextLevel() noexcept { level_.NextDay(); }

bool Game::Save() const noexcept { return false; }
}  // namespace arigato::core
