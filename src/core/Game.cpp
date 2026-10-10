// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <memory>
#include <type_traits>
//
#include <arigato/input.hpp>
//
#include "arigato/physics.hpp"
#include "include/Game.hpp"
#include "include/Level.hpp"

namespace arigato::core {
namespace {
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
      level_{campaign_.GetDecor(), campaign_.GetCats(), bounds},
      ecs_{},
      level_bounds_{bounds},
      state_{Game::State::Title} {
    for (Level::PlacedCat const& cat : level_.GetPlacedCats()) {
        (void)ecs_.SpawnStatic(cat.rect.template Convert<float>());
    }
    for (Level::PlacedDecorum const& decorum : level_.GetPlacedDecor()) {
        (void)ecs_.SpawnStatic(decorum.rect.template Convert<float>());
    }
    (void)ecs_.SpawnPlayer(Game::initial_player_body, Game::dyn_body_speed);
}

Game::Entities::Static Game::GetStatics() noexcept {
    return {
        .cats{level_.GetPlacedCats()},
        .decor{level_.GetPlacedDecor()},
    };
}

Game::Entities::Dynamic Game::GetDynamics() noexcept {
    return {.character{ecs_.GetPlayerBody()}};
}

Game::Entities Game::GetPositions() noexcept {
    return {
        .statics{this->GetStatics()},
        .dynamics{this->GetDynamics()},
    };
}

std::size_t Game::GetCurrentDay() const noexcept { return campaign_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    /// Yes this throws. Yes this would terminate the program. I am (currently)
    /// OK with that.
    return level_.GetCustomersLeft();
}

void Game::RebuildLevel() noexcept {
    // Begins a new object lifetime
    std::destroy_at(&level_);
    std::construct_at(&level_, campaign_.GetDecor(), campaign_.GetCats(),
                      level_bounds_);

    // Static bodies are registered cats-first, then decor, so that collision
    // clamping happens in the same order as before.
    ecs_.Clear();
    for (Level::PlacedCat const& cat : level_.GetPlacedCats()) {
        (void)ecs_.SpawnStatic(cat.rect.template Convert<float>());
    }
    for (Level::PlacedDecorum const& decorum : level_.GetPlacedDecor()) {
        (void)ecs_.SpawnStatic(decorum.rect.template Convert<float>());
    }
    (void)ecs_.SpawnPlayer(Game::initial_player_body, Game::dyn_body_speed);
}

void Game::Update(input::Input intent, float dt) noexcept {
    // Input + movement + collision for every body in the level
    ecs_.Update(intent, dt);
    level_.Apply({.served = intent.pressed.space});
}

void Game::NextLevel() noexcept {
    this->RebuildLevel();
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

float Game::GetCharacterWidth() const noexcept {
    return ecs_.GetPlayerBody().bounds.width;
}

}  // namespace arigato::core
