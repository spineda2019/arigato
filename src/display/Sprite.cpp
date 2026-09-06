#include "include/Sprite.hpp"

#include <memory>
#include <utility>

extern "C" {
#include <raylib.h>
}

namespace arigato::display {
namespace detail {
struct Impl {
    Texture2D texture_;

    explicit Impl(Texture2D texture) noexcept : texture_{std::move(texture)} {}

    ~Impl() noexcept { UnloadTexture(texture_); }
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;
};
}  // namespace detail

Sprite::Sprite(const char* path) noexcept
    : impl_{std::make_unique<detail::Impl>(LoadTexture(path))} {}
}  // namespace arigato::display
