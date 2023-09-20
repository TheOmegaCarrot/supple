/* {{{ doc */
/**
 * @file metaprogramming.hpp
 *
 * @brief This header contains numerous metafunctions.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_METAPROGRAMMING_HPP
#define SUPPLE_CORE_METAPROGRAMMING_HPP

#include <cstddef>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace supl {

///////////////////////////////////////////// type_identity

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

///////////////////////////////////////////// index_constant

template <std::size_t idx>
using index_constant = std::integral_constant<std::size_t, idx>;

///////////////////////////////////////////// index_pair

template <std::size_t t_first, std::size_t t_second>
struct index_pair {
  constexpr inline static std::size_t first {t_first};
  constexpr inline static std::size_t second {t_second};
};

///////////////////////////////////////////// sum_type

/* {{{ doc */
/**
 * @brief Metafunction to determine the type resulting from
 * addition of the parameter types.
 */
/* }}} */
template <typename... Ts>
using sum_type = type_identity<decltype((... + std::declval<Ts>()))>;

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `sum_type`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Ts>
using sum_type_t = typename sum_type<Ts...>::type;

///////////////////////////////////////////// remove_cvref

template <typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

///////////////////////////////////////////// is_type_in_pack

/* {{{ doc */
/**
 * @brief Metafunction to determine if type T is present in pack.
 */
/* }}} */
template <typename T, typename... Pack>
struct is_type_in_pack
    : std::bool_constant<(std::is_same_v<T, Pack> || ...)> { };

/* {{{ doc */
/**
 * @brief Metafunction to determine if type T is present in pack.
 * Specialization for size 0 pack.
 */
/* }}} */
template <typename T>
struct is_type_in_pack<T> : std::false_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_type_in_pack`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename... Pack>
constexpr inline bool is_type_in_pack_v =
  is_type_in_pack<T, Pack...>::value;

///////////////////////////////////////////// peel_first

/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Zero argument case.
 */
/* }}} */
template <typename... Pack>
struct peel_first : type_identity<void> { };

/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Specialization for non-zero length argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_first<First, Pack...> : type_identity<First> { };

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `peel_first`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
using peel_first_t = typename peel_first<Pack...>::type;

///////////////////////////////////////////// peel_last

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Zero argument case.
 */
/* }}} */
template <typename... Pack>
struct peel_last : type_identity<void> { };

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for >1 argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_last<First, Pack...>
    : type_identity<typename peel_last<Pack...>::type> { };

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for base case.
 */
/* }}} */
template <typename Last>
struct peel_last<Last> : type_identity<Last> { };

/* {{{ doc */
/**
 * @brief Helper alias template to make using the `peel_last`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
using peel_last_t = typename peel_last<Pack...>::type;

///////////////////////////////////////////// is_pack_uniform

/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are the same type. Empty pack considered uniform.
 */
/* }}} */
template <typename... Pack>
struct is_pack_uniform { };

template <typename T, typename... Pack>
struct is_pack_uniform<T, Pack...>
    : std::bool_constant<(std::is_same_v<T, Pack> && ...)> { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pack_uniform`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename... Pack>
constexpr inline bool is_pack_uniform_v = is_pack_uniform<Pack...>::value;

///////////////////////////////////////////// is_pack_only

/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are a specifc type. Returns false if pack is empty.
 */
/* }}} */
template <typename T, typename... Pack>
struct is_pack_only
    : std::conjunction<is_pack_uniform<Pack...>,
                       std::is_same<T, peel_first_t<Pack...>>> { };

/* {{{ doc */
/**
 * @brief Metafunction to determine if all types in a pack
 * are a specifc type. Returns false if pack is empty.
 * Specialization for empty pack.
 */
/* }}} */
template <typename T>
struct is_pack_only<T> : std::false_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pack_only`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename... Pack>
constexpr inline bool is_pack_only_v = is_pack_only<T, Pack...>::value;

///////////////////////////////////////////// is_iterable

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type can be iterated over.
 * Uses std::begin internally to determine this.
 */
/* }}} */
template <typename T, typename = void>
struct is_iterable : std::false_type { };

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type can be iterated over.
 * Uses std::begin internally to determine this. Specialization for true
 * case.
 */
/* }}} */
template <typename T>
struct is_iterable<T,
                   std::void_t<decltype(std::begin(std::declval<T&>())),
                               decltype(std::end(std::declval<T&>()))>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_iterable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_iterable_v = is_iterable<T>::value;

///////////////////////////////////////////// is_iterator

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type meets the minimum criteria
 * for being an iterator. Those being that the type is dereferenceable, and
 * pre-incrementable.
 */
/* }}} */
template <typename T, typename = void>
struct is_iterator : std::false_type { };

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type meets the minimum criteria
 * for being an iterator. Those being that the type is dereferenceable, and
 * pre-incrementable. Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_iterator<T,
                   std::void_t<decltype(*std::declval<T>()),
                               decltype(++std::declval<T&>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_iterator`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_iterator_v = is_iterator<T>::value;

