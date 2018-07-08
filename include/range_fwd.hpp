#ifndef UMIGV_RANGES_RANGE_FWD_HPP
#define UMIGV_RANGES_RANGE_FWD_HPP

namespace umigv {
namespace ranges {

template <typename R>
class Range;

template <typename R>
struct RangeTraits {
    using difference_type = typename R::difference_type;
    using iterator = typename R::iterator;
    using pointer = typename R::pointer;
    using reference = typename R::reference;
    using value_type = typename R::value_type;
};

} // namespace ranges
} // namespace umigv

#endif
