// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/Game.hpp"

#include <optional>

#include "include/Level.hpp"

namespace arigato::core {
Game::Game() noexcept : campaign_{}, character_{}, level_{std::nullopt} {}

Character::Vec2D Game::GetPlayerPosition() const noexcept {
    return character_.GetPosition();
}
std::size_t Game::GetCurrentDay() const noexcept { return campaign_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    /// Yes this throws. Yes this would terminate the program. I am (currently)
    /// OK with that.
    return level_->GetCustomersLeft();
}

void Game::Update(Game::Action action) noexcept {
    if (!level_.has_value()) [[unlikely]] {
        level_.emplace();
    }
    character_.Apply(action.character_action);
    level_->Apply(action.level_action);
}

void Game::NextLevel() noexcept {
    level_.emplace();
    campaign_.NextDay();
}

bool Game::Save() const noexcept { return false; }
}  // namespace arigato::core
