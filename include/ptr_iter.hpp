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

#ifndef UMIGV_RANGES_PTR_ITER_HPP
#define UMIGV_RANGES_PTR_ITER_HPP

#include "iterator.hpp"

#include <cstddef>
#include <iterator>
#include <utility>

namespace umigv {
namespace ranges {

template <typename T>
class PtrIter : public RandomAccessIterator<PtrIter<T>> {
public:
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = T*;
    using reference = T&;
    using value_type = T;

    constexpr explicit PtrIter(T *current) noexcept
    : current_{ current } { }

    constexpr PtrIter& next() noexcept {
        ++current_;

        return *this;
    }

    constexpr PtrIter& prev() noexcept {
        --current_;

        return *this;
    }

    constexpr PtrIter& step(difference_type n) noexcept {
        current_ += n;

        return *this;
    }

    constexpr difference_type distance(const PtrIter &other)
    const noexcept {
        return other.current_ - current_;
    }

    constexpr reference deref() const {
        return *current_;
    }

    constexpr bool is_equal(const PtrIter &other) const noexcept {
        return current_ == other.current_;
    }

    constexpr bool is_less(const PtrIter &other) const noexcept {
        return current_ < other.current_;
    }

private:
    T *current_;
};

template <typename T>
constexpr PtrIter<T> make_ptr_iter(T *current) noexcept {
    return PtrIter<T>{ current };
}

template <typename T>
constexpr IterPair<PtrIter<T>> make_ptr_iter(T *first, T *last) noexcept {
    return { PtrIter<T>{ first }, PtrIter<T>{ last } };
}

template <typename T>
constexpr IterPair<PtrIter<T>> make_ptr_iter(
    T *data,
    std::ptrdiff_t length
) noexcept {
    return { PtrIter<T>{ data }, PtrIter<T>{ data + length } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename T>
struct iterator_traits<::umigv::ranges::PtrIter<T>> {
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = T*;
    using reference = T&;
    using value_type = T;
};

} // namespace std

#endif
