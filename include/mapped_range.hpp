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

#ifndef UMIGV_RANGES_MAPPED_RANGE_HPP
#define UMIGV_RANGES_MAPPED_RANGE_HPP

#include "invoke.hpp"
#include "map_iter.hpp"
#include "range_fwd.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, typename F>
class MappedRange : public Range<MappedRange<I, F>> {
    using IteratorT = MapIter<I, F>;

public:
    using difference_type = typename RangeTraits<MappedRange>::difference_type;
    using iterator = typename RangeTraits<MappedRange>::iterator;
    using pointer = typename RangeTraits<MappedRange>::pointer;
    using reference = typename RangeTraits<MappedRange>::reference;
    using value_type = typename RangeTraits<MappedRange>::value_type;

    constexpr MappedRange(const I &first, const I &last, const F &f)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<F>::value)
    : first_{ first, f }, last_{ last, f } { }

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

template <typename R, typename F>
constexpr decltype(auto) map(R &&range, F &&f)
noexcept(std::is_nothrow_copy_constructible<begin_result_t<R>>::value
         && std::is_nothrow_copy_constructible<F>::value) {
    using std::begin;
    using std::end;

    return MappedRange<begin_result_t<R>, std::decay_t<F>>{
        begin(std::forward<R>(range)),
        end(std::forward<R>(range)),
        std::forward<F>(f),
    };
}

template <typename I, typename F>
struct RangeTraits<MappedRange<I, F>> {
    using difference_type = iterator_difference_t<MapIter<I, F>>;
    using iterator = MapIter<I, F>;
    using pointer = iterator_pointer_t<MapIter<I, F>>;
    using reference = iterator_reference_t<MapIter<I, F>>;
    using value_type = iterator_value_t<MapIter<I, F>>;
};

} // namespace ranges
} // namespace umigv

#endif
