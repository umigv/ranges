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

#include "../apply.hpp"
#include "../invoke.hpp"
#include "../traits.hpp"

#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {
namespace detail {

template <typename I, typename F, typename = void>
struct is_invoke_mappable : std::false_type { };

template <typename I, typename F>
struct is_invoke_mappable<I, F, void_t<
    iterator_reference_t<I>,
    std::enable_if_t<
        ::umigv::ranges::is_invocable<const F&, iterator_reference_t<I>>::value
    >
>> : std::true_type { };

template <typename I, typename F, typename = void>
struct is_apply_mappable : std::false_type { };

template <typename I, typename F>
struct is_apply_mappable<I, F, void_t<
    iterator_reference_t<I>,
    std::enable_if_t<
        ::umigv::ranges::is_applicable<const F&, iterator_reference_t<I>>::value
    >
>> : std::true_type { };

template <typename I, typename F>
struct is_mappable
: disjunction<is_invoke_mappable<I, F>, is_apply_mappable<I, F>> { };

template <typename I, typename F, bool = is_invoke_mappable<I, F>::value>
struct map_result {
    using type =
        ::umigv::ranges::invoke_result_t<const F&, iterator_reference_t<I>>;
};

template <typename I, typename F>
struct map_result<I, F, false> {
    using type =
        ::umigv::ranges::apply_result_t<const F&, iterator_reference_t<I>>;
};

template <typename I, typename F>
using map_result_t = typename map_result<I, F>::type;

template <typename I, typename F,
          std::enable_if_t<is_invoke_mappable<I, F>::value, int> = 0>
constexpr decltype(auto) do_map(const I &current, const F &f) {
    return ::umigv::ranges::invoke(f, *current);
}

template <typename I, typename F,
          std::enable_if_t<!is_invoke_mappable<I, F>::value
                           && is_apply_mappable<I, F>::value, int> = 0>
constexpr decltype(auto) do_map(const I &current, const F &f) {
    return ::umigv::ranges::apply(f, *current);
}

} // namespace detail
} // namespace ranges
} // namesapce umigv
