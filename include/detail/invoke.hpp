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
#include "detail/invoke_traits.hpp"

#include <type_traits>

namespace umigv_ranges_invoke_detail {

template <typename T, typename, typename ...As>
struct invoke_result { };

template <typename T, typename ...As>
struct invoke_result<
    T,
    umigv::ranges::VoidT<typename invoke_traits<T, void, As...>::result>,
    As...
> {
    using type = typename invoke_traits<T, void, As...>::result;
};

template <typename T, typename, typename ...As>
struct is_invocable : std::false_type { };

template <typename T, typename ...As>
struct is_invocable<
    T,
    umigv::ranges::VoidT<typename invoke_traits<T, void, As...>::result>,
    As...
> : std::true_type { };

template <typename T, typename, typename ...As>
struct is_nothrow_invocable : std::false_type { };

template <typename T, typename ...As>
struct is_nothrow_invocable<
    T, umigv::ranges::VoidT<
        typename invoke_traits<T, void, As...>::result,
        std::enable_if_t<invoke_traits<T, void, As...>::is_nothrow>
    >, As...
> : std::true_type { };

template <typename T, typename U, typename ...As>
constexpr decltype(auto) invoke(member_function_ref_tag, T &&t, U &&u,
                                As &&...args)
noexcept(is_nothrow_invocable<T, void, U, As...>::value) {
    return (std::forward<U>(u).*std::forward<T>(t))(std::forward<As>(args)...);
}

template <typename T, typename U, typename ...As>
constexpr decltype(auto) invoke(member_function_ptr_tag, T &&t, U &&u,
                                As &&...args)
noexcept(is_nothrow_invocable<T, void, U, As...>::value) {
    return ((*std::forward<U>(u)).*std::forward<T>(t))(
        std::forward<As>(args)...
    );
}

template <typename T, typename U>
constexpr decltype(auto) invoke(member_data_ref_tag, T &&t, U &&u)
noexcept(is_nothrow_invocable<T, void, U>::value) {
    return std::forward<U>(u).*std::forward<T>(t);
}

template <typename T, typename U>
constexpr decltype(auto) invoke(member_data_ptr_tag, T &&t, U &&u)
noexcept(is_nothrow_invocable<T, void, U>::value) {
    return (*std::forward<U>(u)).*std::forward<T>(t);
}

template <typename T, typename ...As>
constexpr decltype(auto) invoke(functor_tag, T &&t, As &&...args)
noexcept(is_nothrow_invocable<T, void, As...>::value) {
    return std::forward<T>(t)(std::forward<As>(args)...);
}

} // namespace umigv_ranges_invoke_detail

#endif
