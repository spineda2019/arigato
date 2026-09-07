// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Window.cpp - Implementation for the game windowing system. See .hpp for API
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

extern "C" {
#include <raylib.h>
}

#include <memory>
#include <utility>

#include "include/Sprite.hpp"
#include "include/Window.hpp"

namespace {
constexpr bool debug{
#ifdef ARIGATO_DEBUG
    true
#else
    false
#endif
};
}  // namespace

namespace arigato::display {
namespace detail {
struct SpriteImpl {
    Texture2D texture_;

    explicit SpriteImpl(Texture2D texture) noexcept
        : texture_{std::move(texture)} {}

    ~SpriteImpl() noexcept { UnloadTexture(texture_); }
    SpriteImpl(SpriteImpl const&) = delete;
    SpriteImpl& operator=(SpriteImpl const&) = delete;
    SpriteImpl(SpriteImpl&&) = delete;
    SpriteImpl& operator=(SpriteImpl&&) = delete;
};
}  // namespace detail

Sprite::Sprite(const char* path, Sprite::Area area) noexcept
    : impl_{std::make_unique<detail::SpriteImpl>(LoadTexture(path))},
      area_{std::move(area)} {}

Sprite::Sprite(const char* path) noexcept
    : impl_{std::make_unique<detail::SpriteImpl>(LoadTexture(path))},
      area_{.width = static_cast<float>(impl_->texture_.width),
            .height = static_cast<float>(impl_->texture_.height)} {}

Sprite::~Sprite() noexcept = default;

Sprite::ReadonlyImplRef_t Sprite::ReadonlyImplRef() const noexcept {
    return impl_;
}

float Sprite::GetHeight() const noexcept { return area_.height; }

float Sprite::GetWidth() const noexcept { return area_.width; }
}  // namespace arigato::display

namespace arigato::display {
namespace {

constexpr decltype(RAYWHITE) ToRayColor(Window::BackgroundColor color) {
    switch (color) {
        case Window::BackgroundColor::White:
            return RAYWHITE;
        case Window::BackgroundColor::LightGray:
            return LIGHTGRAY;
    }
}
}  // namespace

Window::Window(int width, int height, int fps, const char* title) noexcept {
    if constexpr (!debug) {
        ::SetTraceLogLevel(LOG_NONE);
    }
    ::SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    ::InitWindow(width, height, title);
    ::SetTargetFPS(fps);
}

Window::operator bool() const noexcept { return !::WindowShouldClose(); }

Window::~Window() noexcept { ::CloseWindow(); }

Window::Keys Window::GetKeys() const noexcept {
    return Window::Keys{
        .left = ::IsKeyDown(KEY_LEFT),
        .right = ::IsKeyDown(KEY_RIGHT),
        .up = ::IsKeyDown(KEY_UP),
        .down = ::IsKeyDown(KEY_DOWN),
        .space = ::IsKeyPressed(KEY_SPACE),
    };
};

float Window::DeltaTime() const noexcept { return GetFrameTime(); }

int Window::GetFPS() const noexcept { return ::GetFPS(); }

Window::Mouse Window::GetMouse() const noexcept {
    const auto mouse_pos{::GetMousePosition()};
    return {
        .x = mouse_pos.x,
        .y = mouse_pos.y,
        .clicked = ::IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
    };
};

int Window::GetWidth() const noexcept { return ::GetScreenWidth(); }

int Window::GetHeight() const noexcept { return ::GetScreenHeight(); }

Window::Frame::Frame() noexcept { ::BeginDrawing(); }
Window::Frame::~Frame() noexcept { ::EndDrawing(); };
void Window::Frame::SetBackground(BackgroundColor color) const noexcept {
    ::ClearBackground(ToRayColor(color));
}
void Window::Frame::SetBackgroundRGB(Window::RGB rgb) const noexcept {
    constexpr unsigned char full_opaque{255};
    ::ClearBackground(::Color{
        .r = rgb.red,
        .g = rgb.green,
        .b = rgb.blue,
        .a = full_opaque,
    });
}
void Window::Frame::DrawText(const char* text, int x, int y, int size,
                             BackgroundColor color) const noexcept {
    ::DrawText(text, x, y, size, ToRayColor(color));
}
void Window::Frame::DrawText(const char* text, int x, int y, int size,
                             Window::RGB rgb) const noexcept {
    constexpr unsigned char full_opaque{255};
    ::DrawText(text, x, y, size,
               ::Color{
                   .r = rgb.red,
                   .g = rgb.green,
                   .b = rgb.blue,
                   .a = full_opaque,
               });
}
Window::Frame Window::MakeFrame() const noexcept { return Window::Frame{}; }
void Window::Frame::DrawFullSprite(Sprite const& s, float x,
                                   float y) const noexcept {
    constexpr ::Vector2 abs_origin{.x = 0.0f, .y = 0.0f};
    ::DrawTexturePro(
        s.ReadonlyImplRef()->texture_,
        {.x = 0.0f, .y = 0.0f, .width = s.GetWidth(), .height = s.GetHeight()},
        {.x = x, .y = y, .width = s.GetWidth(), .height = s.GetHeight()},
        abs_origin,
        0.0f,  // no rotation
        WHITE);
}

void Window::Frame::DrawSpriteRegion(Sprite const& s, Sprite::Area region,
                                     float x, float y) const noexcept {
    constexpr ::Vector2 abs_origin{.x = 0.0f, .y = 0.0f};
    ::DrawTexturePro(
        s.ReadonlyImplRef()->texture_,
        {.x = region.x,
         .y = region.y,
         .width = region.width,
         .height = region.height},
        {.x = x, .y = y, .width = region.width, .height = region.height},
        abs_origin,
        0.0f,  // no rotation
        WHITE);
}

void Window::Frame::DrawRectangle(const char* text, int x, int y, int width,
                                  int height,
                                  BackgroundColor border_color) const noexcept {
    const auto color{ToRayColor(border_color)};
    ::DrawRectangle(x, y, width, height, color);
    // TODO(SEP): Use std::clamp
    ::DrawText(text, x + 5, y + (height / 2), height / 4, WHITE);
}
}  // namespace arigato::display
