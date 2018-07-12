#ifndef UMIGV_RANGES_COLLECT_HPP
#define UMIGV_RANGES_COLLECT_HPP

#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I>
class Collectable {
public:
    constexpr Collectable(const I &first, const I &last)
    noexcept(std::is_nothrow_copy_constructible<I>::value)
    : first_{ first }, last_{ last } { }

    template <typename C,
              std::enable_if_t<std::is_constructible<C, I, I>::value, int> = 0>
    constexpr operator C() const {
        return C(first_, last_);
    }

private:
    I first_;
    I last_;
};

} // namespae ranges
} // namespace umigv

#endif
