// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Game.hpp - Representation of the game, its owned entities, and rules
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_GAME_HPP_
#define SRC_CORE_INCLUDE_GAME_HPP_

#include <cstddef>
#include <cstdint>

#include "Campaign.hpp"
#include "Character.hpp"
#include "Level.hpp"

namespace arigato::core {
class Game final {
 public:  // types
    struct Action final {
        Character::Action character_action{};
    };

 public:
    explicit Game() noexcept;

    Character::Vec2D GetPlayerPosition() const noexcept;
    std::size_t GetCurrentDay() const noexcept;
    std::uint8_t GetCustomersLeft() const noexcept;

    void Update(Character::Action, Level::Action) noexcept;

    void NextLevel() noexcept;

    [[nodiscard("Save operations may fail and must be reported")]]
    bool Save() const noexcept;

 private:
    Character character_{};
    Campaign campaign_{};
    Level level_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_GAME_HPP_
