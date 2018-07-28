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
