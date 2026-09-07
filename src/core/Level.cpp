#include "include/Level.hpp"

#include <cstddef>
#include <cstdint>
#include <random>

namespace arigato::core {

namespace {
inline constexpr std::uint8_t min_cust_count{3};
inline constexpr std::uint8_t max_cust_count{32};
static_assert(min_cust_count < max_cust_count);
std::uint8_t UnseededRandomCustAmount() noexcept {
    try {
        std::mt19937 twister{std::random_device{}()};
        std::uniform_int_distribution<std::uint8_t> dist{min_cust_count,
                                                         max_cust_count};
        return dist(twister);
    } catch (...) {
        return 42;
    }
}
std::uint8_t SeededRandomCustAmount(std::uint8_t seed) noexcept {
    try {
        std::mt19937 seeded_twister{seed};
        std::uniform_int_distribution<std::uint8_t> dist{min_cust_count,
                                                         max_cust_count};
        return dist(seeded_twister);
    } catch (...) {
        return 42;
    }
}
}  // anonymous namespace

Level::Level() noexcept
    : day_{1}, customers_left_{UnseededRandomCustAmount()} {}

Level::Level(std::uint8_t seed) noexcept
    : day_{1}, customers_left_{SeededRandomCustAmount(seed)} {}

std::size_t Level::GetDay() const noexcept { return day_; }
std::uint8_t Level::GetCustomersLeft() const noexcept {
    return customers_left_;
}
void Level::Apply(Level::Action action) noexcept {
    customers_left_ = (customers_left_ >= action.amount_served)
                          ? customers_left_ - action.amount_served
                          : 0;
}

void Level::NextDay() noexcept {
    ++day_;
    customers_left_ = UnseededRandomCustAmount();
}

void Level::SeededNextDay(std::uint8_t seed) noexcept {
    ++day_;
    customers_left_ = SeededRandomCustAmount(seed);
}
}  // namespace arigato::core
