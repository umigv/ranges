#ifndef UMIGV_RANGES_MAPPED_RANGE_HPP
#define UMIGV_RANGES_MAPPED_RANGE_HPP

#include "invoke.hpp"
#include "range_fwd.hpp"

#include <memory>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, typename F,
          std::enable_if_t<
              is_invocable<const F&, iterator_reference_t<I>>::value, int
          > = 0>
class MappedRange;

template <typename I, typename F,
          std::enable_if_t<
              is_invocable<const F&, iterator_reference_t<I>>::value, int
          > = 0>
class MappedRangeIterator {
    using ResultT = invoke_result_t<const F&, iterator_reference_t<I>>;

public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<ResultT>>;
    using reference = ResultT;
    using value_type = std::decay_t<ResultT>;

    friend MappedRange<I, F>;

    constexpr reference operator*() const {
        return invoke(f_, *current_);
    }

    constexpr pointer operator->() const {
        return { std::addressof(**this) };
    }

    constexpr MappedRangeIterator& operator++() {
        ++current_;

        return *this;
    }

    constexpr MappedRangeIterator operator++(int) {
        const MappedRangeIterator to_return = *this;

        ++(*this);

        return to_return;
    }

    friend constexpr bool operator==(MappedRangeIterator lhs,
                                     MappedRangeIterator rhs) noexcept {
        return lhs.current_ == rhs.current_;
    }

    friend constexpr bool operator!=(MappedRangeIterator lhs,
                                     MappedRangeIterator rhs) noexcept {
        return lhs.current_ != rhs.current_;
    }

private:
    template <typename J, typename G,
              std::enable_if_t<
                  std::is_constructible<I, J&&>::value
                  && std::is_constructible<F, G&&>::value,
                  int
              > = 0>
    constexpr MappedRangeIterator(J &&j, G &&g)
    noexcept(std::is_nothrow_constructible<I, J&&>::value
             && std::is_nothrow_constructible<F, G&&>::value)
    : current_(std::forward<J>(j)), f_(std::forward<G>(g)) { }

    I current_;
    F f_;
};

template <typename I, typename F,
          std::enable_if_t<
              is_invocable<const F&, iterator_reference_t<I>>::value, int
          >>
class MappedRange : public Range<MappedRange<I, F>> {
    using IteratorT = MappedRangeIterator<I, F>;

public:
    using difference_type = iterator_difference_t<IteratorT>;
    using iterator = IteratorT;
    using pointer = iterator_pointer_t<IteratorT>;
    using reference = iterator_reference_t<IteratorT>;
    using value_type = iterator_value_t<IteratorT>;

    constexpr MappedRange(I first, I last, F f)
    noexcept(std::is_nothrow_move_constructible<I>::value
             && std::is_nothrow_move_constructible<F>::value)
    : first_{ std::move(first) }, last_{ std::move(last) }, f_{ std::move(f) }
    { }

    constexpr iterator begin() const noexcept {
        return { first_, f_ };
    }

    constexpr iterator end() const noexcept {
        return { last_, f_ };
    }

private:
    I first_;
    I last_;
    F f_;
};

template <typename R, typename F>
constexpr MappedRange<begin_result_t<R>, std::decay_t<F>> map(R &&range, F &&f) {
    using std::begin;
    using std::end;

    return MappedRange<begin_result_t<R>, F>{
        begin(std::forward<R>(range)),
        end(std::forward<R>(range)),
        std::forward<F>(f),
    };
}

} // namespace ranges
} // namespace umigv

#endif
