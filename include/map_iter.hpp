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

#ifndef UMIGV_RANGES_MAP_ITER_HPP
#define UMIGV_RANGES_MAP_ITER_HPP

#include "detail/map_iter.hpp"

#include "apply.hpp"
#include "invoke.hpp"
#include "meta_iter.hpp"
#include "traits.hpp"

namespace umigv {
namespace ranges {

template <typename I, typename C>
class RandMapIter : public MetaIter<RandMapIter<I, C>> {
public:
    static_assert(is_random_access_iterator<I>::value,
                  "I must be a random access iterator");
    static_assert(is_invocable<const C&, iterator_reference_t<I>>::value
                  || is_applicable<const C&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply C with a value of type I");

    friend MetaIter<RandMapIter<I, C>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        detail::map_result_t<const C&, iterator_reference_t<I>>
    >>;
    using reference = detail::map_result_t<const C&, iterator_reference_t<I>>;
    using value_type =
        remove_cvref_t<detail::map_result_t<const C&, iterator_reference_t<I>>>;

    constexpr RandMapIter(const I &current, const C &callable)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<C>::value)
    : current_{ current }, callable_{ callable } { }

    constexpr reference deref() const {
        return detail::do_map(callable_, *current_);
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    C callable_;
};

template <typename I, typename C>
class BiMapIter : public MetaIter<BiMapIter<I, C>> {
public:
    static_assert(is_bidirectional_iterator<I>::value,
                  "I must be a bidirectional iterator");
    static_assert(is_invocable<const C&, iterator_reference_t<I>>::value
                  || is_applicable<const C&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply C with a value of type I");

    friend MetaIter<BiMapIter<I, C>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        detail::map_result_t<const C&, iterator_reference_t<I>>
    >>;
    using reference = detail::map_result_t<const C&, iterator_reference_t<I>>;
    using value_type =
        remove_cvref_t<detail::map_result_t<const C&, iterator_reference_t<I>>>;

    constexpr BiMapIter(const I &current, const C &callable)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<C>::value)
    : current_{ current }, callable_{ callable } { }

    constexpr reference deref() const {
        return detail::do_map(callable_, *current_);
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    C callable_;
};

template <typename I, typename C>
class FwdMapIter : public MetaIter<FwdMapIter<I, C>> {
public:
    static_assert(is_forward_iterator<I>::value,
                  "I must be a forward iterator");
    static_assert(is_invocable<const C&, iterator_reference_t<I>>::value
                  || is_applicable<const C&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply C with a value of type I");

    friend MetaIter<FwdMapIter<I, C>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        detail::map_result_t<const C&, iterator_reference_t<I>>
    >>;
    using reference = detail::map_result_t<const C&, iterator_reference_t<I>>;
    using value_type =
        remove_cvref_t<detail::map_result_t<const C&, iterator_reference_t<I>>>;

    constexpr FwdMapIter(const I &current, const C &callable)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<C>::value)
    : current_{ current }, callable_{ callable } { }

    constexpr reference deref() const {
        return detail::do_map(callable_, *current_);
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    C callable_;
};

template <typename I, typename C>
class InMapIter : public MetaIter<InMapIter<I, C>> {
public:
    static_assert(is_input_iterator<I>::value,
                  "I must be an input iterator");
    static_assert(is_invocable<const C&, iterator_reference_t<I>>::value
                  || is_applicable<const C&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply C with a value of type I");

    friend MetaIter<InMapIter<I, C>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        detail::map_result_t<const C&, iterator_reference_t<I>>
    >>;
    using reference = detail::map_result_t<const C&, iterator_reference_t<I>>;
    using value_type =
        remove_cvref_t<detail::map_result_t<const C&, iterator_reference_t<I>>>;

    constexpr InMapIter(const I &current, const C &callable)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<C>::value)
    : current_{ current }, callable_{ callable } { }

    constexpr reference deref() const {
        return detail::do_map(callable_, *current_);
    }

private:
    constexpr I& underlying() & noexcept {
        return current_;
    }

    constexpr const I& underlying() const & noexcept {
        return current_;
    }

    constexpr I&& underlying() && noexcept {
        return current_;
    }

    constexpr const I&& underlying() const && noexcept {
        return current_;
    }

    I current_;
    C callable_;
};

template <typename I, typename C>
using MapIter = std::conditional_t<
    is_random_access_iterator<I>::value,
    RandMapIter<I, C>,
    std::conditional_t<
        is_bidirectional_iterator<I>::value,
        BiMapIter<I, C>,
        std::conditional_t<
            is_forward_iterator<I>::value,
            FwdMapIter<I, C>,
            InMapIter<I, C>
        >
    >
>;

template <typename I, typename C>
constexpr MapIter<I, C> make_map_iter(const I &current, const C &callable)
noexcept(std::is_nothrow_copy_constructible<I>::value
         && std::is_nothrow_copy_constructible<C>::value) {
    return { current, callable };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename I, typename C>
struct iterator_traits<umigv::ranges::RandMapIter<I, C>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::random_access_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >>;
    using reference = umigv::ranges::detail::map_result_t<
        const C&,
        typename iterator_traits<I>::reference
    >;
    using value_type = umigv::ranges::remove_cvref_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >;
};

template <typename I, typename C>
struct iterator_traits<umigv::ranges::BiMapIter<I, C>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >>;
    using reference = umigv::ranges::detail::map_result_t<
        const C&,
        typename iterator_traits<I>::reference
    >;
    using value_type = umigv::ranges::remove_cvref_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >;
};

template <typename I, typename C>
struct iterator_traits<umigv::ranges::FwdMapIter<I, C>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::forward_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >>;
    using reference = umigv::ranges::detail::map_result_t<
        const C&,
        typename iterator_traits<I>::reference
    >;
    using value_type = umigv::ranges::remove_cvref_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >;
};

template <typename I, typename C>
struct iterator_traits<umigv::ranges::InMapIter<I, C>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::input_iterator_tag;
    using pointer = std::add_pointer_t<std::remove_reference_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >>;
    using reference = umigv::ranges::detail::map_result_t<
        const C&,
        typename iterator_traits<I>::reference
    >;
    using value_type = umigv::ranges::remove_cvref_t<
        umigv::ranges::detail::map_result_t<
            const C&,
            typename iterator_traits<I>::reference
        >
    >;
};

} // namespace std

#endif
