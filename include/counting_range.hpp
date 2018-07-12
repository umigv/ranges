#ifndef UMIGV_RANGES_COUNTING_RANGE_HPP
#define UMIGV_RANGES_COUNTING_RANGE_HPP

#include "range_fwd.hpp"
#include "traits.hpp"

#include <cstddef>
#include <stdexcept>

namespace umigv {
namespace ranges {

template <typename T>
class CountingRange;

template <typename T>
class CountingRangeIterator {
public:
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;
    using pointer = const T*;
    using reference = const T&;
    using value_type = T;

    friend CountingRange<T>;

    constexpr reference operator*() const {
        range_check("CountingRangeIterator::operator*");

        return current_;
    }

    constexpr pointer operator->() const noexcept {
        return std::addressof(**this);
    }

    constexpr CountingRangeIterator& operator++() {
        range_check("CountingRangeIterator::operator++");

        current_ += step_;

        return *this;
    }

    constexpr CountingRangeIterator operator++(int) {
        const auto to_return = *this;

        ++(*this);

        return to_return;
    }

    friend constexpr bool operator==(const CountingRangeIterator &lhs,
                                     const CountingRangeIterator &rhs) {
        if (lhs.step_ != rhs.step_ || lhs.end_ != rhs.end_) {
            throw std::out_of_range{ "CountingRangeIterator::operator==" };
        }

        return lhs.current_ == rhs.current_;
    }

    friend constexpr bool operator!=(const CountingRangeIterator &lhs,
                                     const CountingRangeIterator &rhs) {
        return !(lhs == rhs);
    }

private:
    constexpr CountingRangeIterator(const T &current, const T &step,
                                    const T &end) noexcept
    : current_{ current }, step_{ step }, end_{ end } { }

    template <std::size_t N>
    constexpr void range_check(const char (&what)[N]) const {
        if (step_ > 0 && current_ >= end_) {
            throw std::out_of_range{ what };
        } else if (step_ < 0 && current_ <= end_) {
            throw std::out_of_range{ what };
        }
    }

    T current_ = 0;
    T step_ = 1;
    T end_;
};

template <typename T>
class CountingRange : public Range<CountingRange<T>> {
public:
    using difference_type =
        typename RangeTraits<CountingRange>::difference_type;
    using iterator = typename RangeTraits<CountingRange>::iterator;
    using pointer = typename RangeTraits<CountingRange>::pointer;
    using reference = typename RangeTraits<CountingRange>::reference;
    using value_type = typename RangeTraits<CountingRange>::value_type;

    constexpr explicit CountingRange(const T &end) noexcept : end_{ end } { }

    constexpr CountingRange(const T &begin, const T &end) noexcept
    : begin_{ begin }, end_{ end } { }

    constexpr CountingRange(const T &begin, const T &step,
                            const T &end) noexcept
    : begin_{ begin }, step_{ step },
      end_{ begin + step * (end - begin) / step } { }

    constexpr CountingRangeIterator<T> begin() const noexcept {
        return { begin_, step_, end_ };
    }

    constexpr CountingRangeIterator<T> end() const noexcept {
        return { end_, step_, end_ };
    }

private:
    T begin_ = 0;
    T step_ = 1;
    T end_;
};

template <typename T>
constexpr CountingRange<T> range(const T &end) noexcept {
    return CountingRange<T>{ end };
}

template <typename T>
constexpr CountingRange<T> range(const T &begin, const T &end) noexcept {
    return CountingRange<T>{ begin, end };
}

template <typename T>
constexpr CountingRange<T> range(const T &begin, const T &step,
                                 const T &end) noexcept {
    return CountingRange<T>{ begin, step, end };
}

template <typename T>
struct RangeTraits<CountingRange<T>> {
    using difference_type = iterator_difference_t<CountingRangeIterator<T>>;
    using iterator = CountingRangeIterator<T>;
    using pointer = iterator_pointer_t<CountingRangeIterator<T>>;
    using reference = iterator_reference_t<CountingRangeIterator<T>>;
    using value_type = iterator_value_t<CountingRangeIterator<T>>;
};

} // namespace ranges
} // namespace umigv

#endif
