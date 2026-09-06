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

Sprite::Sprite(const char* path) noexcept
    : impl_{std::make_unique<detail::SpriteImpl>(LoadTexture(path))} {}

Sprite::~Sprite() noexcept = default;

Sprite::ReadonlyImplRef_t Sprite::ReadonlyImplRef() const noexcept {
    return impl_;
}
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
    ::InitWindow(width, height, title);
    ::SetTargetFPS(fps);
}

Window::operator bool() const noexcept { return !::WindowShouldClose(); }

Window::~Window() noexcept { ::CloseWindow(); }

Window::Keys Window::Frame::GetKeys() const noexcept {
    return Window::Keys{
        .left = ::IsKeyDown(KEY_LEFT),
        .right = ::IsKeyDown(KEY_RIGHT),
        .up = ::IsKeyDown(KEY_UP),
        .down = ::IsKeyDown(KEY_DOWN),
    };
};

double Window::DeltaTime() const noexcept { return GetFrameTime(); }

Window::Frame::Frame() noexcept { ::BeginDrawing(); }
Window::Frame::~Frame() noexcept { ::EndDrawing(); };
void Window::Frame::SetBackground(BackgroundColor color) const noexcept {
    ::ClearBackground(ToRayColor(color));
}
void Window::Frame::DrawText(const char* text, int x, int y, int size,
                             BackgroundColor color) const noexcept {
    ::DrawText(text, x, y, size, ToRayColor(color));
}
Window::Frame Window::MakeFrame() const noexcept { return Window::Frame{}; }
void Window::Frame::DrawSprite(Sprite const& s, int x, int y) const noexcept {
    ::DrawTexture(s.ReadonlyImplRef()->texture_, x, y, WHITE);
}
}  // namespace arigato::display
