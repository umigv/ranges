// BSD 3-Clause License
//
// Copyright (c) 2018, UMIGV
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproducne the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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
                && predicate.count >= INPUT.size());
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
                && tester.invoke_count >= 3
                && tester.apply_count == 0);
}
