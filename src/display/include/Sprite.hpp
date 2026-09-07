#ifndef SRC_DISPLAY_INCLUDE_SPRITE_HPP_
#define SRC_DISPLAY_INCLUDE_SPRITE_HPP_

#include <memory>
#include <type_traits>

namespace arigato::display {
namespace detail {
struct SpriteImpl;
}  // namespace detail

/// A 2D Texture. May represent a single sprite or an entire spritesheet
class Sprite final {
 public:  // types
    struct Area final {
        /// Left edge of sheet
        float x{};
        /// Top edge of sheet
        float y{};
        float width{};
        float height{};
    };

 public:
    explicit Sprite(const char* path) noexcept;
    explicit Sprite(const char* path, Area sprite_area) noexcept;
    using ReadonlyImplRef_t = std::unique_ptr<detail::SpriteImpl> const&;
    ReadonlyImplRef_t ReadonlyImplRef() const noexcept;
    float GetWidth() const noexcept;
    float GetHeight() const noexcept;

 public:                 // rule of 5
    ~Sprite() noexcept;  // in cpp file for pimpl reasons so we can use the
                         // unique_ptr
    Sprite(Sprite const&) = delete;
    Sprite& operator=(Sprite const&) = delete;
    Sprite(Sprite&&) = default;
    Sprite& operator=(Sprite&&) = default;

 private:
    std::unique_ptr<detail::SpriteImpl> impl_{};
    Area area_{};
};

static_assert(!std::is_copy_assignable_v<Sprite>,
              "Sprites require unique ownership");
static_assert(!std::is_copy_constructible_v<Sprite>,
              "Sprites require unique ownership");

static_assert(std::is_move_constructible_v<Sprite>);
static_assert(std::is_move_assignable_v<Sprite>);
}  // namespace arigato::display

#endif  // SRC_DISPLAY_INCLUDE_SPRITE_HPP_
