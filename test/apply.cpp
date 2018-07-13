#include "apply.hpp"
#include "traits.hpp"

#include <tuple>

#include <gtest/gtest.h>

constexpr int sum(int x, int y) noexcept {
    return x + y;
}

constexpr void sum_in_place(int &x, int y) noexcept {
    x += y;
}

TEST(ApplyTest, Basic) {
    const auto tuple = std::make_tuple(1, 2);
    const auto result = umigv::ranges::apply(&sum, tuple);

    EXPECT_EQ(result, 3);
}

TEST(ApplyTest, Pair) {
    const auto pair = std::make_pair(3, 4);
    const auto result = umigv::ranges::apply(&sum, pair);

    EXPECT_EQ(result, 7);
}

TEST(ApplyTest, Reference) {
    auto pair = std::make_pair(0, 8);
    umigv::ranges::apply(&sum_in_place, pair);

    EXPECT_EQ(pair.first, 8);
}
