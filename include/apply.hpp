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
struct IsApplicable : std::false_type { };

template <typename C, typename T>
struct IsApplicable<C, T, VoidT<std::enable_if_t<IS_TUPLE<T>>>>
: decltype(umigv_ranges_apply_detail::check_applicable(
    std::declval<C>(),
    std::declval<T>(),
    std::make_index_sequence<TUPLE_SIZE<T>>()
)) { };

template <typename C, typename T>
constexpr bool IS_APPLICABLE = IsApplicable<C, T>::value;

template <typename C, typename T, typename = void>
struct IsNothrowApplicable : std::false_type { };

template <typename C, typename T>
struct IsNothrowApplicable<C, T, VoidT<std::enable_if_t<IS_TUPLE<T>>>>
: decltype(umigv_ranges_apply_detail::check_nothrow_applicable(
    std::declval<C>(),
    std::declval<T>(),
    std::make_index_sequence<TUPLE_SIZE<T>>()
)) { };

template <typename C, typename T>
constexpr bool IS_NOTHROW_APPLICABLE = IsNothrowApplicable<C, T>::value;

template <typename C, typename T, typename = void>
struct ApplyResult { };

template <typename C, typename T>
struct ApplyResult<C, T, VoidT<std::enable_if_t<IS_TUPLE<T>>>> {
    using type = typename decltype(
        umigv_ranges_apply_detail::check_apply_result(
            std::declval<C>(),
            std::declval<T>(),
            std::make_index_sequence<TUPLE_SIZE<T>>()
        )
    )::type;
};

template <typename C, typename T>
using ApplyResultT = typename ApplyResult<C, T>::type;

template <typename C, typename T,
          std::enable_if_t<
              IS_TUPLE<T> && IS_APPLICABLE<C, T>, int
          > = 0>
constexpr ApplyResultT<C, T> apply(C &&callable, T &&tuple)
noexcept(IsNothrowApplicable<C, T>::value) {
    return umigv_ranges_apply_detail::apply(
        std::forward<C>(callable),
        std::forward<T>(tuple),
        std::make_index_sequence<TUPLE_SIZE<T>>()
    );
}

} // namespace ranges
} // namespace umigv

#endif
