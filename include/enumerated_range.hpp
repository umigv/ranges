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

#ifndef UMIGV_RANGES_ENUMERATED_RANGE_HPP
#define UMIGV_RANGES_ENUMERATED_RANGE_HPP

#include "enum_iter.hpp"
#include "iterator_traits.hpp"
#include "range_fwd.hpp"
#include "range_traits.hpp"
#include "traits.hpp"

#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

#include <type_safe/optional.hpp>

namespace umigv {
namespace ranges {

template <typename I>
class EnumeratedRange : public Range<EnumeratedRange<I>> {
public:
    using difference_type = RangeDiffT<EnumeratedRange>;
    using iterator = RangeIterT<EnumeratedRange>;
    using pointer = RangePtrT<EnumeratedRange>;
    using reference = RangeRefT<EnumeratedRange>;
    using value_type = RangeValT<EnumeratedRange>;

    constexpr EnumeratedRange(const I &first, const I &last)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : first_{ first }, last_{ last } { }

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

template <typename I>
struct RangeTraits<EnumeratedRange<I>> {
    using difference_type = IterDiffT<EnumIter<I>>;
    using iterator = EnumIter<I>;
    using pointer = IterPtrT<EnumIter<I>>;
    using reference = IterRefT<EnumIter<I>>;
    using value_type = IterValT<EnumIter<I>>;
};

template <typename R>
EnumeratedRange<RangeIterT<R>> enumerate(R &&range)
noexcept(std::is_nothrow_copy_constructible<RangeIterT<R>>::value
         && HAS_NOTHROW_BEGINEND<R>) {
    using std::begin;
    using std::end;

    return { begin(std::forward<R>(range)), end(std::forward<R>(range)) };
}

} // namespace umigv
} // namespace ranges

#endif
