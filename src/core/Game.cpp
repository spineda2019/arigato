// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <optional>
//
#include <arigato/input.hpp>
#include <arigato/meta.hpp>
//
#include "include/Game.hpp"
#include "include/Level.hpp"

namespace arigato::core {
namespace {
constexpr Game::Action InputToGameAction(
    arigato::meta::EfficientFuncArgType<input::Input>::type input,
    float dt) noexcept {
    std::uint8_t amount_served{};
    if (input.pressed.space) {
        ++amount_served;
    }

    const auto character_x_direction{
        [](bool left, bool right) noexcept -> Character::Action::Direction {
            auto dir{Character::Action::Direction::Zero};

            if (left && !right) {
                dir = Character::Action::Direction::Negative;
            } else if (right && !left) {
                dir = Character::Action::Direction::Positive;
            }

            return dir;
        }(input.held.left, input.held.right)};

    const auto character_y_direction{
        [](bool up, bool down) noexcept -> Character::Action::Direction {
            auto dir{Character::Action::Direction::Zero};

            if (up && !down) {
                dir = Character::Action::Direction::Negative;
            } else if (down && !up) {
                dir = Character::Action::Direction::Positive;
            }

            return dir;
        }(input.held.up, input.held.down)};

    return {
        .character_action{
            .move_x = character_x_direction,
            .move_y = character_y_direction,
            .dt = dt,
        },
        .level_action{.amount_served = amount_served},
    };
}
}  // namespace

Game::Game() noexcept
    : campaign_{},
      level_{std::nullopt},
      character_{},
      state_{Game::State::Title} {}

Character::Vec2D Game::GetPlayerPosition() const noexcept {
    return character_.GetPosition();
}
std::size_t Game::GetCurrentDay() const noexcept { return campaign_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    /// Yes this throws. Yes this would terminate the program. I am (currently)
    /// OK with that.
    return level_->GetCustomersLeft();
}

void Game::Update(input::Input intent, float dt) noexcept {
    if (!level_.has_value()) [[unlikely]] {
        // TODO(SEP): Somehow get campaign info into a format Level recognizes
        // and construct it with that info (e.g. what decor can the level
        // arrange)
        level_.emplace();
    }
    const Game::Action action{InputToGameAction(intent, dt)};
    character_.Apply(action.character_action);
    level_->Apply(action.level_action);
}

void Game::NextLevel() noexcept {
    level_.emplace();
    campaign_.NextDay();
    state_ = Game::State::Playing;
}

Game::State Game::GetState() const noexcept { return state_; }

void Game::StartNewCampaign() noexcept {
    // TODO(SEP) perhaps new game specifics?
    state_ = Game::State::Playing;
}

void Game::FinishLevel() noexcept { state_ = Game::State::BetweenLevels; }

bool Game::Save() const noexcept {
    return false;
    (void)state_;
}
}  // namespace arigato::core
