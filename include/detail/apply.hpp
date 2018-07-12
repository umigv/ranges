#ifndef UMIGV_RANGES_DETAIL_APPLY_HPP
#define UMIGV_RANGES_DETAIL_APPLY_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../invoke.hpp"

namespace umigv {
namespace ranges {
namespace detail {

template <typename C, typename T, typename = void, std::size_t ...Is>
struct is_applicable : std::false_type { };

template <typename C, typename T, std::size_t ...Is>
struct is_applicable<
    C,
    T,
    void_t<std::enable_if_t<
        ::umigv::ranges::is_invocable<
            C&&, tuple_element_t<Is, T>...
        >::value
    >>,
    Is...
> : std::true_type { };

template <typename C, typename T, std::size_t ...Is>
constexpr is_applicable<C, T, void, Is...>
check_applicable(C, T, std::index_sequence<Is...>);

template <typename C, typename T, typename = void, std::size_t ...Is>
struct is_nothrow_applicable : std::false_type { };

template <typename C, typename T, std::size_t ...Is>
struct is_nothrow_applicable<
    C,
    T,
    void_t<std::enable_if_t<
        ::umigv::ranges::is_nothrow_invocable<
            C&&, tuple_element_t<Is, T>...
        >::value
    >>,
    Is...
> : std::true_type { };

template <typename C, typename T, std::size_t ...Is>
constexpr is_nothrow_applicable<C, T, void, Is...>
check_nothrow_applicable(C, T, std::index_sequence<Is...>);

template <typename C, typename T, typename = void, std::size_t ...Is>
struct apply_result { };

template <typename C, typename T, std::size_t ...Is>
struct apply_result<
    C,
    T,
    void_t<std::enable_if_t<is_applicable<C, T, void, Is...>::value>>, Is...
> {
    using type = invoke_result_t<C&&, tuple_element_t<Is, T>...>;
};

template <typename C, typename T, std::size_t ...Is>
constexpr apply_result<C, T, void, Is...>
check_apply_result(C, T, std::index_sequence<Is...>) noexcept;

template <typename C, typename T, std::size_t ...Is>
constexpr invoke_result_t<C&&, tuple_element_t<Is, T>...>
apply(C &&callable, T &&tuple, std::index_sequence<Is...>)
noexcept(is_nothrow_invocable<C&&, tuple_element_t<Is, T>...>::value) {
    return ::umigv::ranges::invoke(std::forward<C>(callable),
                                   std::get<Is>(std::forward<T>(tuple))...);
}

} // namespace detail
} // namespace ranges
} // namespace umigv

#endif
