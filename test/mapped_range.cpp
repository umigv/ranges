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
    const std::vector<int> u = umigv::ranges::adapt(v)
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
        umigv::ranges::adapt(v).map(&std::string::size).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(MappedRangeTest, Apply) {
    static constexpr std::array<std::pair<int, int>, 3> INPUT{ {
        { 0, 1 },
        { 2, 3 },
        { 4, 5 }
    } };
    static constexpr std::array<int, 3> OUTPUT{ { 1, 5, 9 } };

    const std::vector<int> v =
        umigv::ranges::map(INPUT, [](int x, int y) { return x + y; }).collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}

TEST(MappedRangeTest, ApplyOverload) {
    struct Adder {
        constexpr int operator()(int x, int y) noexcept {
            ++apply_count;

            return x + y;
        }

        constexpr int operator()(const std::pair<int, int> &pair) noexcept {
            ++invoke_count;

            return pair.first + pair.second;
        }

        int apply_count = 0;
        int invoke_count = 0;
    };

    static constexpr std::array<std::pair<int, int>, 3> INPUT{ {
        { 0, 1 },
        { 2, 3 },
        { 4, 5 }
    } };
    static constexpr std::array<int, 3> OUTPUT{ { 1, 5, 9 } };

    Adder adder;

    const std::vector<int> v =
        umigv::ranges::map(INPUT, std::ref(adder)).collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size()
                && adder.apply_count == 0 && adder.invoke_count == 3);
}
