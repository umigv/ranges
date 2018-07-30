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

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

struct A {
    static std::size_t const_copy_count;
    static std::size_t mut_copy_count;

    constexpr A() noexcept = default;

    constexpr bool is_const() noexcept {
        return false;
    }

    constexpr bool is_const() const noexcept {
        return true;
    }
};

TEST(ConstIteratorTest, ConstAccessors) {
    std::array<A, 4> INPUT;

    const auto adapted = umigv::ranges::adapt(INPUT);

    for (auto &&elem : umigv::ranges::adapt(adapted.cbegin(), adapted.cend())) {
        EXPECT_TRUE(std::forward<decltype(elem)>(elem).is_const());
    }
}

TEST(ConstIteratorTest, AsConst) {
    std::array<A, 4> INPUT;

    const auto adapted = umigv::ranges::adapt(INPUT).as_const();

    for (auto &&elem : adapted) {
        EXPECT_TRUE(std::forward<decltype(elem)>(elem).is_const());
    }
}
