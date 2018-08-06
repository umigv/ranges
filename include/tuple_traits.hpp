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

#ifndef UMIGV_RANGES_TUPLE_TRAITS_HPP
#define UMIGV_RANGES_TUPLE_TRAITS_HPP

#include "traits.hpp"

namespace umigv {
namespace ranges {

template <typename T, typename = void>
struct IsTuple : std::false_type { };

template <typename T>
struct IsTuple<T, VoidT<
    decltype(umigv_ranges_detail_tuple_traits::tuple_size<
        std::add_const<T>
    >::value)
>> : std::true_type { };

template <typename T>
constexpr bool IS_TUPLE = IsTuple<T>::value;

template <typename T, bool = IS_TUPLE<T>>
struct TupleSize { };

template <typename T>
struct TupleSize<T, true>
: std::integral_constant<std::size_t, std::tuple_size<T>::value> { };

template <typename T>
constexpr std::size_t TUPLE_SIZE = TupleSize<T>::value;

template <std::size_t I, typename T, bool = IS_GETTABLE<I, T>>
struct GetResult { };

template <std::size_t I, typename T>
struct GetResult<I, T, true> {
    using type = decltype((std::get<I>(std::declval<T>())));
};

template <std::size_t I, typename T>
using GetResultT = typename GetResult<I, T>::type;

} // namespace ranges
} // namespace umigv

#endif
