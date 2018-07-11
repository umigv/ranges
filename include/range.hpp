#ifndef UMIGV_RANGES_RANGE_HPP
#define UMIGV_RANGES_RANGE_HPP

#include "filtered_range.hpp"
#include "mapped_range.hpp"
#include "range_fwd.hpp"

#include <type_traits>
#include <utility>

namespace umigv {
namespace ranges {

template <typename R>
class Range {
public:
    using difference_type = typename RangeTraits<R>::difference_type;
    using iterator = typename RangeTraits<R>::iterator;
    using pointer = typename RangeTraits<R>::pointer;
    using reference = typename RangeTraits<R>::reference;
    using value_type = typename RangeTraits<R>::value_type;

    constexpr iterator begin() const noexcept {
        return as_base().begin();
    }

    constexpr iterator end() const noexcept {
        return as_base().end();
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
