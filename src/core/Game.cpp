// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <optional>
#include <type_traits>
//
#include <arigato/input.hpp>
#include <arigato/meta.hpp>
//
#include "include/Game.hpp"
#include "include/Level.hpp"

namespace arigato::core {
namespace {
constexpr Game::Action InputToGameAction(
    arigato::meta::EfficientFuncArgType<input::Input>::type input) noexcept {
    using MoveDirection = Game::Entities::Dynamic::Action::Direction;

    const auto character_x_direction{
        [](bool left, bool right) noexcept -> MoveDirection {
            auto dir{MoveDirection::Zero};

            if (left && !right) {
                dir = MoveDirection::Negative;
            } else if (right && !left) {
                dir = MoveDirection::Positive;
            }

            return dir;
        }(input.held.left, input.held.right)};

    const auto character_y_direction{
        [](bool up, bool down) noexcept -> MoveDirection {
            auto dir{MoveDirection::Zero};

            if (up && !down) {
                dir = MoveDirection::Negative;
            } else if (down && !up) {
                dir = MoveDirection::Positive;
            }

            return dir;
        }(input.held.up, input.held.down)};

    return {
        .character_action{
            .move_x = character_x_direction,
            .move_y = character_y_direction,
        },
        .level_action{.served = input.pressed.space},
    };
}

template <class T>
    requires std::is_arithmetic_v<T>
constexpr T NoThrowRClamp(T val, T hi) noexcept {
    return val <= hi ? val : hi;
}

template <class T>
    requires std::is_arithmetic_v<T>
constexpr T NoThrowLClamp(T val, T lo) noexcept {
    return val >= lo ? val : lo;
}

template <class T>
    requires std::is_arithmetic_v<T>
constexpr T NoThrowClamp(T val, T lo, T hi) noexcept {
    return NoThrowRClamp<T>(NoThrowLClamp<T>(val, lo), hi);
}

static_assert(NoThrowClamp<unsigned char>(2, 1, 3) == 2);
static_assert(NoThrowClamp<unsigned char>(0, 1, 3) == 1);
static_assert(NoThrowClamp<unsigned char>(4, 1, 3) == 3);

}  // namespace

Game::Game(Game::Bounds bounds) noexcept
    : campaign_{},
      level_{std::nullopt},
      character_{{.pos{}, .bounds{.width = 1, .height = 1}}},
      level_bounds_{bounds},
      state_{Game::State::Title} {}

Game::Entities::Static Game::GetStatics() const noexcept {
    return {
        .cats{level_->GetPlacedCats()},
        .decor{level_->GetPlacedDecor()},
    };
}

Game::Entities::Dynamic Game::GetDynamics() const noexcept {
    return {.character{character_.GetPosition()}};
}

Game::Entities Game::GetPositions() const noexcept {
    return {
        .statics{this->GetStatics()},
        .dynamics{this->GetDynamics()},
    };
}

std::size_t Game::GetCurrentDay() const noexcept { return campaign_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    /// Yes this throws. Yes this would terminate the program. I am (currently)
    /// OK with that.
    return level_->GetCustomersLeft();
}

void Game::Update(input::Input intent, float dt) noexcept {
    if (!level_.has_value()) [[unlikely]] {
        level_.emplace(campaign_.GetDecor(), campaign_.GetCats(),
                       level_bounds_);
    }
    const Game::Action action{InputToGameAction(intent)};

    constexpr int move_speed{7};

    const Game::Entities::Dynamic::Movement character_movement{
        action.character_action.Translate(move_speed, dt)};

    // TODO(SEP): introduce collision handling/clamping
    // const Game::Entities entities{this->GetPositions()};

    character_.Apply(character_movement.delta.x, character_movement.delta.y);
    level_->Apply(action.level_action);
}

void Game::NextLevel() noexcept {
    level_.emplace(campaign_.GetDecor(), campaign_.GetCats(), level_bounds_);
    campaign_.NextDay();
    state_ = Game::State::Playing;
}

Game::State Game::GetState() const noexcept { return state_; }

void Game::StartNewCampaign() noexcept {
    // TODO(SEP) perhaps new game specifics?
    state_ = Game::State::Playing;
}

void Game::LoadExistingCampaign() noexcept {
    // TODO(SEP)
}

void Game::FinishLevel() noexcept { state_ = Game::State::BetweenLevels; }

bool Game::Save() const noexcept {
    return false;
    (void)state_;
}
}  // namespace arigato::core
