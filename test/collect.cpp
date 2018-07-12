#include "ranges.hpp"

#include <algorithm>
#include <array>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

TEST(CollectTest, Basic) {
    const std::array<int, 8> OUTPUT{ 0, 1, 2, 3, 4, 5, 6, 7 };

    const std::unordered_set<int> u = umigv::ranges::range(8).collect();
    std::vector<int> sorted{ u.cbegin(), u.cend() };
    std::sort(sorted.begin(), sorted.end());

    EXPECT_TRUE(std::equal(sorted.cbegin(), sorted.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}
