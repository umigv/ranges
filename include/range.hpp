#ifndef UMIGV_RANGES_RANGE_HPP
#define UMIGV_RANGES_RANGE_HPP

#include "collect.hpp"
#include "const_iterator.hpp"
#include "enumerated_range.hpp"
#include "filtered_range.hpp"
#include "mapped_range.hpp"
#include "range_adapter.hpp"
#include "range_fwd.hpp"
#include "zipped_range.hpp"

#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename R>
class Range {
public:
    using const_iterator = ConstIterator<typename RangeTraits<R>::iterator>;
    using difference_type = typename RangeTraits<R>::difference_type;
    using iterator = typename RangeTraits<R>::iterator;
    using pointer = typename RangeTraits<R>::pointer;
    using reference = typename RangeTraits<R>::reference;
    using value_type = typename RangeTraits<R>::value_type;

    constexpr iterator begin() const noexcept {
        return as_base().begin();
    }

    constexpr const_iterator cbegin() const noexcept {
        return const_iterator{ begin() };
    }

    constexpr iterator end() const noexcept {
        return as_base().end();
    }

    constexpr const_iterator cend() const noexcept {
        return const_iterator{ end() };
    }

    template <typename F>
    constexpr MappedRange<iterator, F> map(F &&f)
    noexcept(noexcept(
        ::umigv::ranges::map(std::declval<Range>(), std::declval<F>())
    )) {
        return ::umigv::ranges::map(*this, std::forward<F>(f));
    }

    template <typename P>
    constexpr FilteredRange<iterator, P> filter(P &&predicate)
    noexcept(noexcept(
        ::umigv::ranges::filter(std::declval<Range>(), std::declval<P>())
    )) {
        return ::umigv::ranges::filter(*this, std::forward<P>(predicate));
    }

    template <typename T = std::size_t>
    constexpr EnumeratedRange<iterator, T> enumerate()
    noexcept(noexcept(
        ::umigv::ranges::enumerate<T>(std::declval<Range>())
    )) {
        return ::umigv::ranges::enumerate<T>(*this);
    }

    template <typename ...Rs>
    constexpr decltype(auto) zip(Rs &&...ranges)
    noexcept(noexcept(
        ::umigv::ranges::zip(std::declval<Range>(), std::declval<Rs>()...)
    )) {
        return ::umigv::ranges::zip(*this, std::forward<Rs>(ranges)...);
    }

    constexpr Collectable<iterator> collect() const {
        return { begin(), end() };
    }

    template <
        typename C,
        std::enable_if_t<
            std::is_constructible<C, iterator, iterator>::value,
            int
        > = 0
    >
    constexpr C collect() const {
        return C(begin(), end());
    }

    constexpr RangeAdapter<ConstIterator<iterator>> as_const() const noexcept {
        return ::umigv::ranges::adapt(cbegin(), cend());
    }

private:
    constexpr R& as_base() & noexcept {
        return static_cast<R&>(*this);
    }

    constexpr const R& as_base() const & noexcept {
        return static_cast<const R&>(*this);
    }

    constexpr R&& as_base() && noexcept {
        return static_cast<R&&>(*this);
    }

    constexpr const R&& as_base() const && noexcept {
        return static_cast<const R&&>(*this);
    }
};

} // namespace range
} // namespace umigv

#endif
