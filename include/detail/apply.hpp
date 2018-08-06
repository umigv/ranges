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

#ifndef UMIGV_RANGES_DETAIL_APPLY_HPP
#define UMIGV_RANGES_DETAIL_APPLY_HPP

#include "../invoke.hpp"
#include "../tuple_traits.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv_ranges_apply_detail {

template <typename C, typename T, typename = void, std::size_t ...Is>
struct is_applicable : std::false_type { };

template <typename C, typename T, std::size_t ...Is>
struct is_applicable<
    C,
    T,
    umigv::ranges::VoidT<std::enable_if_t<
        umigv::ranges::IS_INVOCABLE<
            C&&, umigv::ranges::GetResultT<Is, T>...
        >
    >>,
    Is...
> : std::true_type { };

template <typename C, typename T, std::size_t ...Is>
constexpr is_applicable<C, T, void, Is...>
check_applicable(C&&, T&&, std::index_sequence<Is...>);

template <typename C, typename T, typename = void, std::size_t ...Is>
struct is_nothrow_applicable : std::false_type { };

template <typename C, typename T, std::size_t ...Is>
struct is_nothrow_applicable<
    C,
    T,
    umigv::ranges::VoidT<std::enable_if_t<
        umigv::ranges::IS_NOTHROW_INVOCABLE<
            C&&, umigv::ranges::GetResultT<Is, T>...
        >
    >>,
    Is...
> : std::true_type { };

template <typename C, typename T, std::size_t ...Is>
constexpr is_nothrow_applicable<C, T, void, Is...>
check_nothrow_applicable(C&&, T&&, std::index_sequence<Is...>);

template <typename C, typename T, typename = void, std::size_t ...Is>
struct apply_result { };

template <typename C, typename T, std::size_t ...Is>
struct apply_result<
    C,
    T,
    umigv::ranges::VoidT<
        std::enable_if_t<is_applicable<C&&, T&&, void, Is...>::value>
    >,
    Is...
> {
    using type = umigv::ranges::InvokeResultT<
        C&&,
        umigv::ranges::GetResultT<Is, T>...
    >;
};

template <typename C, typename T, std::size_t ...Is>
constexpr apply_result<C&&, T&&, void, Is...>
check_apply_result(C&&, T&&, std::index_sequence<Is...>) noexcept;

template <typename C, typename T, std::size_t ...Is>
constexpr umigv::ranges::InvokeResultT<
    C&&,
    umigv::ranges::GetResultT<Is, T&&>...
>
apply(C &&callable, T &&tuple, std::index_sequence<Is...>)
noexcept(umigv::ranges::IS_NOTHROW_INVOCABLE<
    C&&,
    umigv::ranges::GetResultT<Is, T&&>...
>) {
    return umigv::ranges::invoke(std::forward<C>(callable),
                                 std::get<Is>(std::forward<T>(tuple))...);
}

} // namespace umigv_ranges_apply_detail

#endif
