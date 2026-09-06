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

enum class GameState : std::uint8_t {
    Title,
    Playing,
};

GameState ProgressGame(Window const& game_window, Game& game,
                       Sprite& player_stand) noexcept {
    const Window::Keys keys{game_window.GetKeys()};
    const Character::Action action{Translate(keys)};
    game.Update(action, game_window.DeltaTime());
    const Vec2D pos{game.GetPlayerPosition()};

    const Window::Frame frame{game_window.MakeFrame()};
    frame.SetBackground(Window::BackgroundColor::White);
    frame.DrawSprite(player_stand, pos.x, pos.y);
    int hud_y{0};
    constexpr int font_height{20};
    if constexpr (debug_build) {
        const int real_fps{game_window.GetFPS()};
        const auto fmt{std::format("FPS: {}", real_fps)};
        frame.DrawText(fmt.c_str(), 0, hud_y, font_height,
                       Window::BackgroundColor::LightGray);
        hud_y += font_height;
    }
    const auto day{game.GetCurrentDay()};
    const auto day_fmt{std::format("Day {}", day)};
    frame.DrawText(day_fmt.c_str(), 0, hud_y, font_height,
                   Window::BackgroundColor::LightGray);
    hud_y += font_height;

    return GameState::Playing;
}

GameState TitleScreen(Window const& window) noexcept {
    constexpr Button new_game_button{
        .x = 300.0f,
        .y = 200.0f,
        .width = 200.0f,
        .height = 400.f,
        .label = "New Game",
    };
    constexpr Button load_game_button{
        .x = 300.0f,
        .y = 260.0f,
        .width = 200.0f,
        .height = 400.f,
        .label = "Load Game",
    };

    const auto mouse{window.GetMouse()};

    const Window::Frame frame{window.MakeFrame()};
    frame.SetBackground(Window::BackgroundColor::White);
    frame.DrawText(new_game_button.label,
                   static_cast<const int>(new_game_button.x),
                   static_cast<const int>(new_game_button.y), 20,
                   Window::BackgroundColor::LightGray);
    frame.DrawText(load_game_button.label,
                   static_cast<const int>(load_game_button.x),
                   static_cast<const int>(load_game_button.y), 20,
                   Window::BackgroundColor::LightGray);

    if (new_game_button.Clicked(mouse)) {
        return GameState::Playing;
    } else {
        return GameState::Title;
    }
}
}  // namespace

int main() noexcept {
    Game game{};
    GameState state{GameState::Title};

    const Window game_window{800, 450, 60, "Arigato!"};
    Sprite player_stand{arigato::sprites::player_left};

    while (game_window) {
        switch (state) {
            case GameState::Title:
                state = TitleScreen(game_window);
                break;
            case GameState::Playing:
                state = ProgressGame(game_window, game, player_stand);
                break;
        }
    }

    (void)game.Save();
}
