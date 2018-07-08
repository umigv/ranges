#ifndef UMIGV_RANGES_ITERATOR_RANGE_HPP
#define UMIGV_RANGES_ITERATOR_RANGE_HPP

#include "range_fwd.hpp"
#include "traits.hpp"

#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, std::enable_if_t<is_iterator<I>::value, int> = 0>
class IteratorRange;

template <typename I, std::enable_if_t<is_iterator<I>::value, int>>
class IteratorRange : public Range<IteratorRange<I>> {
public:
    using difference_type =
        typename RangeTraits<IteratorRange>::difference_type;
    using iterator = typename RangeTraits<IteratorRange>::iterator;
    using pointer = typename RangeTraits<IteratorRange>::pointer;
    using reference = typename RangeTraits<IteratorRange>::reference;
    using value_type = typename RangeTraits<IteratorRange>::value_type;

    constexpr IteratorRange(I first, I last)
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
IteratorRange<I> iterator_range(I first, I last) {
    return { first, last };
}

template <typename R>
IteratorRange<begin_result_t<R>> iterator_range(R &&r) {
    using std::begin;
    using std::end;

    return { begin(std::forward<R>(r)), end(std::forward<R>(r)) };
}

template <typename I>
struct RangeTraits<IteratorRange<I>> {
    using difference_type = iterator_difference_t<I>;
    using iterator = I;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;
};

} // namespace ranges
} // namespace umigv

#endif
