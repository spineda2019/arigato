#ifndef SRC_CORE_INCLUDE_LEVEL_HPP_
#define SRC_CORE_INCLUDE_LEVEL_HPP_

#include <cstddef>
#include <cstdint>

namespace arigato::core {
class Level final {
 public:  // types
    struct Action final {
        std::uint8_t amount_served{};
    };

 public:
    explicit Level() noexcept;
    explicit Level(std::uint8_t seed) noexcept;
    std::size_t GetDay() const noexcept;
    std::uint8_t GetCustomersLeft() const noexcept;
    void Update(Action) noexcept;

 private:
    std::size_t day_{1};
    std::uint8_t customers_left_{};
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_LEVEL_HPP_
