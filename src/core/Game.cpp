#include "include/Game.hpp"

#include "include/Level.hpp"
#include "include/Vec.hpp"

namespace arigato::core {
Game::Game() noexcept : level_{}, character_{} {}

Vec2D Game::GetPlayerPosition() const noexcept {
    return character_.GetPosition();
}
std::size_t Game::GetCurrentDay() const noexcept { return level_.GetDay(); }
std::uint8_t Game::GetCustomersLeft() const noexcept {
    return level_.GetCustomersLeft();
}

void Game::Update(Character::Action action, float dt) noexcept {
    character_.Apply(action, dt);
}

void Game::NextLevel() noexcept { level_ = Level{}; }

bool Game::Save() const noexcept { return false; }
}  // namespace arigato::core
