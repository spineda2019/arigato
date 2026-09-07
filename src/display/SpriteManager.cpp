// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// SpriteManager.cpp - Utility to manage all game sprite/spritesheets
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "include/SpriteManager.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <unordered_set>
#include <utility>
#include <vector>

#include "include/Sprite.hpp"

namespace arigato::display {
SpriteManager::SpriteManager() noexcept : managed_sprites_{} {}
SpriteManager::SpriteManager(std::span<std::filesystem::path> paths) noexcept
    : managed_sprites_{[](std::span<std::filesystem::path> asset_paths) noexcept
                           -> std::vector<SpriteManager::SpriteId> {
          try {
              std::vector<SpriteManager::SpriteId> ids(asset_paths.size());
              std::unordered_set<const char*> encountered_paths(
                  asset_paths.size());
              for (std::filesystem::path p : asset_paths) {
                  // enforce uniqueness of paths to prevent double GPU loading
                  if (const char* native_path{p.c_str()};
                      !encountered_paths.contains(native_path)) {
                      ids.emplace_back(std::move(p),
                                       std::make_unique<Sprite>(native_path),
                                       true);
                      encountered_paths.emplace(native_path);
                  }
              }
              return ids;
          } catch (...) {
              // shouldn't alloc so should be safe for noexcept
              return std::vector<SpriteManager::SpriteId>{};
          }
      }(paths)} {}

Sprite& SpriteManager::Get(std::filesystem::path const& path) noexcept {
    for (SpriteManager::SpriteId const& s_id : managed_sprites_) {
        if (s_id.path == path) {
            return *s_id.sprite;
        }
    }

    managed_sprites_.emplace_back(path, std::make_unique<Sprite>(path.c_str()),
                                  true);
    return *managed_sprites_.back().sprite;
}
}  // namespace arigato::display
