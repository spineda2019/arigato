extern "C" {
#include <raylib.h>
}

#include "include/Window.hpp"

namespace arigato::display {
namespace {
constexpr bool debug{
#ifdef ARIGATO_DEBUG
    true
#else
    false
#endif
};

constexpr decltype(RAYWHITE) ToRayColor(Window::Color color) {
    switch (color) {
        case Window::Color::White:
            return RAYWHITE;
        case Window::Color::LightGray:
            return LIGHTGRAY;
    }
}
}  // namespace

Window::Window(int width, int height, int fps, const char* title) noexcept {
    if constexpr (!debug) {
        ::SetTraceLogLevel(LOG_NONE);
    }
    ::InitWindow(width, height, title);
    ::SetTargetFPS(fps);
}

Window::operator bool() const noexcept { return !::WindowShouldClose(); }

Window::~Window() noexcept { ::CloseWindow(); }

Window::Frame::Frame() noexcept { ::BeginDrawing(); }
Window::Frame::~Frame() noexcept { ::EndDrawing(); };
void Window::Frame::SetBackground(Color color) const noexcept {
    ::ClearBackground(ToRayColor(color));
}
void Window::Frame::DrawText(const char* text, int x, int y, int size,
                             Color color) const noexcept {
    ::DrawText(text, x, y, size, ToRayColor(color));
}
Window::Frame Window::MakeFrame() const noexcept { return Window::Frame{}; }
}  // namespace arigato::display
