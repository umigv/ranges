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

#ifndef UMIGV_RANGES_CHECK_ITER_HPP
#define UMIGV_RANGES_CHECK_ITER_HPP

#include "iterator.hpp"
#include "traits.hpp"

#include <stdexcept>
#include <type_traits>

#include <iostream>

namespace umigv {
namespace ranges {

template <typename I>
class RandCheckIter : public RandomAccessIterator<RandCheckIter<I>> {
public:
    static_assert(is_random_access_iterator<I>::value,
                  "I must be a random access iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr RandCheckIter(const I &first, const I &last, const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : first_{ first }, last_{ last }, current_{ current } { }

    constexpr RandCheckIter& next() {
        if (current_ == last_) {
            throw std::out_of_range{ "RandCheckIter::next" };
        }

        ++current_;

        return *this;
    }

    constexpr RandCheckIter& prev() {
        if (current_ == first_) {
            throw std::out_of_range{ "RandCheckIter::prev" };
        }

        --current_;

        return *this;
    }

    constexpr RandCheckIter& step(difference_type n) {
        const auto first_dist = first_ - current_; // should be <= 0
        const auto last_dist = last_ - current_; // should be >= 0

        // stay in range [first, last]
        if (n < first_dist || n > last_dist) {
            throw std::out_of_range{ "RandCheckIter::step" };
        }

        current_ += n;

        return *this;
    }

    constexpr difference_type distance(const RandCheckIter &other) const {
        if (first_ != other.first_ || last_ != other.last_) {
            throw std::out_of_range{ "RandCheckIter::distance" };
        }

        return other.current_ - current_;
    }

    constexpr reference deref() const {
        // assumes we are in the range [first, last], as those are invariants
        // enforced by next(), prev(), and step()
        if (current_ == last_) {
            throw std::out_of_range{ "RandCheckIter::deref" };
        }

        return *current_;
    }

    constexpr bool is_equal(const RandCheckIter &other) const {
        if (first_ != other.first_ || last_ != other.last_) {
            throw std::out_of_range{ "RandCheckIter::is_equal" };
        }

        return current_ == other.current_;
    }

    constexpr bool is_less(const RandCheckIter &other) const {
        if (first_ != other.first_ || last_ != other.last_) {
            throw std::out_of_range{ "RandCheckIter::is_less" };
        }

        return current_ < other.current_;
    }

private:
    template <std::size_t N>
    constexpr void range_check(const char (&what)[N]) const {
        range_check(current_, what);
    }

    template <std::size_t N>
    constexpr void range_check(const I &iter, const char (&what)[N]) const {
        if (iter < first_ || iter > last_) {
            throw std::out_of_range{ what };
        }
    }

    I first_;
    I last_;
    I current_;
};

template <typename I>
class BiCheckIter : public BidirectionalIterator<BiCheckIter<I>> {
public:
    static_assert(is_bidirectional_iterator<I>::value,
                  "I must be a bidirectional iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr BiCheckIter(const I &first, const I &last, const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : first_{ first }, last_{ last }, current_{ current } { }

    constexpr BiCheckIter& next() {
        if (current_ == last_) {
            throw std::out_of_range{ "BiCheckIter::next" };
        }

        ++current_;

        return *this;
    }

    constexpr BiCheckIter& prev() {
        if (current_ == first_) {
            throw std::out_of_range{ "BiCheckIter::prev" };
        }

        --current_;

        return *this;
    }

    constexpr reference deref() const {
        if (current_ == last_) {
            throw std::out_of_range{ "BiCheckIter::deref" };
        }

        return *current_;
    }

    constexpr bool is_equal(const BiCheckIter &other) const {
        if (first_ != other.first_ || last_ != other.last_) {
            throw std::out_of_range{ "BiCheckIter::is_equal" };
        }

        return current_ == other.current_;
    }

private:
    I first_;
    I last_;
    I current_;
};

template <typename I>
class InCheckIter : public ForwardIterator<InCheckIter<I>> {
public:
    static_assert(is_input_iterator<I>::value, "I must be an input iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr InCheckIter(const I&, const I &last, const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : last_{ last }, current_{ current } { }

    constexpr InCheckIter& next() {
        if (current_ == last_) {
            throw std::out_of_range{ "InCheckIter::next" };
        }

        ++current_;

        return *this;
    }

    constexpr reference deref() const {
        if (current_ == last_) {
            throw std::out_of_range{ "InCheckIter::deref" };
        }

        return *current_;
    }

    constexpr bool is_equal(const InCheckIter &other) const {
        if (last_ != other.last_) {
            throw std::out_of_range{ "InCheckIter::is_equal" };
        }

        return current_ == other.current_;
    }

private:
    I last_;
    I current_;
};

template <typename T>
using CheckIter = std::conditional_t<
    is_random_access_iterator<T>::value,
    RandCheckIter<T>,
    std::conditional_t<
        is_bidirectional_iterator<T>::value,
        BiCheckIter<T>,
        InCheckIter<T>
    >
>;

template <typename I>
constexpr CheckIter<I> make_check_iter(const I &first, const I &last,
                                       const I &current)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return { first, last, current };
}

template <typename I>
constexpr IterPair<CheckIter<I>> make_check_iter(const I &first, const I &last)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return { { first, last, first }, { first, last, last } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename I>
struct iterator_traits<::umigv::ranges::RandCheckIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = typename iterator_traits<I>::iterator_category;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<::umigv::ranges::BiCheckIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = typename iterator_traits<I>::iterator_category;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<::umigv::ranges::InCheckIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = typename iterator_traits<I>::iterator_category;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

} // namespace std

#endif
