
#include <Character.hpp>
#include <Sprite.hpp>
#include <Window.hpp>

#include "./assets.hpp"

namespace {
using Window = arigato::display::Window;
using Character = arigato::core::Character;
using Sprite = arigato::display::Sprite;

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
    const arigato::display::Window game_window{800, 450, 60, "Arigato!"};

    Sprite player_stand{arigato::sprites::player_left};
    Character player{};
    while (game_window) {
        const Window::Keys keys{game_window.GetKeys()};
        const Character::Action action{Translate(keys)};
        player.Apply(action, game_window.DeltaTime());
        const auto pos{player.GetPosition()};

        const Window::Frame frame{game_window.MakeFrame()};
        frame.SetBackground(Window::BackgroundColor::White);
        frame.DrawSprite(player_stand, pos.x, pos.y);
        frame.DrawText("Day 0", 0, 0, 20, Window::BackgroundColor::LightGray);
    }
}
