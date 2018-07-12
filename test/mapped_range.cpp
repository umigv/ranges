#include "ranges.hpp"

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
    const std::vector<int> u = umigv::ranges::iterator_range(v)
        .map([](auto &&x) { return std::forward<decltype(x)>(x) * 2; })
        .collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(MappedRangeTest, MemberFunction) {
    static const std::array<std::string, 3> INPUT{ { "foo", "barbaz", "qux" } };
    static const std::array<std::string::size_type, 3> OUTPUT{ { 3, 6, 3 } };

    const std::vector<std::string> v(INPUT.cbegin(), INPUT.cend());
    const std::vector<std::string::size_type> u =
        umigv::ranges::iterator_range(v).map(&std::string::size).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}
