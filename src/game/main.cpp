// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// main.cpp - the actual game implementation
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <array>
#include <format>
//
#include <Character.hpp>
#include <Game.hpp>
#include <Level.hpp>
#include <Sprite.hpp>
#include <Window.hpp>
#include <arigato/input.hpp>
#include <arigato/meta.hpp>
//
#include "arigato/id.hpp"
#include "assets.hpp"
#include "logical_components.hpp"
#include "ui_components.hpp"

namespace arigato {
namespace {
using Window = display::Window;

using Level = core::Level;
using Character = core::Character;

inline constexpr bool debug_build{
#ifdef ARIGATO_DEBUG
    true
#else
    false
#endif
};

namespace screen_layouts {
using LevelScreen = arigato::ScreenStrata<{.col_count = 32, .row_count = 18}>;
static_assert(std::is_trivially_destructible_v<LevelScreen>);
static_assert(std::is_nothrow_destructible_v<LevelScreen>);
static_assert(std::is_trivially_constructible_v<LevelScreen, int, int>);
static_assert(std::is_nothrow_constructible_v<LevelScreen, int, int>);

using TitleScreen = arigato::ScreenStrata<{.col_count = 10, .row_count = 10}>;
static_assert(std::is_trivially_destructible_v<TitleScreen>);
static_assert(std::is_nothrow_destructible_v<TitleScreen>);
static_assert(std::is_trivially_constructible_v<TitleScreen, int, int>);
static_assert(std::is_nothrow_constructible_v<TitleScreen, int, int>);

}  // namespace screen_layouts

void ProgressLevel(Arigato& arigato) noexcept {
    // Input+Sim
    arigato.game.Update(arigato.window.GetInput(), arigato.window.DeltaTime());
    const core::Game::Entities pos{arigato.game.GetPositions()};

    // Render

    const screen_layouts::LevelScreen screen_layout{
        .screen_width = arigato.window.GetWidth(),
        .screen_height = arigato.window.GetHeight(),
    };

    const Window::Frame frame{arigato.window.MakeFrame()};
    frame.SetBackgroundRGB({.red = 165, .green = 115, .blue = 75});

    for (const auto x : pos.cats) {
        // TODO(SEP): Dispath on cat id
        (void)x;
    }

    for (const auto decorum : pos.decor) {
        switch (decorum.decorum) {
            case id::DecorId::CafeBar:
                frame.DrawSpriteRegion(
                    arigato.sprite_manager.Get(assets::cafe_bar.asset_path),
                    assets::cafe_bar.sub_area,
                    {
                        .pos{
                            .x = static_cast<float>(decorum.bounds.pos.x *
                                                    screen_layout.CellWidth()),
                            .y = static_cast<float>(decorum.bounds.pos.y *
                                                    screen_layout.CellHeight()),
                        },
                        .width = static_cast<float>(decorum.bounds.width *
                                                    screen_layout.CellWidth()),
                        .height = static_cast<float>(
                            decorum.bounds.height * screen_layout.CellHeight()),
                    });
                break;
            case id::DecorId::Unknown:
                // TODO(SEP): Log somewhere
                break;
        }
    }

    const display::Sprite::Area player_region{
        .pos{
            .x = pos.character.x * screen_layout.CellWidth(),
            .y = pos.character.y * screen_layout.CellHeight(),
        },
        .width = static_cast<float>(screen_layout.CellWidth() * 2),
        .height = static_cast<float>(screen_layout.CellHeight() * 2),
    };
    frame.DrawFullSprite(
        arigato.sprite_manager.Get(assets::player_right.asset_path),
        player_region);

    if constexpr (debug_build) {
        const int real_fps{arigato.window.GetFPS()};
        const auto fmt{std::format("FPS: {}", real_fps)};
        const auto fps_rect{
            screen_layout
                .MakeRectangle<{.col = 0, .row = 0},
                               {.left = 0, .top = 0, .right = 0, .down = 5}>()};
        frame.DrawText(fmt.c_str(), fps_rect.pos.x, fps_rect.pos.y,
                       fps_rect.height, Window::BackgroundColor::LightGray);
    }
    const auto day{arigato.game.GetCurrentDay()};
    const auto day_fmt{std::format("Day {}", day)};
    const auto day_rect{screen_layout.MakeRectangle<
        {.col = 0, .row = 1}, {.left = 0, .top = 0, .right = 0, .down = 5}>()};
    frame.DrawText(day_fmt.c_str(), day_rect.pos.x, day_rect.pos.y,
                   day_rect.height, Window::BackgroundColor::LightGray);

    const auto customers_left{arigato.game.GetCustomersLeft()};
    const auto cust_fmt{std::format("Customers left: {}", customers_left)};
    const auto cust_rect{screen_layout.MakeRectangle<
        {.col = 0, .row = 2}, {.left = 0, .top = 0, .right = 0, .down = 5}>()};
    frame.DrawText(cust_fmt.c_str(), cust_rect.pos.x, cust_rect.pos.y,
                   cust_rect.height, Window::BackgroundColor::LightGray);

    if (customers_left == 0) {
        arigato.game.FinishLevel();
    }
}

void LevelTransition(Arigato& arigato) noexcept {
    constexpr Button next_level_button{
        .rectangle{
            .pos{.x = 10, .y = 100},
            .width = 160,
            .height = 80,
        },
        .label = "Next Level",
    };
    const auto frame{arigato.window.MakeFrame()};
    frame.SetBackgroundRGB(Window::RGB{.red = 114, .green = 165, .blue = 82});
    frame.DrawText("You beat the level!", 0, 0, 20,
                   Window::BackgroundColor::LightGray);
    frame.DrawRectangle(next_level_button.label, next_level_button.rectangle,
                        Window::BackgroundColor::LightGray);
    const auto mouse{arigato.window.GetMouse()};
    if (next_level_button.Clicked(mouse)) {
        arigato.game.NextLevel();
    }
}

void TitleScreen(Arigato& arigato) noexcept {
    const screen_layouts::TitleScreen screen_layout{
        .screen_width = arigato.window.GetWidth(),
        .screen_height = arigato.window.GetHeight()};

    constexpr screen_layouts::TitleScreen::Padding padding{
        .left = 5,
        .top = 5,
        .right = 5,
        .down = 5,
    };
    constexpr screen_layouts::TitleScreen::Span span{
        .col_span = 2,
        .row_span = 2,
    };
    const Button new_game_button{
        screen_layout.MakeButton<{.col = 0, .row = 2}, padding, span>(
            "New Game")};
    const Button load_game_button{
        screen_layout.MakeButton<{.col = 0, .row = 4}, padding, span>(
            "Load Game")};

    const auto mouse{arigato.window.GetMouse()};

    const Window::Frame frame{arigato.window.MakeFrame()};
    frame.SetBackgroundRGB(Window::RGB{.red = 114, .green = 165, .blue = 82});
    frame.DrawRectangle(new_game_button.label, new_game_button.rectangle,
                        Window::BackgroundColor::LightGray);
    frame.DrawRectangle(load_game_button.label, load_game_button.rectangle,
                        Window::BackgroundColor::LightGray);

    constexpr screen_layouts::TitleScreen::Span cafe_span{
        .col_span = 3,
        .row_span = 3,
    };
    const auto cafe_dest_rectangle{
        screen_layout.MakeRectangle<{.col = 4, .row = 4}, {}, cafe_span>()};
    frame.DrawSpriteRegion(arigato.sprite_manager.Get(assets::cafe.asset_path),
                           assets::cafe.sub_area,
                           cafe_dest_rectangle.template Convert<float>());

    const auto title_rect{screen_layout.MakeRectangle<
        {.col = 4, .row = 3}, {.left = 0, .top = 0, .right = 0, .down = 5}>()};
    frame.DrawText("Arigato!", title_rect.pos.x, title_rect.pos.y,
                   title_rect.height,
                   Window::RGB{.red = 41, .green = 71, .blue = 62});

    if (new_game_button.Clicked(mouse)) {
        arigato.game.StartNewCampaign();
    } else if (load_game_button.Clicked(mouse)) {
        arigato.game.LoadExistingCampaign();
    }
}
}  // namespace
}  // namespace arigato

int main() noexcept {
    // If this gets too big, wrap in a std::unique_ptr to prevent stack-overflow
    constexpr std::array<char const*, 3> texture_files{
        arigato::assets::player_right.asset_path,
        arigato::assets::cafe.asset_path,
        arigato::assets::cafe_bar.asset_path,
    };
    arigato::Arigato arigato{
        .window{800, 450, 60, "Arigato!"},
        .game{},
        .sprite_manager{texture_files},
    };

    using GameState = arigato::core::Game::State;

    while (arigato.window) {
        switch (arigato.game.GetState()) {
            case GameState::Title:
                arigato::TitleScreen(arigato);
                break;
            case GameState::Playing:
                arigato::ProgressLevel(arigato);
                break;
            case GameState::BetweenLevels:
                arigato::LevelTransition(arigato);
                break;
        }
    }

    (void)arigato.game.Save();
}
