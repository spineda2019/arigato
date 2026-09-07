// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ui_components.hpp - central UI components too specialized for the core lib
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_GAME_UI_COMPONENTS_HPP_
#define SRC_GAME_UI_COMPONENTS_HPP_

#include <Window.hpp>

namespace arigato {
struct Button final {
    /// Top left of button
    float x{};
    /// Top left of button
    float y{};
    float width{};
    float height{};
    const char* label;

    /// Defined here for convenience to avoid multiple TUs in the game-glue
    /// project. Should only be included once by main.cpp anyway
    inline bool Clicked(display::Window::Mouse const& mouse) const noexcept {
        return mouse.clicked && mouse.x >= x && mouse.x < x + width &&
               mouse.y >= y && mouse.y < y + height;
    }
};
}  // namespace arigato

#endif  // SRC_GAME_UI_COMPONENTS_HPP_
