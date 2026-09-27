// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.cpp - Implementation for the Game class. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <algorithm>
#include <concepts>
#include <execution>
#include <optional>
#include <ranges>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>
//
#include <arigato/input.hpp>
#include <arigato/meta.hpp>
//
#include "arigato/physics.hpp"
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
      physics_manager_{},
      character_{{.pos{}, .bounds{.width = 1, .height = 1}}},
      level_bounds_{bounds},
      state_{Game::State::Title} {}

Game::Entities::Static Game::GetStatics() noexcept {
    return {
        .cats{level_->GetPlacedCats()},
        .decor{level_->GetPlacedDecor()},
    };
}

Game::Entities::Dynamic Game::GetDynamics() noexcept {
    return {.character{character_.GetPosition()}};
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
    return level_->GetCustomersLeft();
}

void Game::Update(input::Input intent, float dt) noexcept {
    if (!level_.has_value()) [[unlikely]] {
        level_.emplace(campaign_.GetDecor(), campaign_.GetCats(),
                       level_bounds_);
    }

    // Only affects controllable bodies, like the character
    const Game::Action action{InputToGameAction(intent)};

    const Game::Entities::Dynamic::Movement character_movement{
        action.character_action.Translate(Game::dyn_body_speed, dt)};

    Game::Entities entities{this->GetPositions()};
    entities.dynamics.character.pos.x += character_movement.delta.x;
    entities.dynamics.character.pos.y += character_movement.delta.y;

    auto all_dynamics{entities.dynamics.AllDynamics()};
    const auto all_statics_int{entities.statics.AllStatics()};
    std::vector<std::remove_pointer_t<decltype(all_dynamics)::value_type>>
        all_statics(all_statics_int.size());
    std::transform(std::execution::par_unseq, all_statics_int.begin(),
                   all_statics_int.end(), all_statics.begin(),
                   [](decltype(all_statics_int)::value_type rect) noexcept
                       -> decltype(all_statics)::value_type {
                       return rect.template Convert<float>();
                   });

    physics_manager_.Collide(all_dynamics, all_statics);

    character_.Apply(entities.dynamics.character.pos, {});
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

Game::Entities::Dynamic::Movement Game::Entities::Dynamic::Action::Translate(
    std::uint8_t speed, float dt) const noexcept {
    constexpr auto translate =
        [](Game::Entities::Dynamic::Action::Direction move,
           std::uint8_t speed_arg, float dt_arg) noexcept -> float {
        return static_cast<float>(std::to_underlying(move) * speed_arg) *
               dt_arg;
    };

    return {
        .delta{
            .x = translate(move_x, speed, dt),
            .y = translate(move_y, speed, dt),
        },
    };
}

std::vector<types::Rectangle<int>> Game::Entities::Static::AllStatics() const {
    using Rect = types::Rectangle<int>;

    std::vector<Rect> all_statics(cats.size() + decor.size());

    constexpr auto get_rect = [](auto& rect_owner) noexcept -> Rect
        requires std::same_as<Rect,
                              std::remove_cvref_t<decltype(rect_owner.rect)>>
    { return (rect_owner.rect); };

    auto it{std::transform(
        std::execution::par_unseq, cats.begin(), cats.end(),
        all_statics.begin(),
        [](Level::PlacedCat& cat) noexcept { return cat.rect; })};
    std::transform(std::execution::par_unseq, decor.begin(), decor.end(), it,
                   get_rect);

    return all_statics;
}

std::vector<types::Rectangle<float>*> Game::Entities::Dynamic::AllDynamics() {
    return {&character};
}

}  // namespace arigato::core
