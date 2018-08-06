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

#ifndef UMIGV_RANGES_DETAIL_TUPLE_TRAITS_HPP
#define UMIGV_RANGES_DETAIL_TUPLE_TRAITS_HPP

#include "traits.hpp"

namespace umigv_ranges_detail_tuple_traits {

template <typename T, typename ...Ts>
using HeadT = T;

// this trick is licensed under the MIT license from libc++
template <typename T>
struct TupleSize { };

template <typename T>
struct TupleSize<HeadT<
    const T,
    std::enable_if_t<!std::is_volatile<T>::value>,
    std::integral_constant<std::size_t, sizeof(std::tuple_size<T>)>
>> : std::integral_constant<std::size_t, std::tuple_size<T>::value> { };

template <typename T>
struct TupleSize<HeadT<
    volatile T,
    std::enable_if_t<!std::is_const<T>::value>,
    std::integral_constant<std::size_t, sizeof(std::tuple_size<T>)>
>> : std::integral_constant<std::size_t, std::tuple_size<T>::value> { };

template <typename T>
struct TupleSize<HeadT<
    const volatile T,
    std::integral_constant<std::size_t, sizeof(std::tuple_size<T>)>
>> : std::integral_constant<std::size_t, std::tuple_size<T>::value> { };

} // namespace umigv_ranges_detail_tuple_traits

#endif
