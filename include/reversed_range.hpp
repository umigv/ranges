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

#ifndef UMIGV_RANGES_REVERSED_RANGE_HPP
#define UMIGV_RANGES_REVERSED_RANGE_HPP

#include "range_fwd.hpp"
#include "rev_iter.hpp"
#include "range_traits.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename I>
class ReversedRange : public Range<ReversedRange<I>> {
public:
    using size_type = RangeSizeT<ReversedRange>;
    using iterator = RangeIterT<ReversedRange>;
    using reference = RangeRefT<ReversedRange>;
    using value_type = RangeValT<ReversedRange>;

    constexpr ReversedRange(const I &first, const I &last)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : first_{ last }, last_{ first } { }

    constexpr iterator begin() const
    noexcept(std::is_nothrow_copy_constructible<iterator>::value) {
        return first_;
    }

    constexpr iterator end() const
    noexcept(std::is_nothrow_copy_constructible<iterator>::value) {
        return last_;
    }

private:
    iterator first_;
    iterator last_;
};

template <typename R>
constexpr ReversedRange<BeginResultT<R&&>> reverse(R &&range)
noexcept(std::is_nothrow_copy_constructible<BeginResultT<R&&>>::value
         && IS_NOTHROW_BEGINENDABLE<R&&>) {
    static_assert(IS_BEGINENDABLE<R&&>, "R must have a begin and end");

    using std::begin;
    using std::end;

    return { begin(std::forward<R>(range)), end(std::forward<R>(range)) };
}

template <typename I>
struct RangeTraits<ReversedRange<I>> {
    using size_type = std::make_signed_t<IterDiffT<RevIter<I>>>;
    using iterator = RevIter<I>;
    using reference = IterRefT<RevIter<I>>;
    using value_type = IterValT<RevIter<I>>;
};

} // namespace ranges
} // namespace umigv

#endif
