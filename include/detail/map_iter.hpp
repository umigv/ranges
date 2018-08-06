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

#ifndef UMIGV_RANGES_DETAIL_MAP_ITER_HPP
#define UMIGV_RANGES_DETAIL_MAP_ITER_HPP

#include "../apply.hpp"
#include "../invoke.hpp"

#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {
namespace detail {

template <typename C, typename T,
          std::enable_if_t<IS_INVOCABLE<C&&, T&&>, int> = 0>
constexpr InvokeResultT<C&&, T&&> do_map(C &&callable, T &&t)
noexcept(IS_NOTHROW_INVOCABLE<C&&, T&&>) {
    return invoke(std::forward<C>(callable), std::forward<T>(t));
}

template <typename C, typename T,
          std::enable_if_t<!IS_INVOCABLE<C&&, T&&>
                           && IS_APPLICABLE<C&&, T&&>, int> = 0>
constexpr ApplyResultT<C&&, T&&> do_map(C &&callable, T &&t)
noexcept(IS_NOTHROW_APPLICABLE<C&&, T&&>) {
    return apply(std::forward<C>(callable), std::forward<T>(t));
}

template <typename C, typename T,
          bool = IS_INVOCABLE<C, T>, bool = IS_APPLICABLE<C, T>>
struct map_result { };

template <typename C, typename T, bool B>
struct map_result<C, T, true, B> {
    using type = InvokeResultT<C, T>;
};

template <typename C, typename T>
struct map_result<C, T, false, true> {
    using type = ApplyResultT<C, T>;
};

template <typename C, typename T>
using map_result_t = typename map_result<C, T>::type;

template <typename C, typename T>
struct is_nothrow_mappable : TrueTypeIfT<std::conditional_t<
    IS_INVOCABLE<C, T>,
    IsNothrowInvocable<C, T>,
    IsNothrowApplicable<C, T>
>::value> { };

} // namespace detail
} // namespace ranges
} // namespace umigv

#endif
