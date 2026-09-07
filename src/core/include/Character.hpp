#ifndef SRC_CORE_INCLUDE_CHARACTER_HPP_
#define SRC_CORE_INCLUDE_CHARACTER_HPP_

#include <cstdint>

#include "Vec.hpp"

namespace arigato::core {
class Character final {
 public:  // Types
    struct Action final {
        enum class Direction : std::int8_t {
            Positive = 1,
            Zero = 0,
            Negative = -1,
        };
        Direction move_x;
        Direction move_y;
        float dt;
    };

 public:
    void Apply(Action) noexcept;
    Vec2D GetPosition() const noexcept;

 private:
    Vec2D pos_;
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_CHARACTER_HPP_
