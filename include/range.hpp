#ifndef UMIGV_RANGES_RANGE_HPP
#define UMIGV_RANGES_RANGE_HPP

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

    iterator begin() const noexcept {
        return as_base().begin();
    }

    iterator end() const noexcept {
        return as_base().end();
    }

    template <typename F>
    MappedRange<iterator, F> map(F &&f) {
        return ::umigv::ranges::map(as_base(), std::forward<F>(f));
    }

private:
    R& as_base() & noexcept {
        return static_cast<R&>(*this);
    }

    const R& as_base() const & noexcept {
        return static_cast<const R&>(*this);
    }

    R&& as_base() && noexcept {
        return static_cast<R&&>(*this);
    }

    const R&& as_base() const && noexcept {
        return static_cast<const R&&>(*this);
    }
};

} // namespace range
} // namespace umigv

#endif
