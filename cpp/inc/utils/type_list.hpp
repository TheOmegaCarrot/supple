#ifndef UTILS_TYPE_LIST_HPP
#define UTILS_TYPE_LIST_HPP

#include <type_traits>

#include "metaprogramming.hpp"

namespace ehanc {

///////////////////////////////////////////// Type List

template <typename... Pack>
struct type_list {};

///////////////////////////////////////////// push_back

template <typename LIST, typename T>
struct push_back;

template <template <typename...> typename LIST, typename T,
          typename... Pack>
struct push_back<LIST<Pack...>, T>
    : ::ehanc::type_identity<LIST<Pack..., T>> {};

template <typename LIST, typename T>
using push_back_t = typename ::ehanc::push_back<LIST, T>::type;

///////////////////////////////////////////// push_front

template <typename LIST, typename T>
struct push_front;

template <template <typename...> typename LIST, typename... Pack,
          typename T>
struct push_front<LIST<Pack...>, T>
    : ::ehanc::type_identity<LIST<T, Pack...>> {};

template <typename LIST, typename T>
using push_front_t = typename ::ehanc::push_front<LIST, T>::type;

///////////////////////////////////////////// pop_back
/// oh boy...

///////////////////////////////////////////// pop_front
/// oh boy...

} // namespace ehanc

#endif
