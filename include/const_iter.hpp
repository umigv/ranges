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

#ifndef UMIGV_RANGES_CONST_ITER_HPP
#define UMIGV_RANGES_CONST_ITER_HPP

#include "iterator.hpp"
#include "traits.hpp"

#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I>
class RandConstIter : public RandomAccessIterator<RandConstIter<I>> {
public:
    static_assert(is_random_access_iterator<I>::value,
                  "I must be a random access iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<std::add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = std::add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr RandConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr RandConstIter& next() {
        ++current_;

        return *this;
    }

    constexpr RandConstIter& prev() {
        --current_;

        return *this;
    }

    constexpr RandConstIter& step(difference_type n) {
        current_ += n;

        return *this;
    }

    constexpr difference_type distance(const RandConstIter &other) const {
        return other.current_ - current_;
    }

    constexpr reference deref() const {
        return reference{ *current_ };
    }

    constexpr bool is_equal(const RandConstIter &other) const {
        return current_ == other.current_;
    }

    constexpr bool is_less(const RandConstIter &other) const {
        return current_ < other.current_;
    }

private:
    I current_;
};

template <typename I>
class BiConstIter : public BidirectionalIterator<BiConstIter<I>> {
public:
    static_assert(is_bidirectional_iterator<I>::value,
                  "I must be a bidirectional iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = std::add_pointer_t<std::add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = std::add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr BiConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr BiConstIter& next() {
        ++current_;

        return *this;
    }

    constexpr BiConstIter& prev() {
        --current_;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ *current_ };
    }

    constexpr bool is_equal(const BiConstIter &other) const {
        return current_ == other.current_;
    }

private:
    I current_;
};

template <typename I>
class FwdConstIter : public ForwardIterator<FwdConstIter<I>> {
public:
    static_assert(is_forward_iterator<I>::value,
                  "I must be a forward iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = std::add_pointer_t<std::add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = std::add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr FwdConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr FwdConstIter& next() {
        ++current_;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ *current_ };
    }

    constexpr bool is_equal(const FwdConstIter &other) const {
        return current_ == other.current_;
    }

private:
    I current_;
};

template <typename I>
class InConstIter : public InputIterator<InConstIter<I>> {
public:
    static_assert(is_input_iterator<I>::value,
                  "I must be an input iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = std::add_pointer_t<std::add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = std::add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr InConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr InConstIter& next() {
        ++current_;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ *current_ };
    }

    constexpr bool is_equal(const InConstIter &other) const {
        return current_ == other.current_;
    }

private:
    I current_;
};

template <typename I>
using ConstIter = std::conditional_t<
    is_random_access_iterator<I>::value,
    RandConstIter<I>,
    std::conditional_t<
        is_bidirectional_iterator<I>::value,
        BiConstIter<I>,
        std::conditional_t<
            is_forward_iterator<T>::value,
            FwdConstIter<T>,
            InConstIter<T>
        >
    >
>;

template <typename I>
constexpr ConstIter<I> make_const_iter(const I &iter)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return ConstIter<I>{ iter };
}

} // namespace ranges
} // namespace umigv

#endif
