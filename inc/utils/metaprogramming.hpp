#ifndef EHANC_UTILS_METAPROGRAMMING_HPP
#define EHANC_UTILS_METAPROGRAMMING_HPP

#include <type_traits>

namespace ehanc {

/* {{{ doc */
/**
 * @brief Identity metafunction. This version is unneeded if using >=C++20
 */
/* }}} */
template <typename T>
struct type_identity {
  using type = T;
};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `type_identity`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
using type_identity_t = typename ::ehanc::type_identity<T>::type;

/* {{{ doc */
/**
 * @brief Metafunction to determine the type resulting from
 * addition of the parameter types.
 */
/* }}} */
template <typename... Ts>
struct sum_type
    : ::ehanc::type_identity<decltype((... + std::declval<Ts>()))> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `sum_type`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Ts>
using sum_type_t = typename ::ehanc::sum_type<Ts...>::type;

/* {{{ doc */
/**
 * @brief Metafunction to determine if type T is present in pack.
 */
/* }}} */
template <typename T, typename... Pack>
struct is_type_in_pack
    : std::conditional_t<(std::is_same_v<T, Pack> || ...), std::true_type,
                         std::false_type> {};

/* {{{ doc */
/**
 * @brief Metafunction to determine if type T is present in pack.
 * Specialization for size 0 pack.
 */
/* }}} */
template <typename T>
struct is_type_in_pack<T> : std::false_type {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_type_in_pack`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename... Pack>
constexpr inline const bool is_type_in_pack_v =
    ::ehanc::is_type_in_pack<T, Pack...>::value;

/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Zero argument case.
 */
/* }}} */
template <typename... Pack>
struct peel_first : ::ehanc::type_identity<void> {};

/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Specialization for non-zero length argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_first<First, Pack...> : ::ehanc::type_identity<First> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `peel_first`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
using peel_first_t = typename ::ehanc::peel_first<Pack...>::type;

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Zero argument case.
 */
/* }}} */
template <typename... Pack>
struct peel_last : ::ehanc::type_identity<void> {};

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for >1 argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_last<First, Pack...>
    : ::ehanc::type_identity<typename ::ehanc::peel_last<Pack...>::type> {
};

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for base case.
 */
/* }}} */
template <typename Last>
struct peel_last<Last> : ::ehanc::type_identity<Last> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `peel_last`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
using peel_last_t = typename ::ehanc::peel_last<Pack...>::type;

/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are the same type. Empty pack considered uniform.
 */
/* }}} */
template <typename... Pack>
struct is_pack_uniform
    : std::conditional_t<
          (std::is_same_v<::ehanc::peel_first_t<Pack...>, Pack> && ...),
          std::true_type, std::false_type> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pack_uniform`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
constexpr inline const bool is_pack_uniform_v =
    ::ehanc::is_pack_uniform<Pack...>::value;

namespace impl {
/* {{{ doc */
/**
 * @brief Incomplete implementation of a metafunction to get the size of
 * a pack.
 */
/* }}} */
template <typename T, typename... Pack>
struct pack_size_impl
    : std::integral_constant<
          std::size_t, ::ehanc::impl::pack_size_impl<Pack...>::value + 1> {
};

/* {{{ doc */
/**
 * @brief Incomplete implementation of a metafunction to get the size of
 * a pack. Specialization for base case.
 */
/* }}} */
template <typename T>
struct pack_size_impl<T> : std::integral_constant<std::size_t, 1> {};
} // namespace impl

template <typename... Pack>
struct pack_size
    : std::integral_constant<
          std::size_t,
          ::ehanc::impl::pack_size_impl<void, Pack...>::value - 1> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `pack_size`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
constexpr inline const std::size_t pack_size_v =
    ::ehanc::pack_size<Pack...>::value;

} // namespace ehanc

#endif