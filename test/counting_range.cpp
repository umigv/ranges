#include "ranges.hpp"

#include <limits>
#include <vector>

#include <gtest/gtest.h>

TEST(CountingRangeTest, Basic) {
    constexpr std::array<int, 8> OUTPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };

    const std::vector<int> u = umigv::ranges::range(8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(CountingRangeTest, Negative) {
    constexpr std::array<int, 8> OUTPUT{ { 0, -1, -2, -3, -4, -5, -6, -7 } };

    const std::vector<int> u = umigv::ranges::range(0, -1, -8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(CountingRangeTest, Stride) {
    constexpr std::array<int, 4> OUTPUT{ { 0, 2, 4, 6 } };

    const std::vector<int> u = umigv::ranges::range(0, 2, 8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(CountingRangeTest, Offset) {
    constexpr std::array<int, 4> OUTPUT{ { 4, 5, 6, 7 } };

    const std::vector<int> u = umigv::ranges::range(4, 8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

struct EpsilonEquals {
    template <typename T>
    constexpr bool operator()(const T &lhs, const T &rhs) const noexcept {
        constexpr T EPSILON = std::numeric_limits<T>::epsilon();

        return lhs >= rhs - EPSILON && lhs <= rhs + EPSILON;
    }
};

TEST(CountingRangeTest, Double) {
    constexpr std::array<double, 4> OUTPUT{ { 0.0, 0.5, 1.0, 1.5 } };

    const std::vector<double> u = umigv::ranges::range(0.0, 0.5, 2.0).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin(),
                           EpsilonEquals{ })
                && u.size() == OUTPUT.size());
}
