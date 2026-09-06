#ifndef SRC_DISPLAY_INCLUDE_WINDOW_HPP_
#define SRC_DISPLAY_INCLUDE_WINDOW_HPP_

#include <cstdint>

#include "./Sprite.hpp"

namespace arigato::display {
class Window final {
 public:  // types
    enum class BackgroundColor : std::uint8_t {
        White,
        LightGray,
    };

    struct Keys final {
        bool left{};
        bool right{};
        bool up{};
        bool down{};
    };

    struct Frame final {
        ~Frame() noexcept;
        explicit Frame(Frame const&) = delete;
        explicit Frame(Frame&&) = delete;
        Frame& operator=(Frame const&) = delete;
        Frame& operator=(Frame&&) = delete;

        void SetBackground(BackgroundColor) const noexcept;
        void DrawText(const char* text, int x, int y, int size,
                      BackgroundColor color) const noexcept;
        void DrawSprite(Sprite const&, int x, int y) const noexcept;
        Keys GetKeys() const noexcept;

     private:
        explicit Frame() noexcept;

        friend class Window;
    };

 public:  // APIS
    explicit Window(int width, int height, int fps, const char* title) noexcept;

    explicit operator bool() const noexcept;

    Frame MakeFrame() const noexcept;

    double DeltaTime() const noexcept;

 public:  // Rule of 5
    ~Window() noexcept;
    explicit Window(Window const&) = delete;
    explicit Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;
};
}  // namespace arigato::display

#endif  // SRC_DISPLAY_INCLUDE_WINDOW_HPP_