///////////////////////////////////////////// is_forward

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type supports pre-increment and
 * pre-decrement operations. Intended for use with iterators.
 */
/* }}} */
template <typename T>
struct is_forward
    : std::is_base_of<
        std::forward_iterator_tag,
        typename std::iterator_traits<T>::iterator_category> { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_forward`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_forward_v = is_forward<T>::value;

///////////////////////////////////////////// is_bidirectional

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type supports pre-increment and
 * pre-decrement operations. Intended for use with iterators.
 */
/* }}} */
template <typename T>
struct is_bidirectional
    : std::is_base_of<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<T>::iterator_category> { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_bidirectional`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_bidirectional_v = is_bidirectional<T>::value;

///////////////////////////////////////////// is_random_access

/* {{{ doc */
/**
 * @brief Metafunction to determine if an iterator is a
 * random access iterator.
 */
/* }}} */
template <typename T>
struct is_random_access
    : std::is_base_of<
        std::random_access_iterator_tag,
        typename std::iterator_traits<T>::iterator_category> { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_random_access`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_random_access_v = is_random_access<T>::value;

///////////////////////////////////////////// is_tuple

namespace impl {
  /* {{{ doc */
  /**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::tuple.
 */
  /* }}} */
  template <typename... Ts>
  struct is_tuple_impl : std::false_type { };

  /* {{{ doc */
  /**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::tuple. Specialization for true case.
 */
  /* }}} */
  template <typename... Ts>
  struct is_tuple_impl<std::tuple<Ts...>> : std::true_type { };
}  // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type is a std::tuple.
 */
/* }}} */
template <typename T>
struct is_tuple : impl::is_tuple_impl<remove_cvref_t<T>> { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_tuple`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_tuple_v = is_tuple<T>::value;

///////////////////////////////////////////// is_pair

namespace impl {
  /* {{{ doc */
  /**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::pair.
 */
  /* }}} */
  template <typename T>
  struct is_pair_impl : std::false_type { };

  /* {{{ doc */
  /**
 * @brief Partial implementation of a metafunction to determine
 * if a type is a std::pair. Specialization for true case.
 */
  /* }}} */
  template <typename First, typename Second>
  struct is_pair_impl<std::pair<First, Second>> : std::true_type { };
}  // namespace impl

/* {{{ doc */
/**
 * @brief Metafunction to determine if a type is a std::pair.
 */
/* }}} */
template <typename T>
struct is_pair : impl::is_pair_impl<remove_cvref_t<T>> { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_pair`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_pair_v = is_pair<T>::value;

///////////////////////////////////////////// is_smart_pointer

template <typename T>
struct is_smart_pointer : std::false_type { };

template <typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type { };

template <typename T, typename U>
struct is_smart_pointer<std::unique_ptr<T, U>> : std::true_type { };

template <typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type { };

template <typename T>
struct is_smart_pointer<std::weak_ptr<T>> : std::true_type { };

template <typename T>
constexpr inline bool is_smart_pointer_v = is_smart_pointer<T>::value;

///////////////////////////////////////////// is_printable

/* {{{ doc */
/**
 * @brief Determines if `operator<<(std::ostream&, T)` is defined for a type `T`.
 */
/* }}} */
template <typename T, typename = void>
struct is_printable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if `operator<<(std::ostream&, T)` is defined for a type `T`.
 * Specialization for true case.
 */
/* }}} */
template <typename T>
struct is_printable<T,
                    std::void_t<decltype(std::declval<std::ostream&>()
                                         << std::declval<T>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `is_printable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
constexpr inline bool is_printable_v = is_printable<T>::value;

///////////////////////////////////////////// are_equality_comparable

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator==`.
 */
/* }}} */
template <typename T, typename U, typename = void>
struct are_equality_comparable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator==`.
 * Specialization for true case.
 */
/* }}} */
template <typename T, typename U>
struct are_equality_comparable<
  T,
  U,
  std::void_t<decltype(std::declval<T>() == std::declval<U>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `are_equality_comparable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename U>
constexpr inline bool are_equality_comparable_v =
  are_equality_comparable<T, U>::value;

///////////////////////////////////////////// are_inequality_comparable

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator==`.
 */
/* }}} */
template <typename T, typename U, typename = void>
struct are_inequality_comparable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator==`.
 * Specialization for true case.
 */
/* }}} */
template <typename T, typename U>
struct are_inequality_comparable<
  T,
  U,
  std::void_t<decltype(std::declval<T>() != std::declval<U>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `are_inequality_comparable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename U>
constexpr inline bool are_inequality_comparable_v =
  are_inequality_comparable<T, U>::value;

///////////////////////////////////////////// are_less_comparable

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator<`.
 */
/* }}} */
template <typename T, typename U, typename = void>
struct are_less_comparable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator<`.
 * Specialization for true case.
 */
/* }}} */
template <typename T, typename U>
struct are_less_comparable<
  T,
  U,
  std::void_t<decltype(std::declval<T>() < std::declval<U>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `are_less_comparable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename U>
constexpr inline bool are_less_comparable_v =
  are_less_comparable<T, U>::value;

///////////////////////////////////////////// are_less_eq_comparable

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator<=`.
 */
/* }}} */
template <typename T, typename U, typename = void>
struct are_less_eq_comparable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator<=`.
 * Specialization for true case.
 */
/* }}} */
template <typename T, typename U>
struct are_less_eq_comparable<
  T,
  U,
  std::void_t<decltype(std::declval<T>() <= std::declval<U>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `are_less_eq_comparable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename U>
constexpr inline bool are_less_eq_comparable_v =
  are_less_eq_comparable<T, U>::value;

///////////////////////////////////////////// are_less_comparable

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator>`.
 */
/* }}} */
template <typename T, typename U, typename = void>
struct are_greater_comparable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator>`.
 * Specialization for true case.
 */
/* }}} */
template <typename T, typename U>
struct are_greater_comparable<
  T,
  U,
  std::void_t<decltype(std::declval<T>() > std::declval<U>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `are_greater_comparable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename U>
constexpr inline bool are_greater_comparable_v =
  are_greater_comparable<T, U>::value;

///////////////////////////////////////////// are_greater_eq_comparable

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator>=`.
 */
/* }}} */
template <typename T, typename U, typename = void>
struct are_greater_eq_comparable : std::false_type { };

/* {{{ doc */
/**
 * @brief Determines if two types can be compared with `operator>=`.
 * Specialization for true case.
 */
/* }}} */
template <typename T, typename U>
struct are_greater_eq_comparable<
  T,
  U,
  std::void_t<decltype(std::declval<T>() >= std::declval<U>())>>
    : std::true_type { };

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `are_greater_eq_comparable`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T, typename U>
constexpr inline bool are_greater_eq_comparable_v =
  are_greater_eq_comparable<T, U>::value;

///////////////////////////////////////////// make_const_ref

/* {{{ doc */
/**
 * @brief Strips all cv-qualifiers and reference-ness,
 * and adds `const` and lvalue reference.
 *
 * ex. volatile int&& -> const int&
 */
/* }}} */
template <typename T>
struct make_const_ref
    : type_identity<
        std::add_lvalue_reference_t<std::add_const_t<remove_cvref_t<T>>>> {
};

/* {{{ doc */
/**
 * @brief Helper variable template to make using the `make_const_ref`
 * metafunction less verbose and cumbersome.
 */
/* }}} */
template <typename T>
using make_const_ref_t = typename make_const_ref<T>::type;

///////////////////////////////////////////// is_same_as

/* {{{ doc */
/**
 * @brief Provides a unary metafunction `is_same_as<T>::template func<U>`,
 * and a helper variable template `is_same_as<T>::template func_v<U>`.
 * The result of calling the inner `func` with type `U` provided by
 * `is_same_as<T>` is equivalent to calling `std::is_same<T, U>`.
 *
 * ex. `is_same_as<T>::template func_v<U> == std::is_same_v<T, U>`
 *
 * Indended to be used to produce a predicate for another metafunction.
 */
/* }}} */
template <typename T>
struct is_same_as {
  template <typename U>
  struct func : std::is_same<T, U> { };

  /* using func = std::is_same<T, U>; */

  template <typename U>
  constexpr inline static bool func_v = func<U>::value;
};

///////////////////////////////////////////// conjunction_compose

/* {{{ doc */
/**
 * @brief Provides a unary metafunction `conjunction_compose<Ts...>::template func<U>`,
 * and a helper variable template conjunction_compose<Ts...>::template func_v<U>.
 * The result of calling the inner `func` with type `U` provided by
 * `conjunction_compose<Ts...>`  is equivalent to calling
 * `std::conjunction<Ts<U>...>`.
 *
 * ex. `conjunction_compose<std::is_integral, std::is_const>::template func_v<const void>
 * == std::conjunction_v<std::is_integral<const void>, std::is_const<const void>>`
 *
 * Indended to be used to produce a predicate for another metafunction.
 */
/* }}} */
template <template <typename> typename... PREDS>
struct conjunction_compose {
  template <typename T>
  using func = std::conjunction<PREDS<T>...>;

  template <typename T>
  constexpr inline static bool func_v = func<T>::value;
};

///////////////////////////////////////////// disjunction_compose

/* {{{ doc */
/**
 * @brief Provides a unary metafunction `disjunction_compose<Ts...>::template func<U>`,
 * and a helper variable template disjunction_compose<Ts...>::template func_v<U>.
 * The result of calling the inner `func` with type `U` provided by
 * `disjunction_compose<Ts...>`  is equivalent to calling
 * `std::disjunction<Ts<U>...>`.
 *
 * ex. `disjunction_compose<std::is_integral, std::is_const>::template func_v<const void>
 * == std::disjunction_v<std::is_integral<const void>, std::is_const<const void>>`
 *
 * Indended to be used to produce a predicate for another metafunction.
 */
/* }}} */
template <template <typename> typename... PREDS>
struct disjunction_compose {
  template <typename T>
  using func = std::disjunction<PREDS<T>...>;

  template <typename T>
  constexpr inline static bool func_v = func<T>::value;
};

///////////////////////////////////////////// binary_partial_apply

/* {{{ doc */
/**
 * @brief Partially applies binary metafunction `FUNC`, passing
 * `First` as the first parameter of `FUNC`. Exposes an inner
 * unary metafunction `func` which passes `Second` as the
 * second parameter to `FUNC`.
 *
 * ex. `binary_partial_apply<std::is_same, int>::template func_v<int> == true`
 *
 * ex. `binary_partial_apply<std::is_same, int>::template func_v<void> == false`
 */
/* }}} */
template <template <typename, typename> typename FUNC, typename First>
struct binary_partial_apply {
  template <typename Second>
  using func = FUNC<First, Second>;

  template <typename Second>
  using func_t = typename func<Second>::type;

  template <typename Second>
  constexpr inline static decltype(func<Second>::value) func_v =
    func<Second>::value;
};

///////////////////////////////////////////// sequential_apply

/* {{{ doc */
/**
 * @brief Apply a series of type -> type metafunctions in one expression
 *
 * @details `FUNC` and `FUNC_PACK` must be unary metafunctions taking a single
 * type parameter. `FUNC`, and all elements of `FUNC_PACK` must expose
 * a `::type`
 * However, an exception is made for the last element of `FUNC_PACK`,
 * which is permitted to be a type -> value metafunction.
 *
 * In short: both of these are valid:
 *
 * sequential_compose_t<int, std::add_const, std::add_pointer> -> const int*
 *
 * sequential_compose_v<int, std::add_const, std::is_const> -> true
 */
/* }}} */
template <typename T,
          template <typename>
          typename FUNC,
          template <typename>
          typename... FUNC_PACK>
struct sequential_apply
    : sequential_apply<typename FUNC<T>::type, FUNC_PACK...> { };

template <typename T, template <typename> typename FUNC>
struct sequential_apply<T, FUNC> : FUNC<T> { };

template <typename T,
          template <typename>
          typename FUNC,
          template <typename>
          typename... FUNC_PACK>
using sequential_apply_t =
  typename sequential_apply<T, FUNC, FUNC_PACK...>::type;

template <typename T,
          template <typename>
          typename FUNC,
          template <typename>
          typename... FUNC_PACK>
constexpr inline auto sequential_apply_v =
  sequential_apply<T, FUNC, FUNC_PACK...>::value;

///////////////////////////////////////////// sequential_applicator

/* {{{ doc */
/**
 * @brief Provides a unary metafunction
 * `sequential_applicator<FUNCS...>::template func<T>`, which is equivalent
 * to calling `sequential_apply<T, FUNCS...>`.
 *
 * @details This is helpful to compose many metafunctions to
 * create a new metafunction to provide to another higher-order
 * metafunction which expects a unary type -> type metafunction.
 */
/* }}} */
template <template <typename> typename... FUNCS>
struct sequential_applicator {
  template <typename T>
  struct func : sequential_apply<T, FUNCS...> { };

  template <typename T>
  using func_t = typename func<T>::type;
};

}  // namespace supl

#endif
