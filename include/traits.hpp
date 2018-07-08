#ifndef UMIGV_RANGES_TRAITS_HPP
#define UMIGV_RANGES_TRAITS_HPP

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace umigv {

template <typename ...Ts>
using void_t = void;

template <bool Condition>
struct true_type_if : std::false_type { };

template <>
struct true_type_if<true> : std::true_type { };

template <bool Condition>
using true_type_if_t = typename true_type_if<Condition>::type;

template <typename T, typename = void>
struct is_iterator : std::false_type { };

template <typename T>
struct is_iterator<
    T,
    void_t<typename std::iterator_traits<T>::difference_type,
           typename std::iterator_traits<T>::value_type,
           typename std::iterator_traits<T>::pointer,
           typename std::iterator_traits<T>::reference,
           typename std::iterator_traits<T>::iterator_category>
> : std::true_type { };

template <typename T, bool IsIterator = is_iterator<T>::value>
struct iterator_difference { };

template <typename T>
struct iterator_difference<T, true> {
    using type = typename std::iterator_traits<T>::difference_type;
};

template <typename T>
using iterator_difference_t = typename iterator_difference<T>::type;

template <typename T, bool IsIterator = is_iterator<T>::value>
struct iterator_value { };

template <typename T>
struct iterator_value<T, true> {
    using type = typename std::iterator_traits<T>::value_type;
};

template <typename T>
using iterator_value_t = typename iterator_value<T>::type;

template <typename T, bool IsIterator = is_iterator<T>::value>
struct iterator_pointer { };

template <typename T>
struct iterator_pointer<T, true> {
    using type = typename std::iterator_traits<T>::pointer;
};

template <typename T>
using iterator_pointer_t = typename iterator_pointer<T>::type;

template <typename T, bool IsIterator = is_iterator<T>::value>
struct iterator_reference { };

template <typename T>
struct iterator_reference<T, true> {
    using type = typename std::iterator_traits<T>::reference;
};

template <typename T>
using iterator_reference_t = typename iterator_reference<T>::type;

template <typename T, bool IsIterator = is_iterator<T>::value>
struct iterator_category { };

template <typename T>
struct iterator_category<T, true> {
    using type = typename std::iterator_traits<T>::iterator_category;
};

template <typename T>
using iterator_category_t = typename iterator_category<T>::type;

template <typename T, bool IsIterator = is_iterator<T>::value>
struct is_input_iterator : std::false_type { };

template <typename T>
struct is_input_iterator<T, true>
: true_type_if_t<std::is_base_of<std::input_iterator_tag,
                                 iterator_category_t<T>>::value> { };

template <typename T, bool IsIterator = is_iterator<T>::value>
struct is_output_iterator : std::false_type { };

template <typename T>
struct is_output_iterator<T, true>
: true_type_if_t<std::is_base_of<std::output_iterator_tag,
                                 iterator_category_t<T>>::value> { };

template <typename T, bool IsIterator = is_iterator<T>::value>
struct is_forward_iterator : std::false_type { };

template <typename T>
struct is_forward_iterator<T, true>
: true_type_if_t<std::is_base_of<std::forward_iterator_tag,
                                 iterator_category_t<T>>::value> { };

template <typename T, bool IsIterator = is_iterator<T>::value>
struct is_bidirectional_iterator : std::false_type { };

template <typename T>
struct is_bidirectional_iterator<T, true>
: true_type_if_t<std::is_base_of<std::bidirectional_iterator_tag,
                                 iterator_category_t<T>>::value> { };

template <typename T, bool IsIterator = is_iterator<T>::value>
struct is_random_access_iterator : std::false_type { };

template <typename T>
struct is_random_access_iterator<T, true>
: true_type_if_t<std::is_base_of<std::random_access_iterator_tag,
                                 iterator_category_t<T>>::value> { };

template <typename T, typename = void>
struct begin_result { };

template <typename T>
struct begin_result<T, void_t<decltype(std::begin(std::declval<T>()))>> {
    using type = decltype((std::begin(std::declval<T>())));
};

template <typename T>
using begin_result_t = typename begin_result<T>::type;

} // namespace umigv

#endif
