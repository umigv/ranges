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

#ifndef UMIGV_RANGES_RANGE_HPP
#define UMIGV_RANGES_RANGE_HPP

#include "collect.hpp"
#include "const_iter.hpp"
#include "enumerated_range.hpp"
#include "filtered_range.hpp"
#include "mapped_range.hpp"
#include "range_adapter.hpp"
#include "range_fwd.hpp"
#include "zipped_range.hpp"

#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename R>
class Range {
public:
    using const_iterator = ConstIter<typename RangeTraits<R>::iterator>;
    using difference_type = typename RangeTraits<R>::difference_type;
    using iterator = typename RangeTraits<R>::iterator;
    using pointer = typename RangeTraits<R>::pointer;
    using reference = typename RangeTraits<R>::reference;
    using value_type = typename RangeTraits<R>::value_type;

    constexpr iterator begin() const noexcept {
        return as_base().begin();
    }

    constexpr const_iterator cbegin() const noexcept {
        return const_iterator{ begin() };
    }

    constexpr iterator end() const noexcept {
        return as_base().end();
    }

    constexpr const_iterator cend() const noexcept {
        return const_iterator{ end() };
    }

    template <typename F>
    constexpr MappedRange<iterator, F> map(F &&f)
    noexcept(noexcept(
        ::umigv::ranges::map(std::declval<Range>(), std::declval<F>())
    )) {
        return ::umigv::ranges::map(*this, std::forward<F>(f));
    }

    template <typename P>
    constexpr FilteredRange<iterator, P> filter(P &&predicate)
    noexcept(noexcept(
        ::umigv::ranges::filter(std::declval<Range>(), std::declval<P>())
    )) {
        return ::umigv::ranges::filter(*this, std::forward<P>(predicate));
    }

    constexpr EnumeratedRange<iterator> enumerate()
    noexcept(noexcept(
        ::umigv::ranges::enumerate(std::declval<Range>())
    )) {
        return ::umigv::ranges::enumerate(*this);
    }

    template <typename ...Rs>
    constexpr decltype(auto) zip(Rs &&...ranges)
    noexcept(noexcept(
        ::umigv::ranges::zip(std::declval<Range>(), std::declval<Rs>()...)
    )) {
        return ::umigv::ranges::zip(*this, std::forward<Rs>(ranges)...);
    }

    constexpr Collectable<iterator> collect() const {
        return { begin(), end() };
    }

    template <
        typename C,
        std::enable_if_t<
            std::is_constructible<C, iterator, iterator>::value,
            int
        > = 0
    >
    constexpr C collect() const {
        return C(begin(), end());
    }

    constexpr RangeAdapter<const_iterator> as_const() const noexcept {
        return ::umigv::ranges::adapt(cbegin(), cend());
    }

private:
    constexpr R& as_base() & noexcept {
        return static_cast<R&>(*this);
    }

    constexpr const R& as_base() const & noexcept {
        return static_cast<const R&>(*this);
    }

    constexpr R&& as_base() && noexcept {
        return static_cast<R&&>(*this);
    }

    constexpr const R&& as_base() const && noexcept {
        return static_cast<const R&&>(*this);
    }
};

} // namespace range
} // namespace umigv

#endif
