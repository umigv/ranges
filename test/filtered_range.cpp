#include "ranges.hpp"

#include <functional>
#include <vector>

#include <gtest/gtest.h>

TEST(FilteredRangeTest, Vector) {
    constexpr std::array<int, 8> INPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };
    constexpr std::array<int, 4> OUTPUT{ { 0, 2, 4, 6 } };

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const auto range = umigv::ranges::iterator_range(v)
        .filter([](auto &&x) { return std::forward<decltype(x)>(x) % 2 == 0; });
    const std::vector<int> u(range.begin(), range.end());

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

struct CountingPredicate {
    std::size_t count = 0;

    template <typename T>
    constexpr decltype(auto) operator()(T &&t)
    noexcept(noexcept(std::declval<T>() % 2 == 0)) {
        ++count;

        return std::forward<T>(t) % 2 == 0;
    }
};

TEST(FilteredRangeTest, State) {
    constexpr std::array<int, 8> INPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };
    constexpr std::array<int, 4> OUTPUT{ { 0, 2, 4, 6 } };

    CountingPredicate predicate;

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const auto range = umigv::ranges::iterator_range(v)
        .filter(std::ref(predicate));
    const std::vector<int> u(range.begin(), range.end());

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size()
                && predicate.count == INPUT.size());
}
