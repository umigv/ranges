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
