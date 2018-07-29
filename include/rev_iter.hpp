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

#ifndef UMIGV_RANGES_REV_ITER_HPP
#define UMIGV_RANGES_REV_ITER_HPP

#include "iterator.hpp"
#include "traits.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename T>
class RandRevIter : public RandomAccessIterator<RandRevIter<T>> {
public:
    static_assert(is_random_access_iterator<T>::value,
                  "T must be a random access iterator");

    using difference_type = iterator_difference_t<T>;
    using iterator_category = iterator_category_t<T>;
    using pointer = iterator_pointer_t<T>;
    using reference = iterator_reference_t<T>;
    using value_type = iterator_value_t<T>;

    constexpr explicit RandRevIter(T current) : base_{ current } { }

    constexpr RandRevIter& next() {
        --base_;

        return *this;
    }

    constexpr RandRevIter& prev() {
        ++base_;

        return *this;
    }

    constexpr RandRevIter& step(difference_type n) {
        base_ += n;

        return *this;
    }

    constexpr difference_type distance(const RandRevIter &other) const {
        return base_ - other.base_;
    }

    constexpr reference deref() const {
        return *std::prev(base_);
    }

    constexpr bool is_equal(const RandRevIter &other) const {
        return base_ == other.base_;
    }

    constexpr bool is_less(const RandRevIter &other) const {
        return other.base_ < base_;
    }

private:
    T base_;
};

template <typename T>
class BiRevIter : public BidirectionalIterator<BiRevIter<T>> {
public:
    static_assert(is_bidirectional_iterator<T>::value,
                  "T must be a bidirectional iterator");

    using difference_type = iterator_difference_t<T>;
    using iterator_category = iterator_category_t<T>;
    using pointer = iterator_pointer_t<T>;
    using reference = iterator_reference_t<T>;
    using value_type = iterator_value_t<T>;

    constexpr explicit BiRevIter(T current) : base_{ current } { }

    constexpr BiRevIter& next() {
        --base_;

        return *this;
    }

    constexpr BiRevIter& prev() {
        ++base_;

        return *this;
    }

    constexpr reference deref() const {
        return *std::prev(base_);
    }

    constexpr bool is_equal(const BiRevIter &other) const {
        return base_ == other.base_;
    }

    constexpr bool is_less(const BiRevIter &other) const {
        return other.base_ < base_;
    }

private:
    T base_;
};

template <typename T>
using RevIter = std::conditional_t<
    is_random_access_iterator<T>::value,
    RandRevIter<T>,
    BiRevIter<T>
>;

template <typename T>
constexpr RevIter<T> make_rev_iter(const T &current) {
    return RevIter<T>{ current };
}

template <typename T>
constexpr IterPair<RevIter<T>> make_rev_iter(const T &first, const T &last) {
    return { RevIter<T>{ last }, RevIter<T>{ first } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename T>
struct iterator_traits<::umigv::ranges::BiRevIter<T>> {
    using difference_type = typename iterator_traits<T>::difference_type;
    using iterator_category = typename iterator_traits<T>::iterator_category;
    using pointer = typename iterator_traits<T>::pointer;
    using reference = typename iterator_traits<T>::reference;
    using value_type = typename iterator_traits<T>::value_type;
};

template <typename T>
struct iterator_traits<::umigv::ranges::RandRevIter<T>> {
    using difference_type = typename iterator_traits<T>::difference_type;
    using iterator_category = typename iterator_traits<T>::iterator_category;
    using pointer = typename iterator_traits<T>::pointer;
    using reference = typename iterator_traits<T>::reference;
    using value_type = typename iterator_traits<T>::value_type;
};

} // namespace std

#endif
