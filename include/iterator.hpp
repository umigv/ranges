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

#ifndef UMIGV_RANGES_ITERATOR_HPP
#define UMIGV_RANGES_ITERATOR_HPP

#include "traits.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename I>
class InputIterator {
public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr I& next() {
        return as_base().next();
    }

    constexpr reference deref() const {
        return as_base().deref();
    }

    constexpr bool is_equal(const I &other) const {
        return as_base().eq(other);
    }

    constexpr reference operator*() const {
        return deref();
    }

    constexpr pointer operator->() const {
        return static_cast<pointer>(std::addressof(deref()));
    }

    constexpr I& operator++() {
        return next();
    }

    constexpr I operator++(int) {
        const auto current = as_base();

        next();

        return current;
    }

    friend constexpr bool operator==(const I &lhs, const I &rhs) {
        return lhs.is_equal(rhs);
    }

    friend constexpr bool operator!=(const I &lhs, const I &rhs) {
        return !lhs.is_equal(rhs);
    }

private:
    constexpr I& as_base() & noexcept {
        return static_cast<I&>(*this);
    }

    constexpr const I& as_base() const & noexcept {
        return static_cast<const I&>(*this);
    }

    constexpr I&& as_base() && noexcept {
        return static_cast<I&&>(*this);
    }

    constexpr const I&& as_base() const && noexcept {
        return static_cast<const I&&>(*this);
    }
};

template <typename I>
class ForwardIterator : public InputIterator<I> {
public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;
};

template <typename I>
class BidirectionalIterator : public ForwardIterator<I> {
public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr I& prev() {
        return as_base().prev();
    }

    constexpr I& operator--() {
        return prev();
    }

    constexpr I operator--(int) {
        const auto current = as_base();

        prev();

        return current;
    }

private:
    constexpr I& as_base() & noexcept {
        return static_cast<I&>(*this);
    }

    constexpr const I& as_base() const & noexcept {
        return static_cast<const I&>(*this);
    }

    constexpr I&& as_base() && noexcept {
        return static_cast<I&&>(*this);
    }

    constexpr const I&& as_base() const && noexcept {
        return static_cast<const I&&>(*this);
    }
};

template <typename I>
class RandomAccessIterator : public BidirectionalIterator<I> {
public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = iterator_category_t<I>;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr I& step(difference_type n) {
        return as_base().step(n);
    }

    constexpr I& next() {
        return step(1);
    }

    constexpr I& prev() {
        return step(-1);
    }

    constexpr difference_type distance(const I &other) const {
        return as_base().distance(other);
    }

    constexpr bool is_equal(const I &other) {
        return !is_less(other) && !other.is_less(*this);
    }

    constexpr bool is_less(const I &other) {
        return as_base().is_less(other);
    }

    constexpr I& operator+=(difference_type n) {
        return step(n);
    }

    constexpr I& operator-=(difference_type n) {
        return step(-n);
    }

    constexpr reference operator[](difference_type n) const {
        auto copy = as_base();

        return copy.step(n).deref();
    }

    friend constexpr I operator+(I iter, difference_type n) {
        return iter.step(n);
    }

    friend constexpr I operator+(difference_type n, I iter) {
        return iter.step(n);
    }

    friend constexpr I operator-(I iter, difference_type n) {
        return iter.step(-n);
    }

    friend constexpr difference_type operator-(const I &lhs, const I &rhs) {
        return rhs.distance(lhs);
    }

    friend constexpr bool operator<(const I &lhs, const I &rhs) {
        return lhs.is_less(rhs);
    }

    friend constexpr bool operator<=(const I &lhs, const I &rhs) {
        return !rhs.is_less(lhs);
    }

    friend constexpr bool operator>(const I &lhs, const I &rhs) {
        return rhs.is_less(lhs);
    }

    friend constexpr bool operator>=(const I &lhs, const I &rhs) {
        return !lhs.is_less(rhs);
    }

private:
    constexpr I& as_base() & noexcept {
        return static_cast<I&>(*this);
    }

    constexpr const I& as_base() const & noexcept {
        return static_cast<const I&>(*this);
    }

    constexpr I&& as_base() && noexcept {
        return static_cast<I&&>(*this);
    }

    constexpr const I&& as_base() const && noexcept {
        return static_cast<const I&&>(*this);
    }
};

template <typename I>
struct IterPair {
    static_assert(is_iterator<I>::value, "I must be an iterator");

    I first;
    I last;
};

template <typename I>
struct IterTriple {
    static_assert(is_iterator<I>::value, "I must be an iterator");

    I first;
    I last;
    I current;
};

} // namespace ranges
} // namespace umigv

#endif
