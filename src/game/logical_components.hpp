// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// logical_components.hpp - logical game components too specialized for the core
// lib
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_GAME_LOGICAL_COMPONENTS_HPP_
#define SRC_GAME_LOGICAL_COMPONENTS_HPP_

#include <cstdint>
//
#include <Game.hpp>
#include <Sprite.hpp>
#include <SpriteManager.hpp>
#include <Window.hpp>

namespace arigato {
enum class GameState : std::uint8_t {
    Title,
    Playing,
    BetweenLevels,
};

struct Arigato final {
    GameState state;
    display::Window window;
    core::Game game;
    /// TODO(SEP): Replace with some type of asset manager
    display::SpriteManager sprite_manager;
};
}  // namespace arigato

#endif  // SRC_GAME_LOGICAL_COMPONENTS_HPP_
