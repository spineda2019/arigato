
#include <Window.hpp>

int main() noexcept {
    using Window = arigato::display::Window;
    using Frame = Window::Frame;
    const arigato::display::Window game_window{800, 450, 60, "Arigato!"};
    while (game_window) {
        const Frame frame{game_window.MakeFrame()};
        frame.SetBackground(Window::Color::White);
        frame.DrawText("Congrats! You created your first window!", 190, 200, 20,
                       Window::Color::LightGray);
    }
}
