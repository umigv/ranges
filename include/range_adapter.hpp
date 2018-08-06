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

#ifndef UMIGV_RANGES_RANGE_ADAPTER_HPP
#define UMIGV_RANGES_RANGE_ADAPTER_HPP

#include "iterator_traits.hpp"
#include "range_fwd.hpp"
#include "range_traits.hpp"
#include "traits.hpp"

#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I>
class RangeAdapter : public Range<RangeAdapter<I>> {
public:
    static_assert(IS_ITER<I>, "I must be an iterator");

    using difference_type = RangeDiffT<RangeAdapter>;
    using iterator = RangeIterT<RangeAdapter>;
    using pointer = RangePtrT<RangeAdapter>;
    using reference = RangeRefT<RangeAdapter>;
    using value_type = RangeValT<RangeAdapter>;

    constexpr RangeAdapter(const I &first, const I &last)
    : first_{ first }, last_{ last } { }

    constexpr iterator begin() const noexcept {
        return first_;
    }

    constexpr iterator end() const noexcept {
        return last_;
    }

private:
    I first_;
    I last_;
};

template <typename I>
constexpr RangeAdapter<I> adapt(const I &first, const I &last)
noexcept(std::is_nothrow_copy_constructible<I>::value) {
    return { first, last };
}

template <typename R>
constexpr RangeAdapter<RangeIterT<R>> adapt(R &&range)
noexcept(HAS_NOTHROW_BEGINEND<R>
         && std::is_nothrow_copy_constructible<RangeIterT<R>>::value) {
    static_assert(HAS_BEGINEND<R>, "R must have a begin and end");

    using std::begin;
    using std::end;

    return { begin(std::forward<R>(range)), end(std::forward<R>(range)) };
}

template <typename T>
constexpr RangeAdapter<RangeIterT<std::initializer_list<T>>>
adapt(std::initializer_list<T> list) noexcept {
    using std::begin;
    using std::end;

    return { begin(list), end(list) };
}

template <typename I>
struct RangeTraits<RangeAdapter<I>> {
    using difference_type = IterDiffT<I>;
    using iterator = I;
    using pointer = IterPtrT<I>;
    using reference = IterRefT<I>;
    using value_type = IterValT<I>;
};

} // namespace ranges
} // namespace umigv

#endif
