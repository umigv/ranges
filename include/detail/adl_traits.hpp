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

#ifndef UMIGV_RANGES_DETAIL_ADL_TRAITS_HPP
#define UMIGV_RANGES_DETAIL_ADL_TRAITS_HPP

#include <iterator>
#include <utility>

namespace umigv_ranges_detail_adl_traits {

using std::swap;
using std::begin;
using std::end;

template <typename ...Ts>
using VoidT = void;

template <bool B>
using BooleanConstant = std::integral_constant<bool, B>;

template <typename T, typename U, typename = void>
struct IsSwappableWith : std::false_type { };

template <typename T, typename U>
struct IsSwappableWith<T, U, VoidT<
    decltype(swap(std::declval<T>(), std::declval<U>())),
    decltype(swap(std::declval<U>(), std::declval<T>()))
>> : std::true_type { };

template <typename T>
struct IsSwappable : BooleanConstant<IsSwappableWith<T, T>::value> { };

template <typename T, typename U, bool = IsSwappableWith<T, U>::value>
struct IsNothrowSwappableWith : std::false_type { };

template <typename T, typename U>
struct IsNothrowSwappableWith<T, U, true> : BooleanConstant<
    noexcept(swap(std::declval<T>(), std::declval<U>()))
    && noexcept(swap(std::declval<U>(), std::declval<T>()))
> { };

template <typename T>
struct IsNothrowSwappable : BooleanConstant<
    IsNothrowSwappableWith<T, T>::value
> { };

template <typename T, typename = void>
struct IsBeginable : std::false_type { };

template <typename T>
struct IsBeginable<T, VoidT<
    decltype(begin(std::declval<T>()))>
> : std::true_type { };

template <typename T, bool = IsBeginable<T>::value>
struct IsNothrowBeginable : std::false_type { };

template <typename T>
struct IsNothrowBeginable<T, true> : BooleanConstant<noexcept(
    begin(std::declval<T>())
)> { };

template <typename T, typename = void>
struct IsEndable : std::false_type { };

template <typename T>
struct IsEndable<T, VoidT<
    decltype(end(std::declval<T>()))>
> : std::true_type { };

template <typename T, bool = IsEndable<T>::value>
struct IsNothrowEndable : std::false_type { };

template <typename T>
struct IsNothrowEndable<T, true> : BooleanConstant<noexcept(
    end(std::declval<T>())
)> { };

template <typename T, bool = IsBeginable<T>::value>
struct BeginResult { };

template <typename T>
struct BeginResult<T, true> {
    using type = decltype((begin(std::declval<T>())));
};

template <typename T, bool = IsEndable<T>::value>
struct EndResult { };

template <typename T>
struct EndResult<T, true> {
    using type = decltype((end(std::declval<T>())));
};

} // namespace umigv_ranges_detail_adl_traits

#endif
