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

#include <limits>
#include <vector>

#include <gtest/gtest.h>

TEST(CountingRangeTest, Basic) {
    constexpr std::array<int, 8> OUTPUT{ { 0, 1, 2, 3, 4, 5, 6, 7 } };

    const std::vector<int> u = umigv::ranges::range(8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(CountingRangeTest, Negative) {
    constexpr std::array<int, 8> OUTPUT{ { 0, -1, -2, -3, -4, -5, -6, -7 } };

    const std::vector<int> u = umigv::ranges::range(0, -1, -8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(CountingRangeTest, Stride) {
    constexpr std::array<int, 4> OUTPUT{ { 0, 2, 4, 6 } };

    const std::vector<int> u = umigv::ranges::range(0, 2, 8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

TEST(CountingRangeTest, Offset) {
    constexpr std::array<int, 4> OUTPUT{ { 4, 5, 6, 7 } };

    const std::vector<int> u = umigv::ranges::range(4, 8).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin())
                && u.size() == OUTPUT.size());
}

struct EpsilonEquals {
    template <typename T>
    constexpr bool operator()(const T &lhs, const T &rhs) const noexcept {
        constexpr T EPSILON = std::numeric_limits<T>::epsilon();

        return lhs >= rhs - EPSILON && lhs <= rhs + EPSILON;
    }
};

TEST(CountingRangeTest, Double) {
    constexpr std::array<double, 4> OUTPUT{ { 0.0, 0.5, 1.0, 1.5 } };

    const std::vector<double> u = umigv::ranges::range(0.0, 0.5, 2.0).collect();

    EXPECT_TRUE(std::equal(u.cbegin(), u.cend(), OUTPUT.cbegin(),
                           EpsilonEquals{ })
                && u.size() == OUTPUT.size());
}
