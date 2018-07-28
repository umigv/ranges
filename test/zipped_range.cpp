#include "ranges.hpp"

#include <algorithm>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

struct PairMaker {
    template <typename T, typename U>
    constexpr decltype(auto) operator()(T &&x, U &&y) const {
        return std::make_pair(std::forward<T>(x), std::forward<U>(y));
    }
};

TEST(ZippedRange, Basic) {
    const std::vector<int> integers{ 0, 1, 2 };
    const std::vector<double> reals{ 3.14, 2.718 };

    const std::vector<std::pair<int, double>> zipped =
        umigv::ranges::adapt(integers)
            .zip(reals)
            .map(PairMaker{ })
            .collect();

    const std::vector<std::pair<int, double>> expected{
        { 0, 3.14 }, { 1, 2.718 }
    };

    EXPECT_TRUE(std::equal(zipped.cbegin(), zipped.cend(), expected.cbegin())
                && zipped.size() == expected.size());
}

TEST(ZippedRange, Map) {
    constexpr std::array<int, 4> INPUT_1{ { 0, 1, 2, 3 } };
    constexpr std::array<int, 3> INPUT_2{ { 14, 9, 4 } };
    constexpr std::array<int, 3> OUTPUT{ { 14, 10, 6 } };

    const std::vector<int> v = umigv::ranges::zip(INPUT_1, INPUT_2)
        .map([](int x, int y) { return x + y; })
        .collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}

TEST(ZippedRange, IstreambufIterator) {
    std::istringstream iss1{ "fbbq" };
    std::istringstream iss2{ "oaau" };
    std::istringstream iss3{ "orzx" };

    constexpr std::array<const char*, 4> OUTPUT{
        { "foo", "bar", "baz", "qux" }
    };

    const auto r1 = umigv::ranges::adapt(std::istreambuf_iterator<char>{ iss1 },
                                         std::istreambuf_iterator<char>{ });
    const auto r2 = umigv::ranges::adapt(std::istreambuf_iterator<char>{ iss2 },
                                         std::istreambuf_iterator<char>{ });
    const auto r3 = umigv::ranges::adapt(std::istreambuf_iterator<char>{ iss3 },
                                         std::istreambuf_iterator<char>{ });

    const std::vector<std::string> v = umigv::ranges::zip(r1, r2, r3)
        .map([](char first, char second, char third) -> std::string {
            return { first, second, third };
        })
        .collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}

TEST(ZippedRange, Set) {
    constexpr std::array<int, 5> INPUT_1{ { 4, 0, 3, 2, 1 } };
    constexpr std::array<int, 4> INPUT_2{ { 8, 7, 6, 5 } };
    constexpr std::array<int, 4> OUTPUT{ { 5, 7, 9, 11 } };

    const std::set<int> input_1(INPUT_1.cbegin(), INPUT_1.cend());
    const std::set<int> input_2(INPUT_2.cbegin(), INPUT_2.cend());

    const std::set<int> s = umigv::ranges::zip(input_1, input_2)
        .map([](int x, int y) { return x + y; })
        .collect();

    EXPECT_TRUE(std::equal(s.cbegin(), s.cend(), OUTPUT.cbegin())
                && s.size() == OUTPUT.size());
}
