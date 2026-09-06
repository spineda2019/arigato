
#include <Window.hpp>

int main() noexcept {
    //
    const arigato::display::Window game_window{800, 450, 60, "Arigato!"};
    while (game_window.IsRunning()) {
        game_window.Draw();
    }
}
