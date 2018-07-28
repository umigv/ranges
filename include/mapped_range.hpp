// BSD 3-Clause License
//
// Copyright (c) 2018, UMIGV
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproducne the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef UMIGV_RANGES_MAPPED_RANGE_HPP
#define UMIGV_RANGES_MAPPED_RANGE_HPP

#include "detail/mapped_range.hpp"

#include "invoke.hpp"
#include "range_fwd.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, typename F,
          std::enable_if_t<detail::is_mappable<I, F>::value, int> = 0>
class MappedRange;

template <typename I, typename F,
          std::enable_if_t<detail::is_mappable<I, F>::value, int> = 0>
class MappedRangeIterator {
    using ResultT = detail::map_result_t<I, F>;

public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<ResultT>>;
    using reference = ResultT;
    using value_type = std::decay_t<ResultT>;

    friend MappedRange<I, F>;

    constexpr reference operator*() const {
        if (current_ == last_) {
            throw std::out_of_range{ "MappedRangeIterator::operator*" };
        }

        return detail::do_map(current_, f_);
    }

    constexpr pointer operator->() const {
        return { std::addressof(**this) };
    }

    constexpr MappedRangeIterator& operator++() {
        if (current_ == last_) {
            throw std::out_of_range{ "MappedRangeIterator::operator++" };
        }

        ++current_;

        return *this;
    }

    constexpr MappedRangeIterator operator++(int) {
        const MappedRangeIterator to_return = *this;

        ++(*this);

        return to_return;
    }

    friend constexpr bool operator==(const MappedRangeIterator &lhs,
                                     const MappedRangeIterator &rhs) {
        if (!(lhs.last_ == rhs.last_)) {
            throw std::out_of_range{ "MappedRangeIterator::operator==" };
        }

        return lhs.current_ == rhs.current_;
    }

    friend constexpr bool operator!=(const MappedRangeIterator &lhs,
                                     const MappedRangeIterator &rhs) {
        return !(lhs == rhs);
    }

private:
    constexpr MappedRangeIterator(const I &current, const I &last, const F &f)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<F>::value)
    : current_{ current }, last_{ last }, f_{ f } { }

    I current_;
    I last_;
    F f_;
};

template <typename I, typename F,
          std::enable_if_t<detail::is_mappable<I, F>::value, int>>
class MappedRange : public Range<MappedRange<I, F>> {
    using IteratorT = MappedRangeIterator<I, F>;

public:
    using difference_type = typename RangeTraits<MappedRange>::difference_type;
    using iterator = typename RangeTraits<MappedRange>::iterator;
    using pointer = typename RangeTraits<MappedRange>::pointer;
    using reference = typename RangeTraits<MappedRange>::reference;
    using value_type = typename RangeTraits<MappedRange>::value_type;

    constexpr MappedRange(const I &first, const I &last, const F &f)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<F>::value)
    : first_{ first }, last_{ last }, f_{ f } { }

    constexpr iterator begin() const
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<F>::value) {
        return { first_, last_, f_ };
    }

    constexpr iterator end() const
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<F>::value) {
        return { last_, last_, f_ };
    }

private:
    I first_;
    I last_;
    F f_;
};

template <typename R, typename F>
constexpr decltype(auto) map(R &&range, F &&f)
noexcept(std::is_nothrow_copy_constructible<begin_result_t<R>>::value
         && std::is_nothrow_copy_constructible<F>::value) {
    using std::begin;
    using std::end;

    return MappedRange<begin_result_t<R>, std::decay_t<F>>{
        begin(std::forward<R>(range)),
        end(std::forward<R>(range)),
        std::forward<F>(f),
    };
}

template <typename I, typename F>
struct RangeTraits<MappedRange<I, F>> {
    using difference_type = iterator_difference_t<MappedRangeIterator<I, F>>;
    using iterator = MappedRangeIterator<I, F>;
    using pointer = iterator_pointer_t<MappedRangeIterator<I, F>>;
    using reference = iterator_reference_t<MappedRangeIterator<I, F>>;
    using value_type = iterator_value_t<MappedRangeIterator<I, F>>;
};

} // namespace ranges
} // namespace umigv

#endif
