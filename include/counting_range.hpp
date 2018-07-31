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

#ifndef UMIGV_RANGES_COUNTING_RANGE_HPP
#define UMIGV_RANGES_COUNTING_RANGE_HPP

#include "range_fwd.hpp"
#include "count_iter.hpp"

namespace umigv {
namespace ranges {

template <typename T>
class CountingRange : public Range<CountingRange<T>> {
public:
    using difference_type =
        typename RangeTraits<CountingRange>::difference_type;
    using iterator = typename RangeTraits<CountingRange>::iterator;
    using pointer = typename RangeTraits<CountingRange>::pointer;
    using reference = typename RangeTraits<CountingRange>::reference;
    using value_type = typename RangeTraits<CountingRange>::value_type;

    constexpr explicit CountingRange(const T &end) noexcept
    : CountingRange{ 0, 1, end } { }

    constexpr CountingRange(const T &begin, const T &end) noexcept
    : CountingRange{ begin, 1, end } { }

    constexpr CountingRange(const T &begin, const T &step,
                            const T &end) noexcept
    : first_{ begin, step },
      last_{ begin + step * (end - begin) / step, step } { }

    constexpr iterator begin() const noexcept {
        return first_;
    }

    constexpr iterator end() const noexcept {
        return last_;
    }

private:
    iterator first_;
    iterator last_;
};

template <typename T>
constexpr CountingRange<T> range(const T &end) noexcept {
    return CountingRange<T>{ end };
}

template <typename T>
constexpr CountingRange<T> range(const T &begin, const T &end) noexcept {
    return CountingRange<T>{ begin, end };
}

template <typename T>
constexpr CountingRange<T> range(const T &begin, const T &step,
                                 const T &end) noexcept {
    return CountingRange<T>{ begin, step, end };
}

template <typename T>
struct RangeTraits<CountingRange<T>> {
    using difference_type = iterator_difference_t<CountIter<T>>;
    using iterator = CountIter<T>;
    using pointer = iterator_pointer_t<CountIter<T>>;
    using reference = iterator_reference_t<CountIter<T>>;
    using value_type = iterator_value_t<CountIter<T>>;
};

} // namespace ranges
} // namespace umigv

#endif
