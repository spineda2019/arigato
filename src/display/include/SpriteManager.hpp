// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// SpriteManager.hpp - Utility to manage all game sprite/spritesheets
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_DISPLAY_INCLUDE_SPRITEMANAGER_HPP_
#define SRC_DISPLAY_INCLUDE_SPRITEMANAGER_HPP_

#include <filesystem>
#include <memory>
#include <span>
#include <type_traits>
#include <vector>

#include "./Sprite.hpp"

namespace arigato::display {
class SpriteManager final {
 public:
    explicit SpriteManager() noexcept;
    explicit SpriteManager(std::span<char const* const> paths) noexcept;
    Sprite& Get(char const* path) noexcept;

 public:  // rule of 5
    ~SpriteManager() noexcept = default;
    SpriteManager(SpriteManager&&) noexcept = default;
    SpriteManager& operator=(SpriteManager&&) noexcept = default;
    SpriteManager(SpriteManager const&) noexcept = delete;
    SpriteManager& operator=(SpriteManager const&) noexcept = delete;

 private:  // types
    struct SpriteId final {
        std::filesystem::path path{};
        std::unique_ptr<Sprite> sprite{};
        bool loaded{};
    };

 private:
    std::vector<SpriteId> managed_sprites_{};
};

static_assert(std::is_move_assignable_v<SpriteManager>);
static_assert(std::is_move_constructible_v<SpriteManager>);

static_assert(!std::is_copy_assignable_v<SpriteManager>,
              "SpriteManager shall be move-only");
static_assert(!std::is_copy_constructible_v<SpriteManager>,
              "SpriteManager shall move-only");
}  // namespace arigato::display

#endif  // SRC_DISPLAY_INCLUDE_SPRITEMANAGER_HPP_
