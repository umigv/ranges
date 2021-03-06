#ifndef UMIGV_RANGES_ENUMERATED_RANGE_HPP
#define UMIGV_RANGES_ENUMERATED_RANGE_HPP

#include "range_fwd.hpp"
#include "traits.hpp"

#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

#include <type_safe/optional.hpp>

namespace umigv {
namespace ranges {

template <typename I, typename T>
class EnumeratedRange;

template <typename I, typename T>
class EnumeratedRangeIterator {
public:
    static_assert(is_input_iterator<I>::value,
                  "I must be at least an InputIterator");
    static_assert(std::is_trivially_destructible<
                      iterator_reference_t<I>
                  >::value, "I::reference must be trivially destructible");
    static_assert(std::is_trivially_destructible<T>::value,
                  "T must be trivially destructible");

    friend EnumeratedRange<I, T>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = const std::pair<T, iterator_reference_t<I>>*;
    using reference = const std::pair<T, iterator_reference_t<I>>&;
    using value_type = std::pair<T, iterator_reference_t<I>>;

    reference operator*() const {
        bounds_check();

        data_.emplace(index_, *current_);

        return data_.value();
    }

    pointer operator->() const {
        return std::addressof(**this);
    }

    constexpr EnumeratedRangeIterator& operator++() {
        bounds_check();

        ++index_;
        ++current_;

        return *this;
    }

    constexpr EnumeratedRangeIterator operator++(int) {
        const auto to_return = *this;

        ++*this;

        return to_return;
    }

    friend constexpr bool operator==(const EnumeratedRangeIterator &lhs,
                                     const EnumeratedRangeIterator &rhs) {
        if (!(lhs.last_ == rhs.last_)) {
            throw std::out_of_range{ "EnumeratedRangeIterator::operator==" };
        }

        return lhs.current_ == rhs.current_;
    }

    friend constexpr bool operator!=(const EnumeratedRangeIterator &lhs,
                                     const EnumeratedRangeIterator &rhs) {
        return !(lhs == rhs);
    }

private:
    constexpr EnumeratedRangeIterator(const I &current, const I &last)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_default_constructible<T>::value)
    : current_{ current }, last_{ last } { }

    constexpr void bounds_check() const {
        if (current_ == last_) {
            throw std::out_of_range{ "EnumeratedRangeIterator::bounds_check" };
        }
    }

    I current_;
    I last_;
    T index_{ };
    mutable type_safe::optional<std::pair<T, iterator_reference_t<I>>> data_;
};

template <typename I, typename T>
class EnumeratedRange : public Range<EnumeratedRange<I, T>> {
public:
    using difference_type =
        typename RangeTraits<EnumeratedRange>::difference_type;
    using iterator = typename RangeTraits<EnumeratedRange>::iterator;
    using pointer = typename RangeTraits<EnumeratedRange>::pointer;
    using reference = typename RangeTraits<EnumeratedRange>::reference;
    using value_type = typename RangeTraits<EnumeratedRange>::value_type;

    constexpr EnumeratedRange(const I &first, const I &last)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : first_{ first }, last_{ last } { }

    constexpr EnumeratedRangeIterator<I, T> begin() const
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_default_constructible<T>::value) {
        return { first_, last_ };
    }

    constexpr EnumeratedRangeIterator<I, T> end() const
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_default_constructible<T>::value) {
        return { last_, last_ };
    }

private:
    I first_;
    I last_;
};

template <typename I, typename T>
struct RangeTraits<EnumeratedRange<I, T>> {
    using difference_type =
        iterator_difference_t<EnumeratedRangeIterator<I, T>>;
    using iterator = EnumeratedRangeIterator<I, T>;
    using pointer = iterator_pointer_t<EnumeratedRangeIterator<I, T>>;
    using reference = iterator_reference_t<EnumeratedRangeIterator<I, T>>;
    using value_type = iterator_value_t<EnumeratedRangeIterator<I, T>>;
};

template <typename T = std::size_t, typename R>
EnumeratedRange<begin_result_t<R>, T> enumerate(R &&range)
noexcept(std::is_nothrow_copy_constructible<begin_result_t<R>>::value
         && std::is_nothrow_default_constructible<T>::value) {
    using std::begin;
    using std::end;

    return { begin(std::forward<R>(range)), end(std::forward<R>(range)) };
}

} // namespace umigv
} // namespace ranges

#endif
