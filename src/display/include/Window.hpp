#ifndef SRC_WINDOW_HPP_
#define SRC_WINDOW_HPP_

#include <cstdint>
namespace arigato::display {
class Window final {
 public:  // types
    enum class Color : std::uint8_t {
        White,
        LightGray,
    };

    struct Frame final {
        ~Frame() noexcept;
        explicit Frame(Frame const&) = delete;
        explicit Frame(Frame&&) = delete;
        Frame& operator=(Frame const&) = delete;
        Frame& operator=(Frame&&) = delete;

        void SetBackground(Color) const noexcept;
        void DrawText(const char* text, int x, int y, int size,
                      Color color) const noexcept;

     private:
        explicit Frame() noexcept;

        friend class Window;
    };

 public:  // APIS
    explicit Window(int width, int height, int fps, const char* title) noexcept;

    explicit operator bool() const noexcept;

    Frame MakeFrame() const noexcept;

 public:  // Rule of 5
    ~Window() noexcept;
    explicit Window(Window const&) = delete;
    explicit Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;
};
}  // namespace arigato::display

#endif  // SRC_WINDOW_HPP_
