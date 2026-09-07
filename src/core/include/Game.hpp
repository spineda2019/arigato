#ifndef SRC_CORE_INCLUDE_GAME_HPP_
#define SRC_CORE_INCLUDE_GAME_HPP_

#include <cstddef>
#include <cstdint>

#include "./Character.hpp"
#include "./Level.hpp"
#include "./Vec.hpp"

namespace arigato::core {
class Game final {
 public:
    explicit Game() noexcept;

    Vec2D GetPlayerPosition() const noexcept;
    std::size_t GetCurrentDay() const noexcept;
    std::uint8_t GetCustomersLeft() const noexcept;

    void Update(Character::Action, float dt) noexcept;

    void NextLevel() noexcept;

    [[nodiscard("Save operations may fail and must be reported")]]
    bool Save() const noexcept;

 private:
    Level level_{};
    Character character_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_GAME_HPP_
