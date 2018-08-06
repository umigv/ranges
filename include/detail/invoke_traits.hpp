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

namespace umigv_ranges_invoke_detail {

struct member_function_ref_tag { };

struct member_function_ptr_tag { };

struct member_data_ref_tag { };

struct member_data_ptr_tag { };

struct functor_tag { };

template <typename T, typename, typename ...Args>
struct invoke_traits { };

template <typename T, typename U, typename V, typename ...Args>
struct invoke_traits<
    T U::*,
    umigv::ranges::VoidT<decltype(
        (std::declval<V>().*std::declval<T U::*>())(std::declval<Args>()...)
    ), std::enable_if_t<std::is_member_function_pointer<T U::*>::value>,
    std::enable_if_t<std::is_base_of<U, std::decay_t<V>>::value>>,
    V, Args...
> {
    using type = member_function_ref_tag;
    using result = decltype((
        (std::declval<V>().*std::declval<T U::*>())(std::declval<Args>()...)
    ));

    static constexpr bool is_nothrow = noexcept(
        (std::declval<V>().*std::declval<T U::*>())(std::declval<Args>()...)
    );
};

template <typename T, typename U, typename ...Args>
struct invoke_traits<
    T,
    umigv::ranges::VoidT<decltype(
        ((*std::declval<U>()).*std::declval<T>())(
            std::declval<Args>()...
        )
    ), std::enable_if_t<std::is_member_function_pointer<T>::value>>,
    U, Args...
> {
    using type = member_function_ptr_tag;
    using result = decltype((
        ((*std::declval<U>()).*std::declval<T>())(
            std::declval<Args>()...
        )
    ));

    static constexpr bool is_nothrow = noexcept(
        ((*std::declval<U>()).*std::declval<T>())(
            std::declval<Args>()...
        )
    );
};

template <typename T, typename U, typename V>
struct invoke_traits<
    T U::*,
    umigv::ranges::VoidT<decltype(
        std::declval<V>().*std::declval<T U::*>()
    ), std::enable_if_t<std::is_member_object_pointer<T U::*>::value>,
    std::enable_if_t<std::is_base_of<U, std::decay_t<V>>::value>>,
    V
> {
    using type = member_data_ref_tag;
    using result = decltype((
        std::declval<V>().*std::declval<T U::*>()
    ));

    static constexpr bool is_nothrow = noexcept(
        std::declval<V>().*std::declval<T U::*>()
    );
};

template <typename T, typename U>
struct invoke_traits<
    T,
    umigv::ranges::VoidT<decltype(
        (*std::declval<U>()).*std::declval<T>()
    ), std::enable_if_t<std::is_member_object_pointer<T>::value>>,
    U
> {
    using type = member_data_ptr_tag;
    using result = decltype((
        (*std::declval<U>()).*std::declval<T>()
    ));

    static constexpr bool is_nothrow = noexcept(
        (*std::declval<U>()).*std::declval<T>()
    );
};

template <typename T, typename ...Args>
struct invoke_traits<
    T,
    umigv::ranges::VoidT<decltype(
        std::declval<T>()(std::declval<Args>()...)
    )>,
    Args...
> {
    using type = functor_tag;
    using result = decltype((
        std::declval<T>()(std::declval<Args>()...)
    ));

    static constexpr bool is_nothrow = noexcept(
        std::declval<T>()(std::declval<Args>()...)
    );
};

} // namespace umigv_ranges_invoke_detail

#endif
