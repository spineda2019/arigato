#include "include/Level.hpp"

#include <cstddef>
#include <cstdint>

namespace arigato::core {
std::size_t Level::GetDay() const noexcept { return day_; }
std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}
}  // namespace arigato::core
