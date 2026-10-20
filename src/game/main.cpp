// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// main.cpp - the actual game implementation
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <array>
#include <cstdlib>
#include <format>
//
#include <Game.hpp>
#include <Level.hpp>
#include <Sprite.hpp>
#include <SpriteManager.hpp>
#include <Window.hpp>
#include <arigato/input.hpp>
#include <arigato/meta.hpp>
#include <zig.hpp>
//
#include "ECS.hpp"
#include "arigato/id.hpp"
#include "arigato/physics.hpp"
#include "assets.hpp"
#include "build_flags.hpp"
#include "ui_components.hpp"

namespace arigato {
namespace {
namespace screen_layouts {
using LevelScreen = arigato::ScreenStrata<{.col_count = 32, .row_count = 18}>;
static_assert(std::is_trivially_destructible_v<LevelScreen>);
static_assert(std::is_trivially_constructible_v<LevelScreen, int, int>);

using TitleScreen = arigato::ScreenStrata<{.col_count = 10, .row_count = 10}>;
static_assert(std::is_trivially_destructible_v<TitleScreen>);
static_assert(std::is_trivially_constructible_v<TitleScreen, int, int>);
}  // namespace screen_layouts

using Window = display::Window;
//
using Level = core::Level;

struct Arigato final {
    display::Window window;
    core::Game game;
    display::SpriteManager sprite_manager;

    inline constexpr void TitleScreen(this auto& arigato) noexcept {
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
        frame.SetBackgroundRGB(
            Window::RGB{.red = 114, .green = 165, .blue = 82});
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
        frame.DrawSpriteRegion(
            arigato.sprite_manager.Get(assets::cafe.asset_path),
            assets::cafe.sub_area,
            cafe_dest_rectangle.template Convert<float>());

        const auto title_rect{
            screen_layout
                .MakeRectangle<{.col = 4, .row = 3},
                               {.left = 0, .top = 0, .right = 0, .down = 5}>()};
        frame.DrawText("Arigato!", title_rect.pos.x, title_rect.pos.y,
                       title_rect.bounds.height,
                       Window::RGB{.red = 41, .green = 71, .blue = 62});

        if (new_game_button.Clicked(mouse)) {
            arigato.game.StartNewCampaign();
        } else if (load_game_button.Clicked(mouse)) {
            arigato.game.LoadExistingCampaign();
        }
    }

