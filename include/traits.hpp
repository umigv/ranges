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

#ifndef UMIGV_RANGES_TRAITS_HPP
#define UMIGV_RANGES_TRAITS_HPP

#include "detail/adl_traits.hpp"

#include "range_fwd.hpp"

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename ...Ts>
using VoidT = void;

template <bool B>
using BooleanConstant = std::integral_constant<bool, B>;

template <typename T, typename U, typename = void>
struct IsEqualityComparableWith : std::false_type { };

template <typename T, typename U>
struct IsEqualityComparableWith<T, U, VoidT<
    decltype(std::declval<T>() == std::declval<U>()),
    decltype(std::declval<U>() == std::declval<T>())
>> : std::true_type { };

template <typename T, typename U>
constexpr bool IS_EQUALITY_COMPARABLE_WITH =
    IsEqualityComparableWith<T, U>::value;

template <typename T>
struct IsEqualityComparable : BooleanConstant<
    IS_EQUALITY_COMPARABLE_WITH<T, T>
> { };

template <typename T>
constexpr bool IS_EQUALITY_COMPARABLE = IsEqualityComparable<T>::value;

template <typename T, typename U, bool = IS_EQUALITY_COMPARABLE_WITH<T, U>>
struct IsNothrowEqualityComparableWith : std::false_type { };

template <typename T, typename U>
struct IsNothrowEqualityComparableWith<T, U, true> : BooleanConstant<
    noexcept(std::declval<T>() == std::declval<U>())
    && noexcept(std::declval<U>() == std::declval<T>())
> { };

template <typename T, typename U>
constexpr bool IS_NOTHROW_EQUALITY_COMPARABLE_WITH =
    IsNothrowEqualityComparableWith<T, U>::value;

template <typename T>
struct IsNothrowEqualityComparable : BooleanConstant<
    IS_NOTHROW_EQUALITY_COMPARABLE_WITH<T, T>
> { };

template <typename T>
constexpr bool IS_NOTHROW_EQUALITY_COMPARABLE =
    IsNothrowEqualityComparable<T>::value;

template <typename T>
struct RemoveCvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using RemoveCvrefT = typename RemoveCvref<T>::type;

template <typename T>
struct Identity {
    using type = T;
};

template <typename T>
using IdentityT = typename Identity<T>::type;

template <typename...>
struct Conjunction : std::true_type { };

template <typename T>
struct Conjunction<T> : T { };

template <typename T, typename ...Ts>
struct Conjunction<T, Ts...>
: std::conditional_t<static_cast<bool>(T::value), Conjunction<Ts...>, T> { };

template <typename ...Ts>
constexpr bool CONJUNCTION = Conjunction<Ts...>::value;

template <typename...>
struct Disjunction : std::false_type { };

template <class T>
struct Disjunction<T> : T { };

template <class T, class... Ts>
struct Disjunction<T, Ts...>
: std::conditional_t<static_cast<bool>(T::value), T, Disjunction<Ts...>> { };

template <typename ...Ts>
constexpr bool DISJUNCTION = Disjunction<Ts...>::value;

template <typename T>
struct IsSwappable : BooleanConstant<
    umigv_ranges_detail_adl_traits::IsSwappable<T>::value
> { };

template <typename T>
constexpr bool IS_SWAPPABLE = IsSwappable<T>::value;

template <typename T, typename U>
struct IsSwappableWith : BooleanConstant<
    umigv_ranges_detail_adl_traits::IsSwappableWith<T, U>::value
> { };

template <typename T, typename U>
constexpr bool IS_SWAPPABLE_WITH = IsSwappableWith<T, U>::value;

template <typename T>
struct IsNothrowSwappable : BooleanConstant<
    umigv_ranges_detail_adl_traits::IsNothrowSwappable<T>::value
> { };

template <typename T>
constexpr bool IS_NOTHROW_SWAPPABLE = IsNothrowSwappable<T>::value;

template <typename T, typename U>
struct IsNothrowSwappableWith : BooleanConstant<
    umigv_ranges_detail_adl_traits::IsNothrowSwappableWith<T, U>::value
> { };

template <typename T, typename U>
constexpr bool IS_NOTHROW_SWAPPABLE_WITH = IsNothrowSwappableWith<T, U>::value;

template <
    typename T,
    bool = std::is_pointer<std::remove_reference_t<T>>::value
           || std::is_member_pointer<std::remove_reference_t<T>>::value
>
struct UnwrapPtrRef {
    using type = T;
};

template <typename T>
struct UnwrapPtrRef<T, true> {
    using type = std::remove_reference_t<T>;
};

template <typename T>
using UnwrapPtrRefT = typename UnwrapPtrRef<T>::type;

} // namespace ranges
} // namespace umigv

#endif
