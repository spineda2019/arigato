#ifndef SRC_WINDOW_HPP_
#define SRC_WINDOW_HPP_

extern "C" {
#include <raylib.h>
}

namespace arigato {
template <int width, int height, int fps>
class Window final {
 public:
    explicit Window(const char* title) noexcept {
        InitWindow(width, height, title);
        SetTargetFPS(fps);
    }

    template <class Self>
    bool IsRunning(this Self&&) noexcept {
        return !WindowShouldClose();
    }

 public:  // Rule of 5
    ~Window() noexcept { CloseWindow(); }
    explicit Window(Window const&) = delete;
    explicit Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;
};
}  // namespace arigato

#endif  // SRC_WINDOW_HPP_
