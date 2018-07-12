#include "ranges.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <set>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

TEST(RangeAdapterTest, Vector) {
    constexpr std::array<int, 8> INPUT{ { 3, 1, 2, 4, 5, 0, 6, 7 } };
    constexpr std::array<int, 8> OUTPUT = INPUT;

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const auto range = umigv::ranges::adapt(v);
    const std::vector<int> u(range.begin(), range.end());

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(RangeAdapterTest, Iterators) {
    constexpr std::array<int, 4> INPUT{ { 0, 1, 2, 3} };
    constexpr std::array<int, 2> OUTPUT{ { 1, 2 } };

    const auto range = umigv::ranges::adapt(INPUT.cbegin() + 1,
                                            INPUT.cend() - 1);
    const std::vector<int> v(range.begin(), range.end());

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}

TEST(RangeAdapterTest, InitializerList) {
    constexpr std::array<int, 3> OUTPUT{ { 4, 2, 0 } };

    const auto range = umigv::ranges::adapt({ 4, 2, 0 });
    const std::vector<int> v(range.begin(), range.end());

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}
