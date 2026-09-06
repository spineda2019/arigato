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
}  // namespace

int main() noexcept {
    Game game{};

    const Window game_window{800, 450, 60, "Arigato!"};
    Sprite player_stand{arigato::sprites::player_left};

    while (game_window) {
        const Window::Keys keys{game_window.GetKeys()};
        const Character::Action action{Translate(keys)};
        game.Update(action, game_window.DeltaTime());
        const Vec2D pos{game.GetPlayerPosition()};

        const Window::Frame frame{game_window.MakeFrame()};
        frame.SetBackground(Window::BackgroundColor::White);
        frame.DrawSprite(player_stand, pos.x, pos.y);
        int hud_y{0};
        constexpr int font_height{20};
        frame.DrawText("Day 0", 0, hud_y, font_height,
                       Window::BackgroundColor::LightGray);
        hud_y += font_height;
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
    }

    (void)game.Save();
}
