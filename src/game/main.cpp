
#include <Character.hpp>
#include <Sprite.hpp>
#include <Window.hpp>

#include "assets.hpp"

namespace {
using Window = arigato::display::Window;
using Frame = Window::Frame;
using Character = arigato::core::Character;
using Action = Character::Action;

arigato::core::Character::Action Translate(Window::Keys keys) noexcept {
    constexpr auto help = [](bool neg, bool pos) -> Action::Direction {
        if (neg) {
            if (pos) {
                return Action::Direction::Zero;
            } else {
                return Action::Direction::Negative;
            }
        } else {
            if (pos) {
                return Action::Direction::Positive;
            } else {
                return Action::Direction::Zero;
            }
        }
    };

    return Action{
        .move_x = help(keys.left, keys.right),
        .move_y = help(keys.up, keys.down),
    };
}
}  // namespace

int main() noexcept {
    const arigato::display::Window game_window{800, 450, 60, "Arigato!"};

    arigato::display::Sprite player_stand{arigato::sprites::player_left};
    Character player{};
    while (game_window) {
        const Frame frame{game_window.MakeFrame()};

        const Window::Keys keys{frame.GetKeys()};
        const Action action{Translate(keys)};
        player.Apply(action, game_window.DeltaTime());
        const auto pos{player.GetPosition()};

        frame.SetBackground(Window::BackgroundColor::White);
        frame.DrawSprite(player_stand, static_cast<const int>(pos.x),
                         static_cast<const int>(pos.y));
    }
}
