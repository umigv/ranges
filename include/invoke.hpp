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

#ifndef UMIGV_RANGES_INVOKE_HPP
#define UMIGV_RANGES_INVOKE_HPP

#include "traits.hpp"
#include "detail/invoke.hpp"

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename T>
struct UnwrapRefResult {
    using type = T;
};

template <typename T>
struct UnwrapRefResult<std::reference_wrapper<T>> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<const std::reference_wrapper<T>> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<volatile std::reference_wrapper<T>> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<const volatile std::reference_wrapper<T>> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<std::reference_wrapper<T>&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<const std::reference_wrapper<T>&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<volatile std::reference_wrapper<T>&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<const volatile std::reference_wrapper<T>&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<std::reference_wrapper<T>&&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<const std::reference_wrapper<T>&&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<volatile std::reference_wrapper<T>&&> {
    using type = T&;
};

template <typename T>
struct UnwrapRefResult<const volatile std::reference_wrapper<T>&&> {
    using type = T&;
};

template <typename T>
using UnwrapRefResultT = typename UnwrapRefResult<T>::type;

template <typename T>
constexpr T&& unwrap_ref(T &&t) noexcept {
    return std::forward<T>(t);
}

template <typename T>
constexpr T& unwrap_ref(std::reference_wrapper<T> t) noexcept {
    return t.get();
}

template <typename T, typename ...As>
using InvokeResult = umigv_ranges_invoke_detail::invoke_result<
    UnwrapPtrRefT<T>,
    void,
    UnwrapRefResultT<As>...
>;

template <typename T, typename ...As>
using InvokeResultT = typename InvokeResult<T, As...>::type;

template <typename T, typename ...As>
using IsInvocable = umigv_ranges_invoke_detail::is_invocable<
    UnwrapPtrRefT<T>,
    void,
    UnwrapRefResultT<As>...
>;

template <typename T, typename ...As>
constexpr bool IS_INVOCABLE = IsInvocable<T, As...>::value;

template <typename T, typename ...As>
using IsNothrowInvocable = umigv_ranges_invoke_detail::is_nothrow_invocable<
    UnwrapPtrRefT<T>,
    void,
    UnwrapRefResultT<As>...
>;


template <typename T, typename ...As>
constexpr bool IS_NOTHROW_INVOCABLE = IsNothrowInvocable<T, As...>::value;

template <typename C, typename ...As,
          std::enable_if_t<IS_INVOCABLE<C, As...>, int> = 0>
constexpr InvokeResultT<C, UnwrapRefResult<As>...> invoke(C &&c, As &&...args)
noexcept(IS_NOTHROW_INVOCABLE<C, As...>) {
    using TraitsT = typename umigv_ranges_invoke_detail::invoke_traits<
        UnwrapPtrRefT<C>,
        void,
        UnwrapRefResult<As>...
    >;
    using TypeT = typename TraitsT::type;

    return umigv_ranges_invoke_detail::invoke(
        TypeT{ },
        std::forward<C>(c),
        unwrap_ref(std::forward<As>(args))...
    );
}

} // namespace ranges
} // namespace umigv

#endif
