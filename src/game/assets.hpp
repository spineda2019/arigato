// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// assets.hpp - Helper TU that just holds paths to assets
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_GAME_ASSETS_HPP_
#define SRC_GAME_ASSETS_HPP_

#include <Sprite.hpp>

namespace arigato {
namespace sprites {
inline static constexpr const char* player_left{
    "assets/sprites/player/stand.png"};
}  // namespace sprites
namespace spritesheets {
inline static constexpr const char* cafe{
    "assets/spritesheets/areas/coffeeshop.png"};
}

namespace regions {
static inline constexpr arigato::display::Sprite::Area cafe{
    .x = 200,
    .y = 5,
    .width = 186,
    .height = 95,
};
}
}  // namespace arigato

#endif  // SRC_GAME_ASSETS_HPP_
