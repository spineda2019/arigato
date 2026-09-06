#ifndef SRC_DISPLAY_INCLUDE_TEXTURE_HPP_
#define SRC_DISPLAY_INCLUDE_TEXTURE_HPP_

#include <memory>
#include <type_traits>

namespace arigato::display {
namespace detail {
struct Impl;
}  // namespace detail

class Sprite final {
 public:
    explicit Sprite(const char* path) noexcept;

 private:
    std::unique_ptr<detail::Impl> impl_{};
};

static_assert(!std::is_copy_assignable_v<Sprite>,
              "Sprites require unique ownership");
static_assert(!std::is_copy_constructible_v<Sprite>,
              "Sprites require unique ownership");

static_assert(std::is_move_constructible_v<Sprite>);
static_assert(std::is_move_assignable_v<Sprite>);
}  // namespace arigato::display

#endif  // SRC_DISPLAY_INCLUDE_TEXTURE_HPP_
