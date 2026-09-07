#include "include/Level.hpp"

#include <cstddef>
#include <cstdint>
#include <random>

namespace arigato::core {

namespace {
inline constexpr std::uint8_t min_cust_count{3};
inline constexpr std::uint8_t max_cust_count{32};
static_assert(min_cust_count < max_cust_count);
}  // anonymous namespace

Level::Level() noexcept
    : day_{1}, customers_left_{[]() -> std::uint8_t {
          try {
              std::mt19937 twister{std::random_device{}()};
              std::uniform_int_distribution<std::uint8_t> dist{min_cust_count,
                                                               max_cust_count};
              return dist(twister);
          } catch (...) {
              return 42;
          }
      }()} {}

Level::Level(std::uint8_t seed) noexcept
    : day_{1}, customers_left_{[](std::uint8_t seed) -> std::uint8_t {
          try {
              std::mt19937 seeded_twister{seed};
              std::uniform_int_distribution<std::uint8_t> dist{min_cust_count,
                                                               max_cust_count};
              return dist(seeded_twister);
          } catch (...) {
              return 41;
          }
      }(seed)} {}

std::size_t Level::GetDay() const noexcept { return day_; }
std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}
}  // namespace arigato::core
