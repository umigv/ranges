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

#ifndef UMIGV_RANGES_FILTER_ITER_HPP
#define UMIGV_RANGES_FILTER_ITER_HPP

#include "detail/filter_iter.hpp"

#include "apply.hpp"
#include "invoke.hpp"
#include "iterator.hpp"
#include "meta_iter.hpp"
#include "traits.hpp"

namespace umigv {
namespace ranges {

template <typename I, typename P>
class BiFilterIter : public MetaIter<BiFilterIter<I, P>> {
public:
    static_assert(is_bidirectional_iterator<I>::value,
                  "I must be a bidirectional iterator");
    static_assert(is_invocable<const P&, iterator_reference_t<I>>::value
                  || is_applicable<const P&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply P with a value of type I");
    static_assert(
        std::is_constructible<
            bool,
            detail::filter_result_t<const P&, iterator_reference_t<I>>
        >::value,
        "P must be invocable or applicable with I "
        "to a type explicitly convertible to bool"
    );

    friend MetaIter<BiFilterIter<I, P>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr BiFilterIter(const IterTriple<I> &triple, const P &predicate)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value)
    : first_{ triple.first }, last_{ triple.last },
      current_{ triple.current }, predicate_{ predicate } {
        correct_forward();
    }

    constexpr BiFilterIter& next() {
        if (current_ == last_) {
            return *this;
        }

        do {
            ++current_;
        } while (current_ != last_
                 && !detail::do_filter(predicate_, *current_));

        return *this;
    }

    constexpr BiFilterIter& prev() {
        if (current_ == first_) {
            return *this;
        }

        do {
            --current_;
        } while (current_ != first_
                 && !detail::do_filter(predicate_, *current_));


        return *this;
    }

private:
    constexpr void correct_forward() {
        while (current_ != last_ && !detail::do_filter(predicate_, *current_)) {
            ++current_;
        }
    }

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

    I first_;
    I last_;
    I current_;
    P predicate_;
};

template <typename I, typename P>
class FwdFilterIter : public MetaIter<FwdFilterIter<I, P>> {
public:
    static_assert(is_forward_iterator<I>::value,
                  "I must be a forward iterator");
    static_assert(is_invocable<const P&, iterator_reference_t<I>>::value
                  || is_applicable<const P&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply P with a value of type I");
    static_assert(
        std::is_constructible<
            bool,
            detail::filter_result_t<const P&, iterator_reference_t<I>>
        >::value,
        "P must be invocable or applicable with I "
        "to a type explicitly convertible to bool"
    );

    friend MetaIter<FwdFilterIter<I, P>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr FwdFilterIter(const IterTriple<I> &triple, const P &predicate)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value)
    : last_{ triple.last }, current_{ triple.current },
      predicate_{ predicate } {
        correct_forward();
    }

    constexpr FwdFilterIter& next() {
        if (current_ == last_) {
            return *this;
        }

        do {
            ++current_;
        } while (current_ != last_
                 && !detail::do_filter(predicate_, *current_));

        return *this;
    }

private:
    constexpr void correct_forward() {
        while (current_ != last_ && !detail::do_filter(predicate_, *current_)) {
            ++current_;
        }
    }

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

    I last_;
    I current_;
    P predicate_;
};

template <typename I, typename P>
class InFilterIter : public MetaIter<InFilterIter<I, P>> {
public:
    static_assert(is_input_iterator<I>::value,
                  "I must be a forward iterator");
    static_assert(is_invocable<const P&, iterator_reference_t<I>>::value
                  || is_applicable<const P&, iterator_reference_t<I>>::value,
                  "must be able to invoke or apply P with a value of type I");
    static_assert(
        std::is_constructible<
            bool,
            detail::filter_result_t<const P&, iterator_reference_t<I>>
        >::value,
        "P must be invocable or applicable with I "
        "to a type explicitly convertible to bool"
    );

    friend MetaIter<InFilterIter<I, P>>;

    using difference_type = iterator_difference_t<I>;
    using iterator_category = std::forward_iterator_tag;
    using pointer = iterator_pointer_t<I>;
    using reference = iterator_reference_t<I>;
    using value_type = iterator_value_t<I>;

    constexpr InFilterIter(const IterTriple<I> &triple, const P &predicate)
    noexcept(std::is_nothrow_copy_constructible<I>::value
             && std::is_nothrow_copy_constructible<P>::value)
    : last_{ triple.last }, current_{ triple.current },
      predicate_{ predicate } {
        correct_forward();
    }

    constexpr InFilterIter& next() {
        if (current_ == last_) {
            return *this;
        }

        do {
            ++current_;
        } while (current_ != last_
                 && !detail::do_filter(predicate_, *current_));

        return *this;
    }

private:
    constexpr void correct_forward() {
        while (current_ != last_ && !detail::do_filter(predicate_, *current_)) {
            ++current_;
        }
    }

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

    I last_;
    I current_;
    P predicate_;
};

template <typename I, typename P>
using FilterIter = std::conditional_t<
    is_bidirectional_iterator<I>::value,
    BiFilterIter<I, P>,
    std::conditional_t<
        is_forward_iterator<I>::value,
        FwdFilterIter<I, P>,
        InFilterIter<I, P>
    >
>;

template <typename I, typename P>
constexpr FilterIter<I, P> make_filter_iter(const IterTriple<I> &triple,
                                            const P &predicate)
noexcept(std::is_nothrow_copy_constructible<I>::value
         && std::is_nothrow_copy_constructible<P>::value) {
    return { triple, predicate };
}

template <typename I, typename P>
constexpr IterPair<FilterIter<I, P>> make_filter_iter(
    const IterPair<I> &bounds,
    const P &predicate
) noexcept(std::is_nothrow_copy_constructible<I>::value
           && std::is_nothrow_copy_constructible<P>::value) {
    return { { { bounds.first, bounds.last, bounds.first }, predicate },
             { { bounds.first, bounds.last, bounds.last }, predicate } };
}

} // namespace ranges
} // namespace umigv

namespace std {

template <typename I, typename P>
struct iterator_traits<umigv::ranges::BiFilterIter<I, P>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I, typename P>
struct iterator_traits<umigv::ranges::FwdFilterIter<I, P>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::forward_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

template <typename I, typename P>
struct iterator_traits<umigv::ranges::InFilterIter<I, P>> {
    using difference_type = typename iterator_traits<I>::difference_type;
    using iterator_category = std::input_iterator_tag;
    using pointer = typename iterator_traits<I>::pointer;
    using reference = typename iterator_traits<I>::reference;
    using value_type = typename iterator_traits<I>::value_type;
};

} // namespace std

#endif
