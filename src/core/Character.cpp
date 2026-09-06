#include "include/Character.hpp"

#include <utility>

#include "include/Types.hpp"

namespace arigato::core {
namespace {
constexpr float move_speed{120.0};
constexpr float Direction(Character::Action::Direction dir) {
    return static_cast<float>(std::to_underlying(dir));
}
}  // namespace
void Character::Apply(Character::Action action, Seconds dt) noexcept {
    pos_.x += Direction(action.move_x) * move_speed * dt;
    pos_.y += Direction(action.move_y) * move_speed * dt;
};

Vec2D Character::GetPosition() const noexcept { return pos_; }
}  // namespace arigato::core
