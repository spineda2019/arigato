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

void Game::Update(Character::Action character_action,
                  Level::Action level_action) noexcept {
    character_.Apply(character_action);
    level_.Apply(level_action);
}

void Game::NextLevel() noexcept { level_.NextDay(); }

bool Game::Save() const noexcept { return false; }
}  // namespace arigato::core
