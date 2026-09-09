// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// ui_components.hpp - central UI components too specialized for the core lib
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_GAME_UI_COMPONENTS_HPP_
#define SRC_GAME_UI_COMPONENTS_HPP_

#include <cstddef>
#include <type_traits>
//
#include <Window.hpp>

namespace arigato {
struct ScreenRectangle final {
    /// Top left
    float x{};
    /// Top left
    float y{};
    float width{};
    float height{};
};

static_assert(std::is_trivially_destructible_v<ScreenRectangle>);
static_assert(std::is_nothrow_destructible_v<ScreenRectangle>);
static_assert(std::is_trivially_constructible_v<ScreenRectangle, float, float,
                                                float, float>);
static_assert(std::is_nothrow_constructible_v<ScreenRectangle>);

struct Button final {
    ScreenRectangle rectangle{};
    const char* label{};

    /// Defined here for convenience to avoid multiple TUs in the game-glue
    /// project. Should only be included once by main.cpp anyway
    inline bool Clicked(display::Window::Mouse const& mouse) const noexcept {
        return mouse.clicked && mouse.x >= rectangle.x &&
               mouse.x < rectangle.x + rectangle.width &&
               mouse.y >= rectangle.y &&
               mouse.y < rectangle.y + rectangle.height;
    }
};

template <int cols, int rows>
struct ScreenStrata final {
    constexpr explicit ScreenStrata(int screen_width,
                                    int screen_height) noexcept
        : screen_width_{screen_width}, screen_height_{screen_height} {}

    template <int target_col, int target_row>
    constexpr Button MakeButton(char const* label) const noexcept {
        static_assert(target_col <= cols, "OOB column");
        static_assert(target_row <= rows, "OOB row");

        const int col_width{screen_width_ / cols};
        const int row_height{screen_height_ / rows};

        return Button{
            .rectangle{.x = static_cast<float>(target_col * col_width),
                       .y = static_cast<float>(target_row * row_height),
                       .width = static_cast<float>(col_width),
                       .height = static_cast<float>(row_height)},
            .label = label,
        };
    }

 private:
    int screen_width_{};
    int screen_height_{};
};

}  // namespace arigato

#endif  // SRC_GAME_UI_COMPONENTS_HPP_
