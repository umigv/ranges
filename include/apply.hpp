#ifndef UMIGV_RANGES_APPLY_HPP
#define UMIGV_RANGES_APPLY_HPP

#include "detail/apply.hpp"
#include "invoke.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename C, typename T, typename = void>
struct is_applicable : std::false_type { };

template <typename C, typename T>
struct is_applicable<C, T, void_t<std::enable_if_t<is_tuple<T>::value>>>
: decltype(detail::check_applicable(
    std::declval<C>(),
    std::declval<T>(),
    std::make_index_sequence<tuple_size<T>::value>()
)) { };

template <typename C, typename T, typename = void>
struct is_nothrow_applicable : std::false_type { };

template <typename C, typename T>
struct is_nothrow_applicable<C, T, void_t<std::enable_if_t<is_tuple<T>::value>>>
: decltype(detail::check_nothrow_applicable(
    std::declval<C>(),
    std::declval<T>(),
    std::make_index_sequence<tuple_size<T>::value>()
)) { };

template <typename C, typename T, typename = void>
struct apply_result { };

template <typename C, typename T>
struct apply_result<C, T, void_t<std::enable_if_t<is_tuple<T>::value>>> {
    using type = typename decltype(detail::check_apply_result(
        std::declval<C>(),
        std::declval<T>(),
        std::make_index_sequence<tuple_size<T>::value>()
    ))::type;
};

template <typename C, typename T>
using apply_result_t = typename apply_result<C, T>::type;

template <typename C, typename T,
          std::enable_if_t<
              is_tuple<T>::value && is_applicable<C, T>::value, int
          > = 0>
constexpr apply_result_t<C, T> apply(C &&callable, T &&tuple)
noexcept(is_nothrow_applicable<C, T>::value) {
    return detail::apply(std::forward<C>(callable), std::forward<T>(tuple),
                         std::make_index_sequence<tuple_size<T>::value>());
}

} // namespace ranges
} // namespace umigv

#endif
