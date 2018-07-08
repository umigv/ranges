#ifndef UMIGV_RANGES_RANGE_HPP
#define UMIGV_RANGES_RANGE_HPP

#include <type_traits>
#include <utility>

#include "mapped_range.hpp"

namespace umigv {
namespace ranges {

template <typename R>
class Range {
public:
    using difference_type = typename R::difference_type;
    using iterator = typename R::iterator;
    using pointer = typename R::pointer;
    using reference = typename R::reference;
    using value_type = typename R::value_type;

    iterator begin() const noexcept {
        return as_base().begin();
    }

    iterator end() const noexcept {
        return as_base().end();
    }

    template <typename F>
    MappedRange<iterator, F> map(F &&f) {
        return map(as_base(), std::forward<F>(f));
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
