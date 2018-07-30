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
#include "meta_iter.hpp"
#include "traits.hpp"

#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I>
class RandConstIter : public MetaIter<RandConstIter<I>> {
public:
    static_assert(is_random_access_iterator<I>::value,
                  "I must be a random access iterator");

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    friend MetaIter<RandConstIter<I>>;

    constexpr RandConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr reference deref() const {
        return static_cast<reference>(*current_);
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
};

template <typename I>
class BiConstIter : public MetaIter<BiConstIter<I>> {
public:
    static_assert(is_bidirectional_iterator<I>::value,
                  "I must be a bidirectional iterator");

    friend MetaIter<BiConstIter<I>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = std::add_pointer_t<add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr BiConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr reference deref() const {
        return static_cast<reference>(*current_);
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
};

template <typename I>
class FwdConstIter : public MetaIter<FwdConstIter<I>> {
public:
    static_assert(is_forward_iterator<I>::value,
                  "I must be a forward iterator");

    friend MetaIter<FwdConstIter<I>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = std::add_pointer_t<add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr FwdConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr reference deref() const {
        return static_cast<reference>(*current_);
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
};

template <typename I>
class InConstIter : public MetaIter<InConstIter<I>> {
public:
    static_assert(is_input_iterator<I>::value,
                  "I must be an input iterator");

    friend MetaIter<InConstIter<I>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = std::add_pointer_t<add_const_t<
        std::remove_pointer_t<iterator_pointer_t<I>>
    >>;
    using reference = add_const_t<iterator_reference_t<I>>;
    using value_type = iterator_value_t<I>;

    constexpr InConstIter(const I &current)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : current_{ current } { }

    constexpr reference deref() const {
        return static_cast<reference>(*current_);
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
};

template <typename I>
using ConstIter = std::conditional_t<
    is_random_access_iterator<I>::value,
    RandConstIter<I>,
    std::conditional_t<
        is_bidirectional_iterator<I>::value,
        BiConstIter<I>,
        std::conditional_t<
            is_forward_iterator<I>::value,
            FwdConstIter<I>,
            InConstIter<I>
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

namespace std {

template <typename I>
struct iterator_traits<umigv::ranges::RandConstIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = random_access_iterator_tag;
    using pointer = std::add_pointer_t<umigv::ranges::add_const_t<
        std::remove_pointer_t<typename iterator_traits<I>::pointer>
    >>;
    using reference =
        umigv::ranges::add_const_t<typename iterator_traits<I>::reference>;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<umigv::ranges::BiConstIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = bidirectional_iterator_tag;
    using pointer = std::add_pointer_t<umigv::ranges::add_const_t<
        std::remove_pointer_t<typename iterator_traits<I>::pointer>
    >>;
    using reference =
        umigv::ranges::add_const_t<typename iterator_traits<I>::reference>;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<umigv::ranges::FwdConstIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = forward_iterator_tag;
    using pointer = std::add_pointer_t<umigv::ranges::add_const_t<
        std::remove_pointer_t<typename iterator_traits<I>::pointer>
    >>;
    using reference =
        umigv::ranges::add_const_t<typename iterator_traits<I>::reference>;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I>
struct iterator_traits<umigv::ranges::InConstIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = input_iterator_tag;
    using pointer = std::add_pointer_t<umigv::ranges::add_const_t<
        std::remove_pointer_t<typename iterator_traits<I>::pointer>
    >>;
    using reference =
        umigv::ranges::add_const_t<typename iterator_traits<I>::reference>;
    using value_type = typename iterator_traits<I>::value_type;
};

} // namespace std

#endif
