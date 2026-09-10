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
#include <Vec.hpp>
#include <Window.hpp>
//
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

Level::Action Translate(Window::Keys keys) noexcept {
    if (keys.space) {
        return {.amount_served = 1};
    } else {
        return {.amount_served = 0};
    }
}

Character::Action Translate(Window::Keys keys, float dt) noexcept {
    constexpr auto help = [](bool neg,
                             bool pos) -> Character::Action::Direction {
        if (neg) {
            if (pos) {
                return Character::Action::Direction::Zero;
            } else {
                return Character::Action::Direction::Negative;
            }
        } else {
            if (pos) {
                return Character::Action::Direction::Positive;
            } else {
                return Character::Action::Direction::Zero;
            }
        }
    };

    return Character::Action{
        .move_x = help(keys.left, keys.right),
        .move_y = help(keys.up, keys.down),
        .dt = dt,
    };
}

GameState ProgressLevel(Arigato& game) noexcept {
    const Window::Keys keys{game.window.GetKeys()};
    const Character::Action action{Translate(keys, game.window.DeltaTime())};
    const Level::Action level_action{Translate(keys)};
    game.game.Update(action, level_action);
    const core::Vec2D pos{game.game.GetPlayerPosition()};

    const Window::Frame frame{game.window.MakeFrame()};
    frame.SetBackgroundRGB({.red = 165, .green = 115, .blue = 75});

    const int screen_width{game.window.GetWidth()};
    const int screen_height{game.window.GetHeight()};
    frame.DrawSpriteRegion(game.sprite_manager.Get(assets::cafe_bar.asset_path),
                           assets::cafe_bar.sub_area,
                           static_cast<const float>(screen_width) / 2,
                           static_cast<const float>(screen_height) / 2);

    frame.DrawFullSprite(
        game.sprite_manager.Get(assets::player_right.asset_path), pos.x, pos.y);
    int hud_y{0};
    constexpr int font_height{20};
    if constexpr (debug_build) {
        const int real_fps{game.window.GetFPS()};
        const auto fmt{std::format("FPS: {}", real_fps)};
        frame.DrawText(fmt.c_str(), 0, hud_y, font_height,
                       Window::BackgroundColor::LightGray);
        hud_y += font_height;
    }
    const auto day{game.game.GetCurrentDay()};
    const auto day_fmt{std::format("Day {}", day)};
    frame.DrawText(day_fmt.c_str(), 0, hud_y, font_height,
                   Window::BackgroundColor::LightGray);
    hud_y += font_height;

    const auto customers_left{game.game.GetCustomersLeft()};
    const auto cust_fmt{std::format("Customers left: {}", customers_left)};
    frame.DrawText(cust_fmt.c_str(), 0, hud_y, font_height,
                   Window::BackgroundColor::LightGray);
    hud_y += font_height;

    if (customers_left == 0) {
        return GameState::BetweenLevels;
    } else {
        return GameState::Playing;
    }
}

GameState LevelTransition(Arigato& game) noexcept {
    constexpr Button next_level_button{
        .rectangle{
            .x = 10,
            .y = 100,
            .width = 160,
            .height = 80,
        },
        .label = "Next Level",
    };
    const auto frame{game.window.MakeFrame()};
    frame.SetBackgroundRGB(Window::RGB{.red = 114, .green = 165, .blue = 82});
    frame.DrawText("You beat the level!", 0, 0, 20,
                   Window::BackgroundColor::LightGray);
    frame.DrawRectangle(
        next_level_button.label, next_level_button.rectangle.x,
        next_level_button.rectangle.y, next_level_button.rectangle.width,
        next_level_button.rectangle.height, Window::BackgroundColor::LightGray);
    const auto mouse{game.window.GetMouse()};
    if (next_level_button.Clicked(mouse)) {
        game.game.NextLevel();
        return GameState::Playing;
    } else {
        return GameState::BetweenLevels;
    }
}

GameState TitleScreen(Arigato& game) noexcept {
    using Screen = arigato::ScreenStrata<{.col_count = 10, .row_count = 10}>;
    static_assert(std::is_trivially_destructible_v<Screen>);
    static_assert(std::is_nothrow_destructible_v<Screen>);
    static_assert(std::is_trivially_constructible_v<Screen, int, int>);
    static_assert(std::is_nothrow_constructible_v<Screen, int, int>);

    const Screen screen_layout{.screen_width = game.window.GetWidth(),
                               .screen_height = game.window.GetHeight()};

    constexpr Screen::Padding padding{
        .left = 5,
        .top = 5,
        .right = 5,
        .down = 5,
    };
    constexpr Screen::Span span{
        .col_span = 2,
        .row_span = 2,
    };
    const Button new_game_button{
        screen_layout.MakeButton<{.col = 0, .row = 2}, padding, span>(
            "New Game")};
    const Button load_game_button{
        screen_layout.MakeButton<{.col = 0, .row = 4}, padding, span>(
            "Load Game")};

    const auto mouse{game.window.GetMouse()};

    const Window::Frame frame{game.window.MakeFrame()};
    frame.SetBackgroundRGB(Window::RGB{.red = 114, .green = 165, .blue = 82});
    frame.DrawRectangle(
        new_game_button.label, new_game_button.rectangle.x,
        new_game_button.rectangle.y, new_game_button.rectangle.width,
        new_game_button.rectangle.height, Window::BackgroundColor::LightGray);
    frame.DrawRectangle(
        load_game_button.label, load_game_button.rectangle.x,
        load_game_button.rectangle.y, load_game_button.rectangle.width,
        load_game_button.rectangle.height, Window::BackgroundColor::LightGray);

    const int screen_width{game.window.GetWidth()};
    const int screen_height{game.window.GetHeight()};
    const float screen_width_f{static_cast<const float>(screen_width)};
    const float screen_height_f{static_cast<const float>(screen_height)};
    frame.DrawSpriteRegion(
        game.sprite_manager.Get(assets::cafe.asset_path), assets::cafe.sub_area,
        (screen_width_f / 2) - (assets::cafe.sub_area.width / 2),
        (screen_height_f / 2) - (assets::cafe.sub_area.height / 2));

    constexpr int title_size{30};
    frame.DrawText(
        "Arigato!", screen_width / 2,
        (screen_height / 2) -
            static_cast<const int>((assets::cafe.sub_area.height) / 2) -
            title_size - 5,
        title_size, Window::RGB{.red = 41, .green = 71, .blue = 62});

    if (new_game_button.Clicked(mouse)) {
        return GameState::Playing;
    } else {
        return GameState::Title;
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
    arigato::Arigato game{
        .state = arigato::GameState::Title,
        .window{800, 450, 60, "Arigato!"},
        .game{},
        .sprite_manager{texture_files},
    };

    while (game.window) {
        switch (game.state) {
            case arigato::GameState::Title:
                game.state = arigato::TitleScreen(game);
                break;
            case arigato::GameState::Playing:
                game.state = arigato::ProgressLevel(game);
                break;
            case arigato::GameState::BetweenLevels:
                game.state = arigato::LevelTransition(game);
                break;
        }
    }

    (void)game.game.Save();
}
