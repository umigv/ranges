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

#ifndef UMIGV_RANGES_META_ITER_HPP
#define UMIGV_RANGES_META_ITER_HPP

#include "iterator.hpp"
#include "traits.hpp"

#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I>
class RandMetaIter : public RandomAccessIterator<I> {
public:
    static_assert(IS_RAND_ITER<I>, "I must be a random access iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = IterPtrT<I>;
    using reference = IterRefT<I>;
    using value_type = IterValT<I>;

    constexpr I& next() {
        ++as_base().underlying();

        return as_base();
    }

    constexpr I& prev() {
        --as_base().underlying();

        return as_base();
    }

    constexpr I& step(difference_type n) {
        as_base().underlying() += n;

        return as_base();
    }

    constexpr difference_type distance(const I &other) const {
        return other.underlying() - as_base().underlying();
    }

    constexpr reference deref() const {
        return *as_base().underlying();
    }

    constexpr bool is_equal(const I &other) const {
        return as_base().underlying() == other.underlying();
    }

    constexpr bool is_less(const I &other) const {
        return as_base().underlying() < other.underlying();
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
class BiMetaIter : public BidirectionalIterator<I> {
public:
    static_assert(IS_BIDIR_ITER<I>, "I must be a bidirectional iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = IterPtrT<I>;
    using reference = IterRefT<I>;
    using value_type = IterValT<I>;

    constexpr I& next() {
        ++as_base().underlying();

        return as_base();
    }

    constexpr I& prev() {
        --as_base().underlying();

        return as_base();
    }

    constexpr reference deref() const {
        return *as_base().underlying();
    }

    constexpr bool is_equal(const I &other) const {
        return as_base().underlying() == other.underlying();
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
class FwdMetaIter : public ForwardIterator<I> {
public:
    static_assert(IS_FWD_ITER<I>, "I must be a forward iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = IterPtrT<I>;
    using reference = IterRefT<I>;
    using value_type = IterValT<I>;

    constexpr I& next() {
        ++as_base().underlying();

        return as_base();
    }

    constexpr reference deref() const {
        return *as_base().underlying();
    }

    constexpr bool is_equal(const I &other) const {
        return as_base().underlying() == other.underlying();
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
class InMetaIter : public InputIterator<I> {
public:
    static_assert(IS_IN_ITER<I>, "I must be an input iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = IterPtrT<I>;
    using reference = IterRefT<I>;
    using value_type = IterValT<I>;

    constexpr I& next() {
        ++as_base().underlying();

        return as_base();
    }

    constexpr reference deref() const {
        return *as_base().underlying();
    }

    constexpr bool is_equal(const I &other) const {
        return as_base().underlying() == other.underlying();
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
using MetaIter = RandIterSwitchT<
    I,
    RandMetaIter<I>,
    BiMetaIter<I>,
    FwdMetaIter<I>,
    InMetaIter<I>
>;

} // namespace ranges
} // namespace umigv

namespace std {

template <typename I>
struct iterator_traits<::umigv::ranges::RandMetaIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = random_access_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<::umigv::ranges::BiMetaIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = bidirectional_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<::umigv::ranges::FwdMetaIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = forward_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<::umigv::ranges::InMetaIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = input_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

} // namespace std

#endif
