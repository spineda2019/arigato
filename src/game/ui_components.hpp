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
    int x{};
    /// Top left
    int y{};
    int width{};
    int height{};
};

static_assert(std::is_trivially_destructible_v<ScreenRectangle>);
static_assert(std::is_nothrow_destructible_v<ScreenRectangle>);
static_assert(
    std::is_trivially_constructible_v<ScreenRectangle, int, int, int, int>);
static_assert(
    std::is_nothrow_constructible_v<ScreenRectangle, int, int, int, int>);

struct Button final {
    ScreenRectangle rectangle{};
    const char* label{};

    /// Defined here for convenience to avoid multiple TUs in the game-glue
    /// project. Should only be included once by main.cpp anyway
    inline bool Clicked(display::Window::Mouse const& mouse) const noexcept {
        return mouse.clicked &&
               mouse.x >= static_cast<const float>(rectangle.x) &&
               mouse.x < static_cast<const float>(rectangle.x) +
                             static_cast<const float>(rectangle.width) &&
               mouse.y >= static_cast<const float>(rectangle.y) &&
               mouse.y < static_cast<const float>(rectangle.y) +
                             static_cast<const float>(rectangle.height);
    }
};

static_assert(std::is_trivially_destructible_v<Button>);
static_assert(std::is_nothrow_destructible_v<Button>);
static_assert(
    std::is_trivially_constructible_v<Button, ScreenRectangle, char const*>);
static_assert(
    std::is_nothrow_constructible_v<Button, ScreenRectangle, char const*>);

template <int cols, int rows>
struct ScreenStrata final {
    constexpr explicit ScreenStrata(int screen_width,
                                    int screen_height) noexcept
        : screen_width_{screen_width}, screen_height_{screen_height} {}

    struct Padding final {
        int left{};
        int top{};
        int right{};
        int down{};
    };
    static_assert(std::is_trivially_destructible_v<Padding>);
    static_assert(std::is_nothrow_destructible_v<Padding>);
    static_assert(
        std::is_trivially_constructible_v<Padding, int, int, int, int>);
    static_assert(std::is_nothrow_constructible_v<Padding, int, int, int, int>);

    struct Index final {
        int col{};
        int row{};
    };
    static_assert(std::is_trivially_destructible_v<Index>);
    static_assert(std::is_nothrow_destructible_v<Index>);
    static_assert(std::is_trivially_constructible_v<Index, int, int>);
    static_assert(std::is_nothrow_constructible_v<Index, int, int>);

    template <Index target_index, Padding padding = {}>
    constexpr Button MakeButton(char const* label) const noexcept {
        static_assert(target_index.col <= cols, "OOB column");
        static_assert(target_index.row <= rows, "OOB row");

        const int col_width{[](int w) noexcept -> int {
            int full{w / cols};
            [[likely]]
            if (padding.left + padding.right < w) {
                full -= padding.left;
                full -= padding.right;
            }
            return full;
        }(screen_width_)};
        const int row_height{[](int h) noexcept -> int {
            int full{h / rows};
            [[likely]]
            if (padding.top + padding.down < h) {
                full -= padding.left;
                full -= padding.right;
            }
            return full;
        }(screen_height_)};

        return Button{
            .rectangle{.x = static_cast<int>(target_index.col * col_width),
                       .y = static_cast<int>(target_index.row * row_height),
                       .width = static_cast<int>(col_width),
                       .height = static_cast<int>(row_height)},
            .label = label,
        };
    }

 private:
    int screen_width_{};
    int screen_height_{};
};

}  // namespace arigato

#endif  // SRC_GAME_UI_COMPONENTS_HPP_
