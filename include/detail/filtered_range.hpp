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

template <typename I, typename P, typename = void>
struct is_invoke_filterable : std::false_type { };

template <typename I, typename P>
struct is_invoke_filterable<I, P, void_t<std::enable_if_t<
    ::umigv::ranges::is_invocable<const P&, iterator_reference_t<I>>::value
    && std::is_convertible<
        ::umigv::ranges::invoke_result_t<const P&, iterator_reference_t<I>>,
        bool
    >::value
>>> : std::true_type { };

template <typename I, typename P, typename = void>
struct is_apply_filterable : std::false_type { };

template <typename I, typename P>
struct is_apply_filterable<I, P, void_t<std::enable_if_t<
    ::umigv::ranges::is_applicable<const P&, iterator_reference_t<I>>::value
    && std::is_convertible<
        ::umigv::ranges::apply_result_t<const P&, iterator_reference_t<I>>, bool
    >::value
>>> : std::true_type { };

template <typename I, typename P>
struct is_filterable
: disjunction<is_invoke_filterable<I, P>, is_apply_filterable<I, P>> { };

template <typename I, typename P,
          std::enable_if_t<is_invoke_filterable<I, P>::value, int> = 0>
constexpr void advance(I &current, I &last, const P &predicate) {
    while (!(current == last)
           && !::umigv::ranges::invoke(predicate, *current)) {
        ++current;
    }
}

template <typename I, typename P,
          std::enable_if_t<!is_invoke_filterable<I, P>::value
                           && is_apply_filterable<I, P>::value, int> = 0>
constexpr void advance(I &current, I &last, const P &predicate) {
    while (!(current == last) && !::umigv::ranges::apply(predicate, *current)) {
        ++current;
    }
}

} // namespace detail
} // namespace ranges
} // namesapce umigv
