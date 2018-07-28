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

#ifndef UMIGV_RANGES_CONST_ITERATOR_HPP
#define UMIGV_RANGES_CONST_ITERATOR_HPP

#include "traits.hpp"

#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, bool = is_input_iterator<I>::value>
class ConstIterator { };

template <typename I>
class ConstIterator<I, true> {
public:
    using difference_type = iterator_difference_t<I>;
    using value_type = iterator_value_t<I>;
    using pointer = const std::remove_pointer_t<iterator_pointer_t<I>>*;
    using reference = const iterator_reference_t<I>;
    using iterator_category = std::input_iterator_tag;

    constexpr explicit ConstIterator(const I &base)
    noexcept(std::is_nothrow_copy_constructible<I>::value) : base_{ base } { }

    constexpr reference operator*() const {
        return *base_;
    }

    constexpr pointer operator->() const {
        return base_.operator->();
    }

    constexpr ConstIterator& operator++() {
        ++base_;

        return *this;
    }

    constexpr ConstIterator operator++(int) {
        const ConstIterator to_return = *this;

        ++*this;

        return to_return;
    }

    friend constexpr bool operator==(const ConstIterator &lhs,
                                     const ConstIterator &rhs)
    noexcept(is_nothrow_equality_comparable<I>::value) {
        return lhs.base_ == rhs.base_;
    }

    friend constexpr bool operator!=(const ConstIterator &lhs,
                                     const ConstIterator &rhs)
    noexcept(is_nothrow_equality_comparable<I>::value) {
        return !(lhs == rhs);
    }

private:
    I base_;
};

} // namespace ranges
} // namespace umigv

#endif
