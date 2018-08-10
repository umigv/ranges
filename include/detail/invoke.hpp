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

#ifndef UMIGV_RANGES_DETAIL_INVOKE_HPP
#define UMIGV_RANGES_DETAIL_INVOKE_HPP

#include "traits.hpp"

#include <type_traits>
#include <utility>

namespace umigv_ranges_detail_invoke {

template <typename ...Ts>
using VoidT = void;

struct DotStarParenTag { };

struct GetDotStarParenTag { };

struct StarDotStarParenTag { };

struct DotStarTag { };

struct GetDotStarTag { };

struct StarDotStarTag { };

struct ParenTag { };

struct NotInvocableTag { };

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

template <typename C, typename = void, typename ...Ts>
struct IsParenInvocable : std::false_type { };

template <typename C, typename ...Ts>
struct IsParenInvocable<C, VoidT<decltype(
    std::declval<C>()(std::declval<Ts>()...)
)>, Ts...> : std::true_type { };

template <typename C, typename ...Ts>
using InvokeTag = std::conditional_t<
    IsDotStarParenInvocable<C, void, Ts...>::value,
    DotStarParenTag,
    std::conditional_t<
        IsGetDotStarParenInvocable<C, void, Ts...>::value,
        GetDotStarParenTag,
        std::conditional_t<
            IsStarDotStarParenInvocable<C, void, Ts...>::value,
            StarDotStarParenTag,
            std::conditional_t<
                IsDotStarInvocable<C, void, Ts...>::value,
                DotStarTag,
                std::conditional_t<
                    IsGetDotStarInvocable<C, void, Ts...>::value,
                    GetDotStarTag,
                    std::conditional_t<
                        IsStarDotStarInvocable<C, void, Ts...>::value,
                        StarDotStarTag,
                        std::conditional_t<
                            IsParenInvocable<C, void, Ts...>::value,
                            ParenTag,
                            NotInvocableTag
                        >
                    >
                >
            >
        >
    >
>;

template <typename C, typename ...Ts>
struct IsInvocable : std::integral_constant<bool, !std::is_same<
    NotInvocableTag,
    InvokeTag<C, Ts...>
>::value> { };

template <typename T, typename C, typename ...Ts>
struct InvokeTraits { };

template <typename C, typename T, typename ...Ts>
struct InvokeTraits<DotStarParenTag, C, T, Ts...> {
    using Tag = DotStarParenTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        (std::declval<T>().*std::declval<C>())(std::declval<Ts>()...)
    )>;
    using Result = decltype((
        (std::declval<T>().*std::declval<C>())(std::declval<Ts>()...)
    ));
};

template <typename C, typename T, typename ...Ts>
struct InvokeTraits<GetDotStarParenTag, C, T, Ts...> {
    using Tag = DotStarParenTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        (std::declval<T>().get().*std::declval<C>())(std::declval<Ts>()...)
    )>;
    using Result = decltype((
        (std::declval<T>().get().*std::declval<C>())(std::declval<Ts>()...)
    ));
};

template <typename C, typename T, typename ...Ts>
struct InvokeTraits<StarDotStarParenTag, C, T, Ts...> {
    using Tag = StarDotStarParenTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        ((*std::declval<T>()).*std::declval<C>())(std::declval<Ts>()...)
    )>;
    using Result = decltype((
        ((*std::declval<T>()).*std::declval<C>())(std::declval<Ts>()...)
    ));
};

template <typename C, typename T>
struct InvokeTraits<DotStarTag, C, T> {
    using Tag = DotStarTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        std::declval<T>().*std::declval<C>()
    )>;
    using Result = decltype((
        std::declval<T>().*std::declval<C>()
    ));
};

template <typename C, typename T>
struct InvokeTraits<GetDotStarTag, C, T> {
    using Tag = GetDotStarTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        std::declval<T>().get().*std::declval<C>()
    )>;
    using Result = decltype((
        std::declval<T>().get().*std::declval<C>()
    ));
};

template <typename C, typename T>
struct InvokeTraits<StarDotStarTag, C, T> {
    using Tag = StarDotStarTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        (*std::declval<T>()).*std::declval<C>()
    )>;
    using Result = decltype((
        (*std::declval<T>()).*std::declval<C>()
    ));
};

template <typename C, typename ...Ts>
struct InvokeTraits<ParenTag, C, Ts...> {
    using Tag = ParenTag;
    using IsNothrow = std::integral_constant<bool, noexcept(
        std::declval<C>()(std::declval<Ts>()...)
    )>;
    using Result = decltype((
        std::declval<C>()(std::declval<Ts>()...)
    ));
};

template <typename C, typename = void, typename ...Ts>
struct InvokeResult { };

template <typename C, typename ...Ts>
struct InvokeResult<C, VoidT<std::enable_if_t<
    IsInvocable<C, Ts...>::value
>>, Ts...> {
    using type = typename InvokeTraits<InvokeTag<C, Ts...>, C, Ts...>::Result;
};

template <typename C, typename = void, typename ...Ts>
struct IsNothrowInvocable : std::false_type { };

template <typename C, typename ...Ts>
struct IsNothrowInvocable<C, VoidT<
    std::enable_if_t<IsInvocable<C, Ts...>::value>,
    std::enable_if_t<InvokeTraits<C, Ts...>::IsNothrow::value>
>, Ts...> : std::true_type { };

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
    ((*std::forward<T>(t)).*std::forward<C>(c))(std::forward<Ts>(ts)...)
)) {
    return ((*std::forward<T>(t)).*std::forward<C>(c))(std::forward<Ts>(ts)...);
}

template <typename C, typename T>
constexpr decltype(auto) do_invoke(DotStarTag, C &&c, T &&t)
noexcept(noexcept(std::forward<T>(t).*std::forward<C>(c))) {
    return std::forward<T>(t).*std::forward<C>(c);
}

template <typename C, typename T>
constexpr decltype(auto) do_invoke(GetDotStarTag, C &&c, T &&t)
noexcept(noexcept(std::forward<T>(t).get().*std::forward<C>(c))) {
    return std::forward<T>(t).get().*std::forward<C>(c);
}

template <typename C, typename T>
constexpr decltype(auto) do_invoke(StarDotStarTag, C &&c, T &&t)
noexcept(noexcept((*std::forward<T>(t)).*std::forward<C>(c))) {
    return (*std::forward<T>(t)).*std::forward<C>(c);
}

template <typename C, typename ...Ts>
constexpr decltype(auto) do_invoke(ParenTag, C &&c, Ts &&...ts)
noexcept(noexcept(std::forward<C>(c)(std::forward<Ts>(ts)...))) {
    return std::forward<C>(c)(std::forward<Ts>(ts)...);
}

template <typename C, typename ...Ts>
constexpr void do_invoke(NotInvocableTag, C&&, Ts &&...) {
    static_assert(!std::is_same<C, C>::value, "not invocable");
}

} // namespace umigv_ranges_detail_invoke

#endif
