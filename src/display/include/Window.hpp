#ifndef SRC_WINDOW_HPP_
#define SRC_WINDOW_HPP_

namespace arigato {
class Window final {
 public:
    explicit Window(int width, int height, int fps, const char* title) noexcept;

    bool IsRunning() const noexcept;

    void Draw() const noexcept;

 public:  // Rule of 5
    ~Window() noexcept;
    explicit Window(Window const&) = delete;
    explicit Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;
};
}  // namespace arigato

#endif  // SRC_WINDOW_HPP_
