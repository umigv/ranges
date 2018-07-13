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