    inline constexpr void ProgressLevel(this auto& arigato) noexcept {
        // Input+Sim
        arigato.game.Update(arigato.window.GetInput(),
                            arigato.window.DeltaTime());

        // Render

        const screen_layouts::LevelScreen screen_layout{
            .screen_width = arigato.window.GetWidth(),
            .screen_height = arigato.window.GetHeight(),
        };

        const Window::Frame frame{arigato.window.MakeFrame()};
        frame.SetBackgroundRGB({.red = 165, .green = 115, .blue = 75});

        struct SpriteDrawInfo final {
            assets::SpriteInfo sprite_info;
            types::Rectangle<float> bounds;
        };

        for (const core::ECS::Drawable drawable : arigato.game.GetDrawables()) {
            const SpriteDrawInfo info{
                [](core::ECS::Drawable const* body,
                   decltype(screen_layout) layout) noexcept -> SpriteDrawInfo {
                    SpriteDrawInfo info{
                        .sprite_info{},
                        .bounds{
                            .pos{
                                .x = static_cast<float>(body->rect.pos.x *
                                                        layout.CellWidth()),
                                .y = static_cast<float>(body->rect.pos.y *
                                                        layout.CellHeight()),
                            },
                            .bounds{
                                .width =
                                    static_cast<float>(body->rect.bounds.width *
                                                       layout.CellWidth()),
                                .height = static_cast<float>(
                                    body->rect.bounds.height *
                                    layout.CellHeight()),
                            },
                        },
                    };
                    switch (body->sprite_id) {
                        case arigato::id::SpriteId::Kitters:
                            info.sprite_info.sub_area =
                                assets::cats::white::right_1.sub_area;
                            info.sprite_info.asset_path =
                                assets::cats::white::right_1.asset_path;
                            break;
                        case arigato::id::SpriteId::CafeBar:
                            info.sprite_info.sub_area =
                                assets::cafe_bar.sub_area;
                            info.sprite_info.asset_path =
                                assets::cafe_bar.asset_path;
                            break;
                        case arigato::id::SpriteId::Player:
                            info.sprite_info.sub_area =
                                assets::player_right.sub_area;
                            info.sprite_info.asset_path =
                                assets::player_right.asset_path;
                            break;
                    }

                    return info;
                }(&drawable, screen_layout)};

            frame.DrawSpriteRegion(
                arigato.sprite_manager.Get(info.sprite_info.asset_path),
                info.sprite_info.sub_area, info.bounds);
            if constexpr (draw_hotboxes) {
                frame.DrawHitbox(info.bounds, {.red = 255, .green{}, .blue{}});
            }
        }

        if constexpr (report_framerate) {
            const int real_fps{arigato.window.GetFPS()};
            const auto fmt{std::format("FPS: {}", real_fps)};
            const auto fps_rect{screen_layout.MakeRectangle<
                {.col = 0, .row = 0},
                {.left = 0, .top = 0, .right = 0, .down = 5}>()};
            frame.DrawText(fmt.c_str(), fps_rect.pos.x, fps_rect.pos.y,
                           fps_rect.bounds.height,
                           Window::BackgroundColor::LightGray);
        }
        const auto day{arigato.game.GetCurrentDay()};
        const auto day_fmt{std::format("Day {}", day)};
        const auto day_rect{
            screen_layout
                .MakeRectangle<{.col = 0, .row = 1},
                               {.left = 0, .top = 0, .right = 0, .down = 5}>()};
        frame.DrawText(day_fmt.c_str(), day_rect.pos.x, day_rect.pos.y,
                       day_rect.bounds.height,
                       Window::BackgroundColor::LightGray);

        const auto customers_left{arigato.game.GetCustomersLeft()};
        const auto cust_fmt{std::format("Customers left: {}", customers_left)};
        const auto cust_rect{
            screen_layout
                .MakeRectangle<{.col = 0, .row = 2},
                               {.left = 0, .top = 0, .right = 0, .down = 5}>()};
        frame.DrawText(cust_fmt.c_str(), cust_rect.pos.x, cust_rect.pos.y,
                       cust_rect.bounds.height,
                       Window::BackgroundColor::LightGray);

        if (customers_left == 0) {
            arigato.game.FinishLevel();
        }
    }
    inline constexpr void LevelTransition(this auto& arigato) noexcept {
        constexpr Button next_level_button{
            .rectangle{
                .pos{.x = 10, .y = 100},
                .bounds{.width = 160, .height = 80},
            },
            .label = "Next Level",
        };
        const auto frame{arigato.window.MakeFrame()};
        frame.SetBackgroundRGB(
            Window::RGB{.red = 114, .green = 165, .blue = 82});
        frame.DrawText("You beat the level!", 0, 0, 20,
                       Window::BackgroundColor::LightGray);
        frame.DrawRectangle(next_level_button.label,
                            next_level_button.rectangle,
                            Window::BackgroundColor::LightGray);
        const auto mouse{arigato.window.GetMouse()};
        if (next_level_button.Clicked(mouse)) {
            arigato.game.NextLevel();
        }
    }
};

}  // anonymous namespace
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
        .game{{
            .width =
                arigato::screen_layouts::LevelScreen::layout_info.col_count,
            .height =
                arigato::screen_layouts::LevelScreen::layout_info.row_count,
        }},
        .sprite_manager{texture_files},
    };

    using GameState = arigato::core::Game::State;

    // We have asserts!
    // arigato::core::zig::zig_assert(false);

    while (arigato.window) {
        switch (arigato.game.GetState()) {
            case GameState::Title:
                arigato.TitleScreen();
                break;
            case GameState::Playing:
                arigato.ProgressLevel();
                break;
            case GameState::BetweenLevels:
                arigato.LevelTransition();
                break;
        }
    }

    (void)arigato.game.Save();

    return EXIT_SUCCESS;
}
