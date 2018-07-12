#ifndef UMIGV_RANGES_CONST_ITERATOR_HPP
#define UMIGV_RANGES_CONST_ITERATOR_HPP

#include "traits.hpp"

#include <iterator>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, bool = is_input_iterator<I>::value>
class ConstIterator { };

template <typename I>
class ConstIterator<I, true> {
public:
    using difference_type = iterator_difference_t<I>;
    using value_type = iterator_value_t<I>;
    using pointer = const std::remove_pointer_t<iterator_pointer_t<I>>*;
    using reference = const iterator_reference_t<I>;
    using iterator_category = std::input_iterator_tag;

    constexpr ConstIterator(const ConstIterator &other)
    noexcept(std::is_nothrow_copy_constructible<I>::value) = default;

    constexpr explicit ConstIterator(const I &base)
    noexcept(std::is_nothrow_copy_constructible<I>::value) : base_{ base } { }

    constexpr ConstIterator& operator=(const ConstIterator &other)
    noexcept(std::is_nothrow_copy_constructible<I>::value) = default;

    constexpr reference operator*() const {
        return *base_;
    }

    constexpr pointer operator->() const {
        return base_.operator->();
    }

    constexpr ConstIterator& operator++() {
        ++base_;

        return *this;
    }

    constexpr ConstIterator operator++(int) {
        const ConstIterator to_return = *this;

        ++*this;

        return to_return;
    }

    friend constexpr bool operator==(const ConstIterator &lhs,
                                     const ConstIterator &rhs)
    noexcept(is_nothrow_equality_comparable<I>::value) {
        return lhs.base_ == rhs.base_;
    }

    friend constexpr bool operator!=(const ConstIterator &lhs,
                                     const ConstIterator &rhs)
    noexcept(is_nothrow_equality_comparable<I>::value) {
        return !(lhs == rhs);
    }

private:
    I base_;
};

} // namespace ranges
} // namespace umigv

#endif
