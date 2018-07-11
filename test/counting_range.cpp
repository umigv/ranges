#include "ranges.hpp"

#include <vector>

#include <gtest/gtest.h>

TEST(CountingRangeTest, Basic) {
    constexpr std::array<int, 8> OUTPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };

    const auto range = umigv::ranges::range(8);
    const std::vector<int> u(range.begin(), range.end());

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}
