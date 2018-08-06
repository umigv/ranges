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
#include "iterator_traits.hpp"
#include "meta_iter.hpp"
#include "range_traits.hpp"
#include "traits.hpp"

#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I>
class ConstIter : public MetaIter<ConstIter<I>> {
public:
    static_assert(IS_IN_ITER<I>, "I must be an input iterator");

    using difference_type = IterDiffT<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<std::add_const_t<
        std::remove_pointer_t<IterPtrT<I>>
    >>;
    using reference = std::add_const_t<IterRefT<I>>;
    using value_type = IterValT<I>;

    friend MetaIter<ConstIter<I>>;

    constexpr ConstIter(const I &current)
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
constexpr ConstIter<I> make_const_iter(const I &iter)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return ConstIter<I>{ iter };
}

template <typename I>
constexpr IterPair<ConstIter<I>> make_const_iter(const I &first, const I &last)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return { ConstIter<I>{ first }, ConstIter<I>{ last } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename I>
struct iterator_traits<umigv::ranges::ConstIter<I>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = typename iterator_traits<I>::iterator_category;
    using pointer = std::add_pointer_t<add_const_t<
        std::remove_pointer_t<typename iterator_traits<I>::pointer>
    >>;
    using reference = add_const_t<typename iterator_traits<I>::reference>;
    using value_type = typename iterator_traits<I>::value_type;
};

} // namespace std

#endif
