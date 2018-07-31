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

#ifndef UMIGV_RANGES_COUNT_ITER_HPP
#define UMIGV_RANGES_COUNT_ITER_HPP

#include "iterator.hpp"

#include <cassert>
#include <limits>
#include <type_traits>

namespace umigv {
namespace ranges {

template <typename T = std::ptrdiff_t>
class IntCountIter : public RandomAccessIterator<IntCountIter<T>> {
public:
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    using difference_type = std::make_signed_t<T>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = const T*;
    using reference = const T&;
    using value_type = T;

    constexpr IntCountIter(const T &current, const T &step) noexcept
    : current_{ current }, step_{ step } { }

    constexpr IntCountIter& next() noexcept {
        current_ += step_;

        return *this;
    }

    constexpr IntCountIter& prev() noexcept {
        current_ -= step_;

        return *this;
    }

    constexpr IntCountIter& step(difference_type n) noexcept {
        const auto to_step = static_cast<difference_type>(step_) * n;

        current_ = static_cast<difference_type>(current_) + to_step;

        return *this;
    }

    constexpr reference deref() const noexcept {
        return current_;
    }

    constexpr difference_type distance(
        const IntCountIter &other
    ) const {
        assert(step_ == other.step_);

        return (other.current_ - current_) / step_;
    }

    constexpr bool is_equal(const IntCountIter &other) const {
        assert(step_ == other.step_);

        return current_ == other.current_;
    }

    constexpr bool is_less(const IntCountIter &other) const {
        assert(step_ == other.step_);

        if (step_ > 0) {
            return current_ < other.current_;
        }

        return other.current_ < current_;
    }

private:
    T current_ = 0;
    T step_ = 1;
};

template <typename T = double>
class RealCountIter : public RandomAccessIterator<RealCountIter<T>> {
public:
    static_assert(std::is_floating_point<T>::value,
                  "T must be a floating point type");

    using difference_type = T;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = const T*;
    using reference = const T&;
    using value_type = T;

    constexpr RealCountIter(const T &current, const T &step) noexcept
    : current_{ current }, step_{ step } { }

    constexpr RealCountIter& next() noexcept {
        current_ += step_;

        return *this;
    }

    constexpr RealCountIter& prev() noexcept {
        current_ -= step_;

        return *this;
    }

    constexpr RealCountIter& step(difference_type n) noexcept {
        const auto to_step = static_cast<difference_type>(step_) * n;

        current_ = static_cast<difference_type>(current_) + to_step;

        return *this;
    }

    constexpr reference deref() const noexcept {
        return current_;
    }

    constexpr difference_type distance(
        const RealCountIter &other
    ) const {
        assert(is_epsilon_equal(step_, other.step_));

        return (other.current_ - current_) / step_;
    }

    constexpr bool is_equal(const RealCountIter &other) const {
        assert(is_epsilon_equal(step_, other.step_));

        return is_epsilon_equal(current_, other.current_);
    }

    constexpr bool is_less(const RealCountIter &other) const {
        assert(is_epsilon_equal(step_, other.step_));

        if (step_ > 0) {
            return current_ < other.current_;
        }

        return other.current_ < current_;
    }

private:
    constexpr static bool is_epsilon_equal(const T &lhs,
                                           const T &rhs) noexcept {
        constexpr auto EPSILON =
            std::numeric_limits<difference_type>::epsilon();

        return (rhs - EPSILON < lhs) && (lhs < rhs + EPSILON);
    }

    T current_ = 0;
    T step_ = 1;
};

template <typename T>
using CountIter = std::conditional_t<
    std::is_integral<T>::value,
    IntCountIter<T>,
    RealCountIter<T>
>;

template <typename T>
constexpr CountIter<T> make_count_iter(const T &current,
                                       const T &step) noexcept {
    return { current, step };
}

template <typename T>
constexpr IterPair<CountIter<T>> make_count_iter(const T &first, const T &step,
                                                 const T &last) noexcept {
    const auto lastFloored = first + step * (last - first) / step;

    return { { first, step }, { lastFloored, step } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename T>
struct iterator_traits<umigv::ranges::IntCountIter<T>> {
    using difference_type = std::make_signed_t<T>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = const T*;
    using reference = const T&;
    using value_type = T;
};

template <typename T>
struct iterator_traits<umigv::ranges::RealCountIter<T>> {
    using difference_type = T;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = const T*;
    using reference = const T&;
    using value_type = T;
};

} // namespace std

#endif
