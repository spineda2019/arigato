// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Window.hpp - Independent representation of the game windowing system
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_DISPLAY_INCLUDE_WINDOW_HPP_
#define SRC_DISPLAY_INCLUDE_WINDOW_HPP_

#include <cstdint>
#include <type_traits>

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

        bool space{};
    };

    struct RGB final {
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };

    static_assert(sizeof(RGB) == 3, "RGB should be 3 8-bit numbers");

    struct Frame final {
        ~Frame() noexcept;
        explicit Frame(Frame const&) = delete;
        explicit Frame(Frame&&) = delete;
        Frame& operator=(Frame const&) = delete;
        Frame& operator=(Frame&&) = delete;

        /// Set the entire screen background to a preset color.
        void SetBackground(BackgroundColor) const noexcept;
        /// Set the entire screen to a precise classic RGB value
        void SetBackgroundRGB(RGB) const noexcept;
        void DrawText(const char* text, int x, int y, int size,
                      BackgroundColor color) const noexcept;
        void DrawText(const char* text, int x, int y, int size,
                      RGB) const noexcept;
        /// Draws the entire sprite to the destination x and y coordinate
        void DrawFullSprite(Sprite const&, float x, float y) const noexcept;
        /// Like `DrawFullSprite` but can select a specific region of the source
        /// sprite to draw. Usefull for drawing a single sprite from a
        /// spritesheet, as the whole spritesheet can be stored in GPU memory
        /// once for many sprites in the sheet
        void DrawSpriteRegion(Sprite const&, Sprite::Area region, float x,
                              float y) const noexcept;
        /// Draws a sprite with a specified `src` sub-region (e.g. a single
        /// sprite region from a whole spritesheet) to a specified `dest` (e.g.
        /// for scaling with the window)
        void DrawSpriteRegion(Sprite const&, Sprite::Area src,
                              Sprite::Area dest) const noexcept;
        void DrawRectangle(const char* text, int x, int y, int width,
                           int height,
                           BackgroundColor border_color) const noexcept;
        void DrawRectangle(const char* text, Sprite::IntegralArea src,
                           BackgroundColor border_color) const noexcept;
        void DrawRectangle(const char* text, Sprite::IntegralArea src,
                           RGB border_color) const noexcept;

     private:
        explicit Frame() noexcept;

        friend class Window;
    };

    static_assert(!std::is_copy_assignable_v<Frame>);
    static_assert(!std::is_copy_constructible_v<Frame>);
    static_assert(!std::is_move_assignable_v<Frame>);
    static_assert(!std::is_move_constructible_v<Frame>);

    struct Mouse final {
        float x{};
        float y{};
        bool clicked{};
    };

 public:  // APIS
    explicit Window(int width, int height, int fps, const char* title) noexcept;

    explicit operator bool() const noexcept;

    Frame MakeFrame() const noexcept;

    float DeltaTime() const noexcept;

    int GetFPS() const noexcept;

    Keys GetKeys() const noexcept;

    Mouse GetMouse() const noexcept;

    int GetWidth() const noexcept;

    int GetHeight() const noexcept;

 public:  // Rule of 5
    ~Window() noexcept;
    explicit Window(Window const&) = delete;
    explicit Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;
};
}  // namespace arigato::display

#endif  // SRC_DISPLAY_INCLUDE_WINDOW_HPP_
