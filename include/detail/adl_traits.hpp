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

template <typename ...Ts>
using void_t = void;

using std::swap;
using std::begin;
using std::end;

template <typename T, typename U, typename = void>
struct is_swappable_with : std::false_type { };

template <typename T, typename U>
struct is_swappable_with<
    T, U,
    umigv::void_t<decltype(swap(std::declval<T>(), std::declval<U>()))>
> : std::true_type { };

template <typename T>
struct is_swappable : is_swappable_with<T, T> { };

template <typename T, typename U,
          bool IsSwappable = is_swappable_with<T, U>::value>
struct is_nothrow_swappable_with : std::false_type { };

template <typename T, typename U>
struct is_nothrow_swappable_with<T, U, true>
: umigv::true_type_if_t<noexcept(swap(std::declval<T>(),
                                      std::declval<U>()))> { };

template <typename T>
struct is_nothrow_swappable : is_nothrow_swappable_with<T, T> { };

template <typename T, typename = void>
struct has_begin : std::false_type { };

template <typename T>
struct has_begin<T, umigv::void_t<decltype(begin(std::declval<T>()))>>
: std::true_type { };

template <typename T, typename = void>
struct has_end : std::false_type { };

template <typename T>
struct has_end<T, umigv::void_t<decltype(end(std::declval<T>()))>>
: std::true_type { };

template <typename T, bool = has_begin<T>::value>
struct has_nothrow_begin : std::false_type { };

template <typename T>
struct has_nothrow_begin<T, true>
: umigv::true_type_if_t<noexcept(begin(std::declval<T>()))> { };

template <typename T, bool = has_end<T>::value>
struct has_nothrow_end : std::false_type { };

template <typename T>
struct has_nothrow_end<T, true>
: umigv::true_type_if_t<noexcept(end(std::declval<T>()))> { };

template <typename T, bool = has_begin<T>::value>
struct begin_result { };

template <typename T>
struct begin_result<T, true> {
    using type = decltype(begin(std::declval<T>()));
};

template <typename T>
using begin_result_t = typename begin_result<T>::type;

template <typename T, bool = has_end<T>::value>
struct end_result { };

template <typename T>
struct end_result<T, true> {
    using type = decltype(end(std::declval<T>()));
};

template <typename T>
using end_result_t = typename end_result<T>::type;

} // namespace umigv_detail_adl

#endif
