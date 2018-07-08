#include "iterator_range.hpp"
#include "mapped_range.hpp"
#include "range.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <set>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

TEST(MappedRangeTest, Vector) {
    constexpr std::array<int, 8> INPUT{ { 3, 1, 2, 4, 5, 0, 6, 7 } };
    constexpr std::array<int, 8> OUTPUT{ { 6, 2, 4, 8, 10, 0, 12, 14 } };

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const auto range = umigv::ranges::iterator_range(v)
        .map([](auto &&x) { return std::forward<decltype(x)>(x) * 2; });
    const std::vector<int> u(range.begin(), range.end());

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}
