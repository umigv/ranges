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
