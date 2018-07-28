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
#include <sstream>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

TEST(EnumeratedRangeTest, Basic) {
    constexpr std::array<int, 8> INPUT{ { 9, 8, 7, 6, 5, 4, 3, 2 } };
    constexpr std::array<std::pair<std::size_t, int>, 8> OUTPUT{ {
        { 0, 9 },
        { 1, 8 },
        { 2, 7 },
        { 3, 6 },
        { 4, 5 },
        { 5, 4 },
        { 6, 3 },
        { 7, 2 }
    } };

    const std::vector<std::pair<std::size_t, int>> v =
        umigv::ranges::enumerate(INPUT).collect();

    ASSERT_TRUE(std::equal(OUTPUT.cbegin(), OUTPUT.cend(), v.cbegin())
                && OUTPUT.size() == v.size());
}

TEST(EnumeratedRangeTest, IstreambufIterator) {
    std::istringstream iss{ "foo" };
    constexpr std::array<std::pair<std::size_t, char>, 3> OUTPUT{ {
        { 0, 'f' },
        { 1, 'o' },
        { 2, 'o' }
    } };

    const std::vector<std::pair<std::size_t, char>> v =
        umigv::ranges::adapt(std::istreambuf_iterator<char>{ iss },
                             std::istreambuf_iterator<char>{ }).enumerate().collect();

    ASSERT_TRUE(std::equal(OUTPUT.cbegin(), OUTPUT.cend(), v.cbegin())
                && OUTPUT.size() == v.size());
}
