#ifndef UMIGV_RANGES_ZIPPED_RANGE_HPP
#define UMIGV_RANGES_ZIPPED_RANGE_HPP

#include "range_fwd.hpp"
#include "traits.hpp"

#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename T, std::size_t ...Is>
class ZippedRange;

template <typename T, std::size_t ...Is>
class ZippedRangeIterator {
public:
    friend ZippedRange<T, Is...>;

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;
    using pointer = void;
    using reference =
        std::tuple<iterator_reference_t<std::tuple_element_t<Is, T>>...>;
    using value_type =
        std::tuple<iterator_value_t<std::tuple_element_t<Is, T>>...>;

    constexpr reference operator*() const {
        return { (*std::get<Is>(currents_))... };
    }

    constexpr ZippedRangeIterator& operator++() {
        using ExpanderT = int[];
        (void) ExpanderT{ 0, (++std::get<Is>(currents_), 0)... };

        return *this;
    }

    constexpr ZippedRangeIterator operator++(int) {
        const auto to_return = *this;

        ++*this;

        return to_return;
    }

    constexpr friend bool operator==(const ZippedRangeIterator &lhs,
                                     const ZippedRangeIterator &rhs) {
        return variadic_or(
            (std::get<Is>(lhs.currents_) == std::get<Is>(rhs.currents_))...
        );
    }

    constexpr friend bool operator!=(const ZippedRangeIterator &lhs,
                                     const ZippedRangeIterator &rhs) {
        return !(lhs == rhs);
    }

private:
    constexpr ZippedRangeIterator(const T &currents, const T &ends)
    noexcept(std::is_nothrow_copy_constructible<T>::value)
    : currents_{ currents }, ends_{ ends } { }

    constexpr static bool variadic_or() {
        return false;
    }

    template <typename ...Ts>
    constexpr static bool variadic_or(bool value, Ts &&...rest) noexcept {
        if (value) {
            return true;
        }

        return variadic_or(std::forward<Ts>(rest)...);
    }

    T currents_;
    T ends_;
};

template <typename T, std::size_t ...Is>
class ZippedRange : public Range<ZippedRange<T, Is...>> {
public:
    // static_assert(is_tuple<T>::value, "T must be a tuple of iterators");
    // static_assert(
    //     conjunction<is_input_iterator<tuple_element_t<Is, T>>...>::value,
    //     "T must be a tuple of iterators"
    // );
    // static_assert(tuple_size<T>::value == sizeof...(Is),
    //               "Is must be a sequence of indices corresponding to T's size");

    using iterator = ZippedRangeIterator<T, Is...>;
    using difference_type = typename iterator::difference_type;
    using pointer = typename iterator::pointer;
    using reference = typename iterator::reference;
    using value_type = typename iterator::value_type;

    constexpr ZippedRange(const T &begins, const T &ends)
    noexcept(std::is_nothrow_copy_constructible<T>::value)
    : begins_{ begins }, ends_{ ends } { }

    constexpr iterator begin() const
    noexcept(std::is_nothrow_copy_constructible<T>::value) {
        return { begins_, ends_ };
    }

    constexpr iterator end() const
    noexcept(std::is_nothrow_copy_constructible<T>::value) {
        return { ends_, ends_ };
    }

private:
    T begins_;
    T ends_;
};

template <typename T, std::size_t ...Is>
struct RangeTraits<ZippedRange<T, Is...>> {
    using iterator = ZippedRangeIterator<T, Is...>;
    using difference_type = typename iterator::difference_type;
    using pointer = typename iterator::pointer;
    using reference = typename iterator::reference;
    using value_type = typename iterator::value_type;
};

namespace detail {

template <typename T, std::size_t ...Is>
constexpr ZippedRange<remove_cvref_t<T>, Is...> zip(
    T &&begins, T &&ends, std::index_sequence<Is...>
) noexcept(std::is_nothrow_copy_constructible<T>::value) {
    return { std::forward<T>(begins), std::forward<T>(ends) };
}

} // namespace detail

template <typename ...Rs>
decltype(auto) zip(Rs &&...ranges)
noexcept(conjunction<
    std::is_nothrow_copy_constructible<begin_result_t<Rs>>...
>::value) {
    using std::begin;
    using std::end;

    using TupleT = std::tuple<begin_result_t<Rs>...>;

    return detail::zip(TupleT{ begin(std::forward<Rs>(ranges))... },
                       TupleT{ end(std::forward<Rs>(ranges))... },
                       std::index_sequence_for<Rs...>{ });
}

} // namespace ranges
} // namespace umigv

#endif
