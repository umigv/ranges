#include "ranges.hpp"

#include <functional>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

TEST(FilteredRangeTest, Basic) {
    constexpr std::array<int, 8> INPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };
    constexpr std::array<int, 4> OUTPUT{ { 0, 2, 4, 6 } };

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const std::vector<int> u = umigv::ranges::adapt(v)
        .filter([](auto &&x) { return std::forward<decltype(x)>(x) % 2 == 0; })
        .collect();

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

TEST(FilteredRangeTest, ReferenceWrapper) {
    constexpr std::array<int, 8> INPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };
    constexpr std::array<int, 4> OUTPUT{ { 0, 2, 4, 6 } };

    CountingPredicate predicate;

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const std::vector<int> u = umigv::ranges::adapt(v)
        .filter(std::ref(predicate))
        .collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size()
                && predicate.count == INPUT.size());
}

struct Integer {
    std::int64_t data;

    constexpr bool is_positive() const noexcept {
        return data > 0;
    }
};

constexpr bool operator==(const Integer &lhs, const Integer &rhs) noexcept {
    return lhs.data == rhs.data;
}

TEST(FilteredRangeTest, MemberFunction) {
    constexpr std::array<Integer, 4> INPUT{ { { -2 }, { -1 }, { 0 }, { 1 } } };
    constexpr std::array<Integer, 1> OUTPUT{ { { 1 } } };

    const std::vector<Integer> v(INPUT.cbegin(), INPUT.cend());
    const std::vector<Integer> u = umigv::ranges::adapt(v)
        .filter(&Integer::is_positive)
        .collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(FilteredRangeTest, Apply) {
    const std::vector<std::pair<int, int>> INPUT{
        { 0, 0 },
        { 2, 3 },
        { 1, 1 }
    };
    const std::vector<std::pair<int, int>> OUTPUT{ { 0, 0 }, { 1, 1 } };

    const auto is_equal = [](auto &&lhs, auto &&rhs) noexcept -> bool {
        return std::forward<decltype(lhs)>(lhs)
               == std::forward<decltype(rhs)>(rhs);
    };

    const std::vector<std::pair<int, int>> v =
        umigv::ranges::filter(INPUT, is_equal).collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}

TEST(FilteredRangeTest, ApplyOverload) {
    struct EqualityTester {
        constexpr bool operator()(const std::tuple<int, int> &tuple) noexcept {
            ++invoke_count;
            return std::get<0>(tuple) == std::get<1>(tuple);
        }

        constexpr bool operator()(int lhs, int rhs) noexcept {
            ++apply_count;
            return lhs == rhs;
        }

        int invoke_count = 0;
        int apply_count = 0;
    };

    const std::vector<std::pair<int, int>> INPUT{
        { 0, 0 },
        { 2, 3 },
        { 1, 1 }
    };
    const std::vector<std::pair<int, int>> OUTPUT{ { 0, 0 }, { 1, 1 } };

    EqualityTester tester;

    const std::vector<std::pair<int, int>> v =
        umigv::ranges::filter(INPUT, std::ref(tester)).collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size()
                && tester.invoke_count == 3
                && tester.apply_count == 0);
}
