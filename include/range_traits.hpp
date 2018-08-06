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

#ifndef UMIGV_RANGES_RANGE_TRAITS_HPP
#define UMIGV_RANGES_RANGE_TRAITS_HPP

#include "detail/adl_traits.hpp"

#include <cstddef>
#include <initializer_list>

namespace umigv {
namespace ranges {

template <typename R>
struct RangeTraits {
    using difference_type = typename R::difference_type;
    using iterator = typename R::iterator;
    using pointer = typename R::pointer;
    using reference = typename R::reference;
    using value_type = typename R::value_type;
};

template <typename T, std::size_t N>
struct RangeTraits<T[N]> {
    using difference_type = std::ptrdiff_t;
    using iterator = T*;
    using pointer = T*;
    using reference = T&;
    using value_type = T;
};

template <typename T>
struct HasBeginend : TrueTypeIfT<
    umigv_ranges_detail_adl_traits::HasBegin<T>::value
    && umigv_ranges_detail_adl_traits::HasEnd<T>::value
> { };

template <typename T>
constexpr bool HAS_BEGINEND = HasBeginend<T>::value;

template <typename T>
struct HasNothrowBeginend : TrueTypeIfT<
    umigv_ranges_detail_adl_traits::HasNothrowBegin<T>::value
    && umigv_ranges_detail_adl_traits::HasNothrowEnd<T>::value
> { };

template <typename T>
constexpr bool HAS_NOTHROW_BEGINEND = HasNothrowBeginend<T>::value;

template <typename T, typename = void>
struct HasRangeTraits : std::false_type { };

template <typename T>
struct HasRangeTraits<T, VoidT<
    typename RangeTraits<T>::difference_type,
    typename RangeTraits<T>::iterator,
    typename RangeTraits<T>::pointer,
    typename RangeTraits<T>::reference,
    typename RangeTraits<T>::value_type
>> : std::true_type { };

template <typename T>
constexpr bool HAS_RANGE_TRAITS = HasRangeTraits<T>::value;

template <typename T, bool = HAS_RANGE_TRAITS<T>>
struct RangeDiff { };

template <typename T>
struct RangeDiff<T, true> {
    using type = typename RangeTraits<T>::difference_type;
};

template <typename T>
using RangeDiffT = typename RangeDiff<T>::type;

template <typename T, bool = HAS_RANGE_TRAITS<T>>
struct RangeIter { };

template <typename T>
struct RangeIter<T, true> {
    using type = typename RangeTraits<T>::iterator;
};

template <typename T>
using RangeIterT = typename RangeIter<T>::type;

template <typename T, bool = HAS_RANGE_TRAITS<T>>
struct RangePtr { };

template <typename T>
struct RangePtr<T, true> {
    using type = typename RangeTraits<T>::pointer;
};

template <typename T>
using RangePtrT = typename RangePtr<T>::type;

template <typename T, bool = HAS_RANGE_TRAITS<T>>
struct RangeRef { };

template <typename T>
struct RangeRef<T, true> {
    using type = typename RangeTraits<T>::reference;
};

template <typename T>
using RangeRefT = typename RangeRef<T>::type;

template <typename T, bool = HAS_RANGE_TRAITS<T>>
struct RangeVal { };

template <typename T>
struct RangeVal<T, true> {
    using type = typename RangeTraits<T>::value_type;
};

template <typename T>
using RangeValT = typename RangeVal<T>::type;

template <typename T>
struct IsRange : TrueTypeIfT<
    HasBeginend<T>::value && HasRangeTraits<T>::value
> { };

template <typename T>
constexpr bool IS_RANGE = IsRange<T>::value;

} // namespace ranges
} // namespace umigv

#endif
