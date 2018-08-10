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

template <typename T, typename ...As>
using InvokeResult = umigv_ranges_detail_invoke::InvokeResult<T, void, As...>;

template <typename T, typename ...As>
using InvokeResultT = typename InvokeResult<T, As...>::type;

template <typename T, typename ...As>
using IsInvocable = umigv_ranges_detail_invoke::IsInvocable<T, As...>;

template <typename T, typename ...As>
constexpr bool IS_INVOCABLE = IsInvocable<T, As...>::value;

template <typename T, typename ...As>
using IsNothrowInvocable =
    umigv_ranges_detail_invoke::IsNothrowInvocable<T, void, As...>;

template <typename T, typename ...As>
constexpr bool IS_NOTHROW_INVOCABLE = IsNothrowInvocable<T, As...>::value;

template <typename C, typename ...As,
          std::enable_if_t<IS_INVOCABLE<C, As...>, int> = 0>
constexpr InvokeResultT<C, As...> invoke(C &&c, As &&...args)
noexcept(IS_NOTHROW_INVOCABLE<C, As...>) {
    using Tag = typename umigv_ranges_detail_invoke::InvokeTag<C, As...>;

    return umigv_ranges_detail_invoke::do_invoke(
        Tag{ },
        std::forward<C>(c),
        std::forward<As>(args)...
    );
}

} // namespace ranges
} // namespace umigv

#endif
