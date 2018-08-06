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

#ifndef UMIGV_RANGES_ITERATOR_TRAITS_HPP
#define UMIGV_RANGES_ITERATOR_TRAITS_HPP

#include "traits.hpp"

#include <iterator>

namespace umigv {
namespace ranges {

template <typename T, typename = void>
struct IsIter : std::false_type { };

template <typename T>
struct IsIter<T, VoidT<
    typename std::iterator_traits<T>::difference_type,
    typename std::iterator_traits<T>::iterator_category,
    typename std::iterator_traits<T>::pointer,
    typename std::iterator_traits<T>::reference,
    typename std::iterator_traits<T>::value_type
>> : std::true_type { };

template <typename T>
constexpr bool IS_ITER = IsIter<T>::value;

template <typename T, bool = IS_ITER<T>>
struct IterDiff { };

template <typename T>
struct IterDiff<T, true> {
    using type = typename std::iterator_traits<T>::difference_type;
};

template <typename T>
using IterDiffT = typename IterDiff<T>::type;

template <typename T, bool = IS_ITER<T>>
struct IterVal { };

template <typename T>
struct IterVal<T, true> {
    using type = typename std::iterator_traits<T>::value_type;
};

template <typename T>
using IterValT = typename IterVal<T>::type;

template <typename T, bool = IS_ITER<T>>
struct IterPtr { };

template <typename T>
struct IterPtr<T, true> {
    using type = typename std::iterator_traits<T>::pointer;
};

template <typename T>
using IterPtrT = typename IterPtr<T>::type;

template <typename T, bool = IS_ITER<T>>
struct IterRef { };

template <typename T>
struct IterRef<T, true> {
    using type = typename std::iterator_traits<T>::reference;
};

template <typename T>
using IterRefT = typename IterRef<T>::type;

template <typename T, bool = IS_ITER<T>>
struct IterCat { };

template <typename T>
struct IterCat<T, true> {
    using type = typename std::iterator_traits<T>::iterator_category;
};

template <typename T>
using IterCatT = typename IterCat<T>::type;

template <typename T, bool = IS_ITER<T>>
struct IsInIter : std::false_type { };

template <typename T>
struct IsInIter<T, true> : TrueTypeIfT<std::is_base_of<
    std::input_iterator_tag,
    IterCatT<T>
>::value> { };

template <typename T>
constexpr bool IS_IN_ITER = IsInIter<T>::value;

template <typename T, bool = IS_ITER<T>>
struct IsOutIter : std::false_type { };

template <typename T>
struct IsOutIter<T, true> : TrueTypeIfT<std::is_base_of<
    std::output_iterator_tag,
    IterCatT<T>
>::value> { };

template <typename T>
constexpr bool IS_OUT_ITER = IsOutIter<T>::value;

template <typename T, bool = IS_ITER<T>>
struct IsFwdIter : std::false_type { };

template <typename T>
struct IsFwdIter<T, true> : TrueTypeIfT<std::is_base_of<
    std::forward_iterator_tag,
    IterCatT<T>
>::value> { };

template <typename T>
constexpr bool IS_FWD_ITER = IsFwdIter<T>::value;

template <typename T, bool = IS_ITER<T>>
struct IsBidirIter : std::false_type { };

template <typename T>
struct IsBidirIter<T, true> : TrueTypeIfT<std::is_base_of<
    std::bidirectional_iterator_tag,
    IterCatT<T>
>::value> { };

template <typename T>
constexpr bool IS_BIDIR_ITER = IsBidirIter<T>::value;

template <typename T, bool = IS_ITER<T>>
struct IsRandIter : std::false_type { };

template <typename T>
struct IsRandIter<T, true> : TrueTypeIfT<std::is_base_of<
    std::random_access_iterator_tag,
    IterCatT<T>
>::value> { };

template <typename T>
constexpr bool IS_RAND_ITER = IsRandIter<T>::value;

template <typename T, typename Rand, typename Bidir, typename Fwd, typename In>
struct RandIterSwitch {
    using type = std::conditional_t<
        IS_RAND_ITER<T>,
        Rand,
        std::conditional_t<
            IS_BIDIR_ITER<T>,
            Bidir,
            std::conditional_t<
                IS_FWD_ITER<T>,
                Fwd,
                In
            >
        >
    >;
};

template <typename T, typename Rand, typename Bidir, typename Fwd, typename In>
using RandIterSwitchT = typename RandIterSwitch<T, Rand, Bidir, Fwd, In>::type;

template <typename T, typename Bidir, typename Fwd, typename In>
struct BidirIterSwitch {
    using type = std::conditional_t<
        IS_BIDIR_ITER<T>,
        Bidir,
        std::conditional_t<
            IS_FWD_ITER<T>,
            Fwd,
            In
        >
    >;
};

template <typename T, typename Bidir, typename Fwd, typename In>
using BidirIterSwitchT = typename BidirIterSwitch<T, Bidir, Fwd, In>::type;

template <typename T, typename Fwd, typename In>
struct FwdIterSwitch {
    using type = std::conditional_t<
        IS_FWD_ITER<T>,
        Fwd,
        In
    >;
};

template <typename T, typename Fwd, typename In>
using FwdIterSwitchT = typename FwdIterSwitch<T, Fwd, In>::type;

} // namespace ranges
} // namespace umigv

#endif
