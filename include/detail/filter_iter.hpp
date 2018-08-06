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

#ifndef UMIGV_RANGES_DETAIL_FILTER_ITER_HPP
#define UMIGV_RANGES_DETAIL_FILTER_ITER_HPP

#include "../invoke.hpp"
#include "../apply.hpp"

#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {
namespace detail {

template <typename P, typename T,
          std::enable_if_t<IS_INVOCABLE<P&&, T&&>, int> = 0>
constexpr bool do_filter(P &&predicate, T &&t)
noexcept(IS_NOTHROW_INVOCABLE<P&&, T&&>) {
    return static_cast<bool>(invoke(std::forward<P>(predicate),
                                    std::forward<T>(t)));
}

template <typename P, typename T,
          std::enable_if_t<!IS_INVOCABLE<P&&, T&&>
                           && IS_APPLICABLE<P&&, T&&>, int> = 0>
constexpr bool do_filter(P &&predicate, T &&t)
noexcept(IS_NOTHROW_APPLICABLE<P&&, T&&>) {
    return static_cast<bool>(apply(std::forward<P>(predicate),
                                   std::forward<T>(t)));
}

template <typename P, typename T,
          bool = IS_INVOCABLE<P, T>, bool = IS_APPLICABLE<P, T>>
struct filter_result { };

template <typename P, typename T, bool B>
struct filter_result<P, T, true, B> {
    using type = InvokeResultT<P, T>;
};

template <typename P, typename T>
struct filter_result<P, T, false, true> {
    using type = ApplyResultT<P, T>;
};

template <typename P, typename T>
using filter_result_t = typename filter_result<P, T>::type;

} // namespace detail
} // namespace ranges
} // namespace umigv

#endif
