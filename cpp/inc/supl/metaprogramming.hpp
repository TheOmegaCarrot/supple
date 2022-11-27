#ifndef EHANC_UTILS_METAPROGRAMMING_HPP
#define EHANC_UTILS_METAPROGRAMMING_HPP

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace supl {

// type_identity
/* {{{ doc */
/**
 * @brief Identity metafunction. This version is redundant if using
 * >=C++20, as it is provided by the standard.
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
using type_identity_t = typename ::supl::type_identity<T>::type;

// index_constant
template <std::size_t idx>
struct index_constant : std::integral_constant<std::size_t, idx> {};

// index_pair
template <std::size_t t_first, std::size_t t_second>
struct index_pair {
  constexpr inline static std::size_t first {t_first};
  constexpr inline static std::size_t second {t_second};
};

// sum_type
/* {{{ doc */
/**
 * @brief Metafunction to determine the type resulting from
 * addition of the parameter types.
 */
/* }}} */
template <typename... Ts>
struct sum_type
    : ::supl::type_identity<decltype((... + std::declval<Ts>()))> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `sum_type`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Ts>
using sum_type_t = typename ::supl::sum_type<Ts...>::type;

// is_type_in_pack
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
constexpr inline bool is_type_in_pack_v =
    ::supl::is_type_in_pack<T, Pack...>::value;

// peel_first
/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Zero argument case.
 */
/* }}} */
template <typename... Pack>
struct peel_first : ::supl::type_identity<void> {};

/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Specialization for non-zero length argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_first<First, Pack...> : ::supl::type_identity<First> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `peel_first`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
using peel_first_t = typename ::supl::peel_first<Pack...>::type;

// peel_last
/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Zero argument case.
 */
/* }}} */
template <typename... Pack>
struct peel_last : ::supl::type_identity<void> {};

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for >1 argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_last<First, Pack...>
    : ::supl::type_identity<typename ::supl::peel_last<Pack...>::type> {
};

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for base case.
 */
/* }}} */
template <typename Last>
struct peel_last<Last> : ::supl::type_identity<Last> {};

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `peel_last`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
using peel_last_t = typename ::supl::peel_last<Pack...>::type;

// is_pack_uniform
/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are the same type. Empty pack considered uniform.
 */
/* }}} */
template <typename... Pack>
struct is_pack_uniform
    : std::conditional_t<
          (std::is_same_v<::supl::peel_first_t<Pack...>, Pack> && ...),
          std::true_type, std::false_type> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pack_uniform`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
constexpr inline bool is_pack_uniform_v =
    ::supl::is_pack_uniform<Pack...>::value;

// is_pack_only
/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are a specifc type. Returns false if pack is empty.
 */
/* }}} */
template <typename T, typename... Pack>
struct is_pack_only
    : std::conjunction<::supl::is_pack_uniform<Pack...>,
                       std::is_same<T, supl::peel_first_t<Pack...>>> {};

/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are a specifc type. Returns false if pack is empty.
 * Specialization for empty pack.
 */
/* }}} */
template <typename T>
struct is_pack_only<T> : std::false_type {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pack_only`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename... Pack>
constexpr inline bool is_pack_only_v =
    ::supl::is_pack_only<T, Pack...>::value;

// is_iterable
/* {{{ doc */
/**
 * @brief Metafunction to determine if a type can be iterated over.
 * Uses std::begin internally to determine this.
 */
/* }}} */
template <typename T, typename = void>
struct is_iterable : std::false_type {};

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type can be iterated over.
 * Uses std::begin internally to determine this. Specialization for true
 * case.
 */
/* }}} */
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T&>())),
                                  decltype(std::end(std::declval<T&>()))>>
    : std::true_type {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_iterable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_iterable_v = ::supl::is_iterable<T>::value;

// is_iterator
/* {{{ doc */
/**
 * @brief Metafunction to determine if a type meets the minimum criteria
 * for being an iterator. Those being that the type is dereferenceable, and
 * pre-incrementable.
 */
/* }}} */
template <typename T, typename = void>
struct is_iterator : std::false_type {};

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type meets the minimum criteria
 * for being an iterator. Those being that the type is dereferenceable, and
 * pre-incrementable. Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_iterator<T, std::void_t<decltype(*std::declval<T>()),
                                  decltype(++std::declval<T&>())>>
    : std::true_type {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_iterator`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_iterator_v = ::supl::is_iterator<T>::value;

// is_bidirectional
/* {{{ doc */
/**
 * @brief Metafunction to determine if a type supports pre-increment and
 * pre-decrement operations. Intended for use with iterators.
 */
/* }}} */
template <typename T, typename = void>
struct is_bidirectional : std::false_type {};

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type supports pre-increment and
 * pre-decrement operations. Intended for use with iterators.
 * Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_bidirectional<T, std::void_t<decltype(++std::declval<T&>()),
                                       decltype(--std::declval<T&>())>>
    : std::true_type {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_bidirectional`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_bidirectional_v =
    ::supl::is_bidirectional<T>::value;

// is_random_access
/* {{{ doc */
/**
 * @brief Metafunction to determine if an iterator is a
 * random access iterator.
 */
/* }}} */
template <typename T, typename = void>
struct is_random_access : std::false_type {};

/* {{{ doc */
/**
 * @brief Metafunction to determine if an iterator is a
 * random access iterator. Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_random_access<
    T, std::void_t<decltype(std::declval<T&>() += std::declval<int>()),
                   decltype(std::declval<T&>() -= std::declval<int>()),
                   decltype(std::declval<T&>() + std::declval<int>()),
                   decltype(std::declval<T&>() - std::declval<int>()),
                   decltype(std::declval<T&>() - std::declval<T&>())>>
    : std::true_type {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_random_access`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_random_access_v =
    ::supl::is_random_access<T>::value;

// is_tuple
namespace impl {
/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::tuple.
 */
/* }}} */
template <typename... Ts>
struct is_tuple_impl : std::false_type {};

/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::tuple. Specialization for true case.
 */
/* }}} */
template <typename... Ts>
struct is_tuple_impl<std::tuple<Ts...>> : std::true_type {};
} // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type is a std::tuple.
 */
/* }}} */
template <typename T>
struct is_tuple : ::supl::impl::is_tuple_impl<std::decay_t<T>> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_tuple`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_tuple_v = ::supl::is_tuple<T>::value;

// is_pair
namespace impl {
/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::pair.
 */
/* }}} */
template <typename... Ts>
struct is_pair_impl : std::false_type {};

/* {{{ doc */
/**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::pair. Specialization for true case.
 */
/* }}} */
template <typename... Ts>
struct is_pair_impl<std::pair<Ts...>> : std::true_type {};
} // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type is a std::pair.
 */
/* }}} */
template <typename T>
struct is_pair : ::supl::impl::is_pair_impl<std::decay_t<T>> {};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pair`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_pair_v = ::supl::is_pair<T>::value;

// is_printable
template <typename T, typename = void>
struct is_printable : std::false_type {};

template <typename T>
struct is_printable<T, std::void_t<decltype(std::declval<std::ostream&>()
                                            << std::declval<T>())>>
    : std::true_type {};

template <typename T>
constexpr inline bool is_printable_v = ::supl::is_printable<T>::value;

} // namespace supl

#endif
