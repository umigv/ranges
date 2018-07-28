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
