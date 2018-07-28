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

#ifndef UMIGV_RANGES_FILTERED_RANGE_HPP
#define UMIGV_RANGES_FILTERED_RANGE_HPP

#include "detail/filtered_range.hpp"

#include "apply.hpp"
#include "invoke.hpp"
#include "range_fwd.hpp"
#include "traits.hpp"

#include <type_traits>

namespace umigv {
namespace ranges {

template <typename I, typename P,
          std::enable_if_t<detail::is_filterable<I, P>::value, int> = 0>
class FilteredRange;

template <typename I, typename P,
          std::enable_if_t<detail::is_filterable<I, P>::value, int> = 0,
          typename = void>
class FilteredRangeIterator {
public:
    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::input_iterator_tag;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    friend FilteredRange<I, P>;

    constexpr reference operator*() const {
        if (current_ == last_) {
            throw std::out_of_range{ "FilteredRangeIterator::operator*" };
        }

        return *current_;
    }

    constexpr pointer operator->() const {
        return { std::addressof(**this) };
    }

    constexpr FilteredRangeIterator& operator++() {
        if (current_ == last_) {
            throw std::out_of_range{ "FilteredRangeIterator::operator++" };
        }

        ++current_;
        detail::advance(current_, last_, predicate_);

        return *this;
    }

    constexpr FilteredRangeIterator operator++(int) {
        const FilteredRangeIterator to_return = *this;

        ++(*this);

        return to_return;
    }

    friend constexpr bool operator==(const FilteredRangeIterator &lhs,
                                     const FilteredRangeIterator &rhs) {
        if (!(lhs.last_ == rhs.last_)) {
            throw std::out_of_range{ "FilteredRangeIterator::operator==" };
        }

        return lhs.current_ == rhs.current_;
    }

    friend constexpr bool operator!=(const FilteredRangeIterator &lhs,
                                     const FilteredRangeIterator &rhs) {
        return !(lhs == rhs);
    }

private:
    constexpr FilteredRangeIterator(const I &current, const I &last,
                                    const P &predicate)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value)
    : current_{ current }, last_{ last }, predicate_{ predicate } {
        detail::advance(current_, last_, predicate_);
    }

    I current_;
    I last_;
    P predicate_;
};

template <typename I, typename P,
          std::enable_if_t<detail::is_filterable<I, P>::value, int>>
class FilteredRange : public Range<FilteredRange<I, P>> {
public:
    using difference_type =
        typename RangeTraits<FilteredRange>::difference_type;
    using iterator = typename RangeTraits<FilteredRange>::iterator;
    using pointer = typename RangeTraits<FilteredRange>::pointer;
    using reference = typename RangeTraits<FilteredRange>::reference;
    using value_type = typename RangeTraits<FilteredRange>::value_type;

    constexpr FilteredRange(const I &first, const I &last, const P &predicate)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value)
    : first_{ first }, last_{ last }, predicate_{ predicate } { }

    constexpr iterator begin() const
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value) {
        return { first_, last_, predicate_ };
    }

    constexpr iterator end() const
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value) {
        return { last_, last_, predicate_ };
    }

private:
    I first_;
    I last_;
    P predicate_;
};

template <typename R, typename P>
constexpr decltype(auto) filter(R &&range, P &&predicate)
noexcept(std::is_nothrow_copy_constructible<begin_result_t<R>>::value
         && std::is_nothrow_copy_constructible<P>::value) {
    using std::begin;
    using std::end;

    return FilteredRange<begin_result_t<R>, std::decay_t<P>> {
        begin(std::forward<R>(range)),
        end(std::forward<R>(range)),
        std::forward<P>(predicate)
    };
}

template <typename I, typename P>
struct RangeTraits<FilteredRange<I, P>> {
    using difference_type = iterator_difference_t<FilteredRangeIterator<I, P>>;
    using iterator = FilteredRangeIterator<I, P>;
    using pointer = iterator_pointer_t<FilteredRangeIterator<I, P>>;
    using reference = iterator_reference_t<FilteredRangeIterator<I, P>>;
    using value_type = iterator_value_t<FilteredRangeIterator<I, P>>;
};

} // namespae ranges
} // namespace umigv

#endif
