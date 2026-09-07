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
namespace assets {
struct SpriteInfo final {
    /// Sub-region of the sprite associated with this object. For example,
    /// Many sprites may come from the same sprite_sheet_ (same path as well)
    /// and only differ by the region in the sheet they inhabit. Should match
    /// the image size if this sprite represents an entire sprite on its own.
    display::Sprite::Area sub_area{};
    /// Path (relative to the deployed executable) to the asset on disk
    char const* asset_path{};
};

static inline constexpr SpriteInfo player_right{
    .sub_area{
        .x = 0,
        .y = 0,
        .width = 47,
        .height = 62,
    },
    .asset_path = "assets/sprites/player/stand.png",
};

static inline constexpr SpriteInfo cafe{
    .sub_area{
        .x = 200,
        .y = 5,
        .width = 186,
        .height = 95,
    },
    .asset_path = "assets/spritesheets/areas/coffeeshop.png",
};
}  // namespace assets
}  // namespace arigato

#endif  // SRC_GAME_ASSETS_HPP_
