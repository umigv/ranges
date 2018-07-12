#include "ranges.hpp"

#include <cstddef>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

struct A {
    static std::size_t const_copy_count;
    static std::size_t mut_copy_count;

    constexpr A() noexcept = default;

    A(A&) noexcept { ++mut_copy_count; };

    A(const A&) noexcept { ++const_copy_count; };

    A& operator=(A&) noexcept { ++mut_copy_count; return *this; }

    A& operator=(const A&) noexcept { ++mut_copy_count; return *this; }
};

std::size_t A::const_copy_count = 0;
std::size_t A::mut_copy_count = 0;

TEST(ConstIteratorTest, ConstAccessors) {
    std::array<A, 4> INPUT;

    const auto adapted = umigv::ranges::adapt(INPUT);

    std::vector<A> v;
    v.reserve(4);
    v.assign(adapted.cbegin(), adapted.cend());

    std::cerr << A::mut_copy_count << ' ' << A::const_copy_count << '\n';

    EXPECT_EQ(A::mut_copy_count, 0);
    EXPECT_EQ(A::const_copy_count, 4);

    A::mut_copy_count = 0;
    A::const_copy_count = 0;
}

TEST(ConstIteratorTest, AsConst) {
    std::array<A, 4> INPUT;

    const auto adapted = umigv::ranges::adapt(INPUT).as_const();

    std::vector<A> v;
    v.reserve(4);
    v.assign(adapted.begin(), adapted.end());

    std::cerr << A::mut_copy_count << ' ' << A::const_copy_count << '\n';

    EXPECT_EQ(A::mut_copy_count, 0);
    EXPECT_EQ(A::const_copy_count, 4);

    A::mut_copy_count = 0;
    A::const_copy_count = 0;
}
