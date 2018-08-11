#include "invoke.hpp"

#include <gtest/gtest.h>

struct IntPair {
    int first;
    int second;

    constexpr const int& get_first() const noexcept {
        return first;
    }

    constexpr const int& get_second() const noexcept {
        return second;
    }
};

constexpr const int& get_first(const IntPair &pair) noexcept {
    return pair.first;
}

constexpr const int& get_second(const IntPair &pair) noexcept {
    return pair.second;
}

constexpr auto first_ptr = &IntPair::first;
constexpr auto second_ptr = &IntPair::second;

constexpr IntPair pair{ 0, 1 };

static_assert(umigv::ranges::invoke(first_ptr, pair) == 0, "");
static_assert(umigv::ranges::invoke(second_ptr, pair) == 1, "");

static_assert(umigv::ranges::invoke(first_ptr, &pair) == 0, "");
static_assert(umigv::ranges::invoke(second_ptr, &pair) == 1, "");

static_assert(umigv::ranges::invoke(get_first, pair) == 0, "");
static_assert(umigv::ranges::invoke(get_second, pair) == 1, "");

static_assert(umigv::ranges::invoke(&get_first, pair) == 0, "");
static_assert(umigv::ranges::invoke(&get_second, pair) == 1, "");

static_assert(umigv::ranges::invoke(&IntPair::get_first, pair) == 0, "");
static_assert(umigv::ranges::invoke(&IntPair::get_second, pair) == 1, "");

static_assert(umigv::ranges::invoke(&IntPair::get_first, &pair) == 0, "");
static_assert(umigv::ranges::invoke(&IntPair::get_second, &pair) == 1, "");

constexpr int foo() noexcept {
    return 5;
}

constexpr int bar(int x) noexcept {
    return x * 5;
}

constexpr int baz(int x, int y) noexcept {
    return x * y * 5;
}

TEST(InvokeTest, FunctionPointerArgumentCount) {
    constexpr auto f = foo;
    constexpr auto g = bar;
    constexpr auto h = baz;

    constexpr auto F_INVOCABLE = umigv::ranges::IS_INVOCABLE<decltype((f))>;
    constexpr auto F_INVOCABLE_INT =
        umigv::ranges::IS_INVOCABLE<decltype((f)), int>;
    constexpr auto F_INVOCABLE_INT_INT =
        umigv::ranges::IS_INVOCABLE<decltype((f)), int, int>;

    constexpr auto G_INVOCABLE = umigv::ranges::IS_INVOCABLE<decltype((g))>;
    constexpr auto G_INVOCABLE_INT =
        umigv::ranges::IS_INVOCABLE<decltype((g)), int>;
    constexpr auto G_INVOCABLE_INT_INT =
        umigv::ranges::IS_INVOCABLE<decltype((g)), int, int>;

    constexpr auto H_INVOCABLE = umigv::ranges::IS_INVOCABLE<decltype((h))>;
    constexpr auto H_INVOCABLE_INT =
        umigv::ranges::IS_INVOCABLE<decltype((h)), int>;
    constexpr auto H_INVOCABLE_INT_INT =
        umigv::ranges::IS_INVOCABLE<decltype((h)), int, int>;

    ASSERT_TRUE(F_INVOCABLE);
    ASSERT_FALSE(F_INVOCABLE_INT);
    ASSERT_FALSE(F_INVOCABLE_INT_INT);

    ASSERT_FALSE(G_INVOCABLE);
    ASSERT_TRUE(G_INVOCABLE_INT);
    ASSERT_FALSE(G_INVOCABLE_INT_INT);

    ASSERT_FALSE(H_INVOCABLE);
    ASSERT_FALSE(H_INVOCABLE_INT);
    ASSERT_TRUE(H_INVOCABLE_INT_INT);
}