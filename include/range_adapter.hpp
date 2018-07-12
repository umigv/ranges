#ifndef UMIGV_RANGES_RANGE_ADAPTER_HPP
#define UMIGV_RANGES_RANGE_ADAPTER_HPP

#include "range_fwd.hpp"
#include "traits.hpp"

#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, std::enable_if_t<is_iterator<I>::value, int> = 0>
class RangeAdapter : public Range<RangeAdapter<I>> {
public:
    using difference_type =
        typename RangeTraits<RangeAdapter>::difference_type;
    using iterator = typename RangeTraits<RangeAdapter>::iterator;
    using pointer = typename RangeTraits<RangeAdapter>::pointer;
    using reference = typename RangeTraits<RangeAdapter>::reference;
    using value_type = typename RangeTraits<RangeAdapter>::value_type;

    constexpr RangeAdapter(I first, I last)
    : first_{ first }, last_{ last } { }

    constexpr iterator begin() const noexcept {
        return first_;
    }

    constexpr iterator end() const noexcept {
        return last_;
    }

private:
    I first_;
    I last_;
};

template <typename I>
constexpr RangeAdapter<I> adapt(I first, I last)
noexcept {
    return { first, last };
}

template <typename R>
constexpr RangeAdapter<begin_result_t<R>> adapt(R &&r) noexcept {
    using std::begin;
    using std::end;

    return { begin(std::forward<R>(r)), end(std::forward<R>(r)) };
}

template <typename T>
constexpr RangeAdapter<begin_result_t<std::initializer_list<T>>>
adapt(std::initializer_list<T> list) noexcept {
    using std::begin;
    using std::end;

    return { begin(list), end(list) };
}

template <typename I>
struct RangeTraits<RangeAdapter<I>> {
    using difference_type = iterator_difference_t<I>;
    using iterator = I;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;
};

} // namespace ranges
} // namespace umigv

#endif
