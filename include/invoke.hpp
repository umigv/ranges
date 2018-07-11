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
using invoke_result = detail::invoke_result<T, void, As...>;

template <typename T, typename ...As>
using invoke_result_t = typename invoke_result<T, As...>::type;

template <typename T, typename ...As>
using is_invocable = detail::is_invocable<T, void, As...>;

template <typename T, typename ...As>
using is_nothrow_invocable = detail::is_nothrow_invocable<T, void, As...>;

template <typename T>
constexpr decltype(auto) unwrap(T &&t) noexcept {
    return std::forward<T>(t);
}

template <typename T>
constexpr T& unwrap(std::reference_wrapper<T> t) noexcept {
    return t.get();
}

template <typename C, typename ...As,
          std::enable_if_t<is_invocable<C, As...>::value, int> = 0>
constexpr invoke_result_t<C, As...> invoke(C &&c, As &&...args)
noexcept(is_nothrow_invocable<C, As...>::value) {
    using TraitsT = typename detail::invoke_traits<C, void, As...>;
    using TypeT = typename TraitsT::type;

    return detail::invoke(TypeT{ }, std::forward<C>(c),
                          unwrap(std::forward<As>(args))...);
}

} // namespace ranges
} // namespace umigv

#endif
