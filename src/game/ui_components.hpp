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

struct ScreenStratification final {
    int col_count{};
    int row_count{};
};
static_assert(std::is_trivially_destructible_v<ScreenStratification>);
static_assert(std::is_nothrow_destructible_v<ScreenStratification>);
static_assert(
    std::is_trivially_constructible_v<ScreenStratification, int, int>);
static_assert(std::is_nothrow_constructible_v<ScreenStratification, int, int>);

template <ScreenStratification layout>
struct ScreenStrata final {
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

    struct Span final {
        int col_span{1};
        int row_span{1};
    };
    static_assert(std::is_trivially_destructible_v<Span>);
    static_assert(std::is_nothrow_destructible_v<Span>);
    static_assert(std::is_trivially_constructible_v<Span, int, int>);
    static_assert(std::is_nothrow_constructible_v<Span, int, int>);

    template <Index target_index, Padding padding = {}, Span span = {}>
    constexpr Button MakeButton(char const* label) const noexcept {
        static_assert(target_index.col <= layout.col_count, "OOB column");
        static_assert(target_index.row <= layout.row_count, "OOB row");

        struct ColInfo final {
            int width{};
            int x{};
        };
        static_assert(std::is_trivially_destructible_v<ColInfo>);
        static_assert(std::is_nothrow_destructible_v<ColInfo>);
        static_assert(std::is_trivially_constructible_v<ColInfo, int, int>);
        static_assert(std::is_nothrow_constructible_v<ColInfo, int, int>);

        const auto col_info{[](int w) noexcept -> ColInfo {
            const int cell_width{w / layout.col_count};
            int full_width{cell_width * span.col_span};
            int no_pad_x{target_index.col * cell_width};
            [[likely]]
            if (padding.left + padding.right < w) {
                full_width -= padding.left;
                full_width -= padding.right;
                no_pad_x += padding.left;
            }
            return {.width = full_width, .x = no_pad_x};
        }(screen_width)};
        const int row_height{[](int h) noexcept -> int {
            int full{(h / layout.row_count) * span.row_span};
            [[likely]]
            if (padding.top + padding.down < h) {
                full -= padding.left;
                full -= padding.right;
            }
            return full;
        }(screen_height)};

        return Button{
            .rectangle{.x = col_info.x,
                       .y = target_index.row * row_height,
                       .width = col_info.width,
                       .height = row_height},
            .label = label,
        };
    }

    int screen_width{};
    int screen_height{};
};

}  // namespace arigato

#endif  // SRC_GAME_UI_COMPONENTS_HPP_
