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

#ifndef UMIGV_RANGES_APPLY_HPP
#define UMIGV_RANGES_APPLY_HPP

#include "detail/apply.hpp"
#include "invoke.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename C, typename T, typename = void>
struct is_applicable : std::false_type { };

template <typename C, typename T>
struct is_applicable<C, T, void_t<std::enable_if_t<is_tuple<T>::value>>>
: decltype(detail::check_applicable(
    std::declval<C>(),
    std::declval<T>(),
    std::make_index_sequence<tuple_size<T>::value>()
)) { };

template <typename C, typename T, typename = void>
struct is_nothrow_applicable : std::false_type { };

template <typename C, typename T>
struct is_nothrow_applicable<C, T, void_t<std::enable_if_t<is_tuple<T>::value>>>
: decltype(detail::check_nothrow_applicable(
    std::declval<C>(),
    std::declval<T>(),
    std::make_index_sequence<tuple_size<T>::value>()
)) { };

template <typename C, typename T, typename = void>
struct apply_result { };

template <typename C, typename T>
struct apply_result<C, T, void_t<std::enable_if_t<is_tuple<T>::value>>> {
    using type = typename decltype(detail::check_apply_result(
        std::declval<C>(),
        std::declval<T>(),
        std::make_index_sequence<tuple_size<T>::value>()
    ))::type;
};

template <typename C, typename T>
using apply_result_t = typename apply_result<C, T>::type;

template <typename C, typename T,
          std::enable_if_t<
              is_tuple<T>::value && is_applicable<C, T>::value, int
          > = 0>
constexpr apply_result_t<C, T> apply(C &&callable, T &&tuple)
noexcept(is_nothrow_applicable<C, T>::value) {
    return detail::apply(std::forward<C>(callable), std::forward<T>(tuple),
                         std::make_index_sequence<tuple_size<T>::value>());
}

} // namespace ranges
} // namespace umigv

#endif
