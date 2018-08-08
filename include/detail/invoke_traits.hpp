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

#ifndef UMIGV_RANGES_DETAIL_INVOKE_TRAITS_HPP
#define UMIGV_RANGES_DETAIL_INVOKE_TRAITS_HPP

#include "traits.hpp"

#include <type_traits>
#include <utility>

namespace umigv_ranges_detail_invoke_traits {

template <typename ...Ts>
using VoidT = void;

struct DotStarParenTag { };

struct GetDotStarParenTag { };

struct StarDotStarParenTag { };

struct DotStarTag { };

struct GetDotStarTag { };

struct StarDotStarTag { };

struct ParenTag { };

template <typename C, typename = void, typename ...Ts>
struct IsDotStarParenInvocable : std::false_type { };

template <typename C, typename T, typename ...Ts>
struct IsDotStarParenInvocable<C, VoidT<decltype(
    (std::declval<T>().*std::declval<C>())(std::declval<Ts>()...)
)>, T, Ts...> : std::true_type { };

template <typename C, typename = void, typename ...Ts>
struct IsGetDotStarParenInvocable : std::false_type { };

template <typename C, typename T, typename ...Ts>
struct IsGetDotStarParenInvocable<C, VoidT<decltype(
    (std::declval<T>().get().*std::declval<C>())(std::declval<Ts>()...)
)>, T, Ts...> : std::true_type { };

template <typename C, typename = void, typename ...Ts>
struct IsStarDotStarParenInvocable : std::false_type { };

template <typename C, typename T, typename ...Ts>
struct IsStarDotStarParenInvocable<C, VoidT<decltype(
    ((*std::declval<T>()).*std::declval<C>())(std::declval<Ts>()...)
)>, T, Ts...> : std::true_type { };

template <typename C, typename = void, typename ...Ts>
struct IsDotStarInvocable : std::false_type { };

template <typename C, typename T>
struct IsDotStarInvocable<C, VoidT<decltype(
    std::declval<T>().*std::declval<C>()
)>, T> : std::true_type { };

template <typename C, typename = void, typename ...Ts>
struct IsGetDotStarInvocable : std::false_type { };

template <typename C, typename T>
struct IsGetDotStarInvocable<C, VoidT<decltype(
    std::declval<T>().get().*std::declval<C>()
)>, T> : std::true_type { };

template <typename C, typename = void, typename ...Ts>
struct IsStarDotStarInvocable : std::false_type { };

template <typename C, typename T>
struct IsStarDotStarInvocable<C, VoidT<decltype(
    (*std::declval<T>()).*std::declval<C>()
)>, T> : std::true_type { };

template <typename C, typename T, typename ...Ts>
constexpr decltype(auto)
do_invoke(DotStarParenTag, C &&c, T &&t, Ts &&...ts)
noexcept(noexcept(
    (std::forward<T>(t).*std::forward<C>(c))(std::forward<Ts>(ts)...)
)) {
    return (std::forward<T>(t).*std::forward<C>(c))(std::forward<Ts>(ts)...);
}

template <typename C, typename T, typename ...Ts>
constexpr decltype(auto)
do_invoke(GetDotStarParenTag, C &&c, T &&t, Ts &&...ts)
noexcept(noexcept(
    (std::forward<T>(t).get().*std::forward<C>(c))(std::forward<Ts>(ts)...)
)) {
    return (std::forward<T>(t).get().*std::forward<C>(c))(
        std::forward<Ts>(ts)...
    );
}

template <typename C, typename T, typename ...Ts>
constexpr decltype(auto)
do_invoke(StarDotStarParenTag, C &&c, T &&t, Ts &&...ts)
noexcept(noexcept(
    ((*std::forward<C>(c)).*std::forward<C>(c))(std::forward<Ts>(ts)...)
)) {
    return ((*std::forward<T>(t)).*std::forward<C>(c))(std::forward<Ts>(ts)...);
}


template <typename C, typename T>
constexpr decltype(auto) do_invoke(DotStarTag, C &&c, T &&t)
noexcept(noexcept(std::forward<C>(c).*std::forward<T>(t))) {
    return std::forward<C>(c).*std::forward<T>(t);
}

template <typename C, typename T>
constexpr decltype(auto) do_invoke(GetDotStarTag, C &&c, T &&t)
noexcept(noexcept(std::forward<C>(c).get().*std::forward<T>(t))) {
    return std::forward<C>(c).get().*std::forward<T>(t);
}

template <typename C, typename T>
constexpr decltype(auto) do_invoke(StarDotStarTag, C &&c, T &&t)
noexcept(noexcept((*std::forward<C>(c)).*std::forward<T>(t))) {
    return (*std::forward<C>(c)).*std::forward<T>(t);
}

template <typename C, typename ...Ts>
constexpr decltype(auto) do_invoke(ParenTag, C &&c, Ts &&...ts)
noexcept(noexcept(std::forward<C>(c)(std::forward<Ts>(ts)...))) {
    return std::forward<C>(c)(std::forward<Ts>(ts)...);
}

} // namespace umigv_ranges_detail_invoke_traits

#endif
