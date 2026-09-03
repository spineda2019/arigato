extern "C" {
#include <raylib.h>
}

#include "include/Window.hpp"

namespace arigato {

Window::Window(int width, int height, int fps, const char* title) noexcept {
    InitWindow(width, height, title);
    SetTargetFPS(fps);
}

bool Window::IsRunning() const noexcept { return !WindowShouldClose(); }

void Window::Draw() const noexcept {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    EndDrawing();
}

Window::~Window() noexcept { CloseWindow(); }
}  // namespace arigato
