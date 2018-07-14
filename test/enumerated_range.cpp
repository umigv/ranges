#include "ranges.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

TEST(EnumeratedRangeTest, Basic) {
    constexpr std::array<int, 8> INPUT{ { 9, 8, 7, 6, 5, 4, 3, 2 } };
    constexpr std::array<std::pair<std::size_t, int>, 8> OUTPUT{ {
        { 0, 9 },
        { 1, 8 },
        { 2, 7 },
        { 3, 6 },
        { 4, 5 },
        { 5, 4 },
        { 6, 3 },
        { 7, 2 }
    } };

    const std::vector<std::pair<std::size_t, int>> v =
        umigv::ranges::enumerate(INPUT).collect();

    ASSERT_TRUE(std::equal(OUTPUT.cbegin(), OUTPUT.cend(), v.cbegin())
                && OUTPUT.size() == v.size());
}

TEST(EnumeratedRangeTest, IstreambufIterator) {
    std::istringstream iss{ "foo" };
    constexpr std::array<std::pair<std::size_t, char>, 3> OUTPUT{ {
        { 0, 'f' },
        { 1, 'o' },
        { 2, 'o' }
    } };

    const std::vector<std::pair<std::size_t, char>> v =
        umigv::ranges::adapt(std::istreambuf_iterator<char>{ iss },
                             std::istreambuf_iterator<char>{ }).enumerate().collect();

    ASSERT_TRUE(std::equal(OUTPUT.cbegin(), OUTPUT.cend(), v.cbegin())
                && OUTPUT.size() == v.size());
}
