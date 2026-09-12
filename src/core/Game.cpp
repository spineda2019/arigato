// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Game.hpp"

#include "include/Level.hpp"

namespace arigato::core {
Game::Game() noexcept : character_{}, campaign_{}, level_{} {}

Character::Vec2D Game::GetPlayerPosition() const noexcept {
    return character_.GetPosition();
}
std::size_t Game::GetCurrentDay() const noexcept { return campaign_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    return level_.GetCustomersLeft();
}

void Game::Update(Game::Action action) noexcept {
    character_.Apply(action.character_action);
    level_.Apply(action.level_action);
}

void Game::NextLevel() noexcept {
    level_ = Level{};
    campaign_.NextDay();
}

bool Game::Save() const noexcept { return false; }
}  // namespace arigato::core
