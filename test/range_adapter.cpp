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

TEST(RangeAdapterTest, Vector) {
    constexpr std::array<int, 8> INPUT{ { 3, 1, 2, 4, 5, 0, 6, 7 } };
    constexpr std::array<int, 8> OUTPUT = INPUT;

    const std::vector<int> v(INPUT.cbegin(), INPUT.cend());
    const auto range = umigv::ranges::adapt(v);
    const std::vector<int> u(range.begin(), range.end());

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(RangeAdapterTest, Iterators) {
    constexpr std::array<int, 4> INPUT{ { 0, 1, 2, 3} };
    constexpr std::array<int, 2> OUTPUT{ { 1, 2 } };

    const std::vector<int> v =
        umigv::ranges::adapt(INPUT.cbegin() + 1, INPUT.cend() - 1).collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}

TEST(RangeAdapterTest, InitializerList) {
    constexpr std::array<int, 3> OUTPUT{ { 4, 2, 0 } };

    const std::vector<int> v = umigv::ranges::adapt({ 4, 2, 0 }).collect();

    EXPECT_TRUE(std::equal(v.cbegin(), v.cend(), OUTPUT.cbegin())
                && v.size() == OUTPUT.size());
}
