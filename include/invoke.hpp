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
struct unwrap_result {
    using type = T;
};

template <typename T>
struct unwrap_result<std::reference_wrapper<T>> {
    using type = T&;
};

template <typename T>
using unwrap_result_t = typename unwrap_result<T>::type;

template <typename T>
constexpr unwrap_result_t<T> unwrap(T &&t) noexcept {
    return std::forward<T>(t);
}

template <typename T>
constexpr unwrap_result_t<T> unwrap(std::reference_wrapper<T> t) noexcept {
    return t.get();
}

template <typename T, typename ...As>
using invoke_result =
    detail::invoke_result<decompose_t<T>, void, unwrap_result_t<As>...>;

template <typename T, typename ...As>
using invoke_result_t = typename invoke_result<T, As...>::type;

template <typename T, typename ...As>
using is_invocable =
    detail::is_invocable<decompose_t<T>, void, unwrap_result_t<As>...>;

template <typename T, typename ...As>
using is_nothrow_invocable =
    detail::is_nothrow_invocable<decompose_t<T>, void, unwrap_result_t<As>...>;

template <typename C, typename ...As,
          std::enable_if_t<is_invocable<C, As...>::value, int> = 0>
constexpr invoke_result_t<C, unwrap_result_t<As>...> invoke(C &&c, As &&...args)
noexcept(is_nothrow_invocable<C, As...>::value) {
    using TraitsT = typename detail::invoke_traits<
        decompose_t<C>,
        void,
        unwrap_result_t<As>...
    >;
    using TypeT = typename TraitsT::type;

    return detail::invoke(TypeT{ }, std::forward<C>(c),
                          unwrap(std::forward<As>(args))...);
}

} // namespace ranges
} // namespace umigv

#endif
