
#include "Window.hpp"

int main() noexcept {
    //
    const arigato::Window<800, 450, 60> game_window{"Arigato!"};
    while (game_window.IsRunning()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20,
                 LIGHTGRAY);

        EndDrawing();
    }
}
