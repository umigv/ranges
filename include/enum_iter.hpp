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

#ifndef UMIGV_RANGES_ENUM_ITER_HPP
#define UMIGV_RANGES_ENUM_ITER_HPP

#include "iterator.hpp"
#include "meta_iter.hpp"
#include "traits.hpp"

#include <cstddef>
#include <iterator>
#include <utility>

namespace umigv {
namespace ranges {

template <typename I>
class RandEnumIter : public MetaIter<RandEnumIter<I>> {
public:
    static_assert(IS_RAND_ITER<I>, "I must be a random access iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = const std::pair<std::size_t, IterRefT<I>>*;
    using reference = std::pair<std::size_t, IterRefT<I>>;
    using value_type = std::pair<std::size_t, IterValT<I>>;

    friend MetaIter<RandEnumIter<I>>;

    constexpr explicit RandEnumIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr RandEnumIter& next() {
        ++current_;
        ++index_;

        return *this;
    }

    constexpr RandEnumIter& prev() {
        --current_;
        --index_;

        return *this;
    }

    constexpr RandEnumIter& step(difference_type n) {
        current_ += n;
        index_ += n;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ index_, *current_ };
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    difference_type index_{ };
};

template <typename I>
class BiEnumIter : public MetaIter<BiEnumIter<I>> {
public:
    static_assert(IS_BIDIR_ITER<I>, "I must be a bidirectional iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = const std::pair<std::size_t, IterRefT<I>>*;
    using reference = std::pair<std::size_t, IterRefT<I>>;
    using value_type = std::pair<std::size_t, IterValT<I>>;

    friend MetaIter<BiEnumIter<I>>;

    constexpr explicit BiEnumIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr BiEnumIter& next() {
        ++current_;
        ++index_;

        return *this;
    }

    constexpr BiEnumIter& prev() {
        --current_;
        --index_;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ index_, *current_ };
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    difference_type index_{ };
};

template <typename I>
class FwdEnumIter : public MetaIter<FwdEnumIter<I>> {
public:
    static_assert(IS_FWD_ITER<I>, "I must be a forward iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = const std::pair<std::size_t, IterRefT<I>>*;
    using reference = std::pair<std::size_t, IterRefT<I>>;
    using value_type = std::pair<std::size_t, IterValT<I>>;

    friend MetaIter<FwdEnumIter<I>>;

    constexpr explicit FwdEnumIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr FwdEnumIter& next() {
        ++current_;
        ++index_;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ index_, *current_ };
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    difference_type index_{ };
};

template <typename I>
class InEnumIter : public MetaIter<InEnumIter<I>> {
public:
    static_assert(IS_IN_ITER<I>, "I must be am input iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = const std::pair<std::size_t, IterRefT<I>>*;
    using reference = std::pair<std::size_t, IterRefT<I>>;
    using value_type = std::pair<std::size_t, IterValT<I>>;

    friend MetaIter<InEnumIter<I>>;

    constexpr explicit InEnumIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr InEnumIter& next() {
        ++current_;
        ++index_;

        return *this;
    }

    constexpr reference deref() const {
        return reference{ index_, *current_ };
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    difference_type index_{ };
};

template <typename I>
using EnumIter = RandIterSwitchT<
    I,
    RandEnumIter<I>,
    BiEnumIter<I>,
    FwdEnumIter<I>,
    InEnumIter<I>
>;

template <typename I>
constexpr EnumIter<I> make_enum_iter(const I &current)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return EnumIter<I>{ current };
}

template <typename I>
constexpr IterPair<EnumIter<I>> make_enum_iter(const I &first, const I &last)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return { EnumIter<I>{ first }, EnumIter<I>{ last } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename I>
struct iterator_traits<umigv::ranges::RandEnumIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = const std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >*;
    using reference = std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >;
    using value_type = std::pair<
        std::size_t,
        typename iterator_traits<I>::value_type
    >;
};

template <typename I>
struct iterator_traits<umigv::ranges::BiEnumIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = const std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >*;
    using reference = std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >;
    using value_type = std::pair<
        std::size_t,
        typename iterator_traits<I>::value_type
    >;
};

template <typename I>
struct iterator_traits<umigv::ranges::FwdEnumIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::forward_iterator_tag;
    using pointer = const std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >*;
    using reference = std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >;
    using value_type = std::pair<
        std::size_t,
        typename iterator_traits<I>::value_type
    >;
};

template <typename I>
struct iterator_traits<umigv::ranges::InEnumIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::input_iterator_tag;
    using pointer = const std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >*;
    using reference = std::pair<
        std::size_t,
        typename iterator_traits<I>::reference
    >;
    using value_type = std::pair<
        std::size_t,
        typename iterator_traits<I>::value_type
    >;
};

} // namespace std

#endif
