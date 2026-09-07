#include <cstdint>
#include <format>
//
#include <Character.hpp>
#include <Game.hpp>
#include <Sprite.hpp>
#include <Vec.hpp>
#include <Window.hpp>
//
#include "./assets.hpp"

namespace {
using Window = arigato::display::Window;
using Sprite = arigato::display::Sprite;

using Character = arigato::core::Character;
using Vec2D = arigato::core::Vec2D;
using Game = arigato::core::Game;

inline constexpr bool debug_build{
#ifdef ARIGATO_DEBUG
    true
#else
    false
#endif
};

struct Button final {
    /// Top left of button
    float x{};
    /// Top left of button
    float y{};
    float width{};
    float height{};
    const char* label;

    bool Clicked(Window::Mouse const& mouse) const noexcept {
        return mouse.clicked && mouse.x >= x && mouse.x < x + width &&
               mouse.y >= y && mouse.y < y + height;
    }
};

enum class GameState : std::uint8_t {
    Title,
    Playing,
    BetweenLevels,
};

struct Arigato final {
    GameState state;
    Window window;
    Game game;
    /// TODO(SEP): Replace with some type of asset manager
    Sprite player;
};

Character::Action Translate(Window::Keys keys) noexcept {
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
    };
}

GameState ProgressLevel(Arigato& game) noexcept {
    const Window::Keys keys{game.window.GetKeys()};
    const Character::Action action{Translate(keys)};
    game.game.Update(action, game.window.DeltaTime());
    const Vec2D pos{game.game.GetPlayerPosition()};

    const Window::Frame frame{game.window.MakeFrame()};
    frame.SetBackground(Window::BackgroundColor::White);
    frame.DrawSprite(game.player, pos.x, pos.y);
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
        .x = 10.0f,
        .y = 100.0f,
        .width = 160.0f,
        .height = 80.0f,
        .label = "Next Level",
    };
    const auto frame{game.window.MakeFrame()};
    frame.SetBackground(Window::BackgroundColor::White);
    frame.DrawText("You beat the level!", 0, 0, 20,
                   Window::BackgroundColor::LightGray);
    const auto mouse{game.window.GetMouse()};
    if (next_level_button.Clicked(mouse)) {
        game.game.NextLevel();
        return GameState::Playing;
    } else {
        return GameState::BetweenLevels;
    }
}

GameState TitleScreen(Arigato const& game) noexcept {
    constexpr Button new_game_button{
        .x = 10.0f,
        .y = 100.0f,
        .width = 160.0f,
        .height = 80.0f,
        .label = "New Game",
    };
    constexpr Button load_game_button{
        .x = 10.0f,
        .y = new_game_button.y + new_game_button.height + 10.0f,
        .width = 160.0f,
        .height = 80.0f,
        .label = "Load Game",
    };

    const auto mouse{game.window.GetMouse()};

    const Window::Frame frame{game.window.MakeFrame()};
    frame.SetBackground(Window::BackgroundColor::White);
    frame.DrawRectangle(new_game_button.label,
                        static_cast<const int>(new_game_button.x),
                        static_cast<const int>(new_game_button.y),
                        static_cast<const int>(new_game_button.width),
                        static_cast<const int>(new_game_button.height),
                        Window::BackgroundColor::LightGray);
    frame.DrawRectangle(load_game_button.label,
                        static_cast<const int>(load_game_button.x),
                        static_cast<const int>(load_game_button.y),
                        static_cast<const int>(load_game_button.width),
                        static_cast<const int>(load_game_button.height),
                        Window::BackgroundColor::LightGray);

    if (new_game_button.Clicked(mouse)) {
        return GameState::Playing;
    } else {
        return GameState::Title;
    }
}
}  // namespace

int main() noexcept {
    Arigato game{
        .state = GameState::Title,
        .window{800, 450, 60, "Arigato!"},
        .game{},
        .player{arigato::sprites::player_left},
    };

    while (game.window) {
        switch (game.state) {
            case GameState::Title:
                game.state = TitleScreen(game);
                break;
            case GameState::Playing:
                game.state = ProgressLevel(game);
                break;
            case GameState::BetweenLevels:
                game.state = LevelTransition(game);
                break;
        }
    }

    (void)game.game.Save();
}
