#ifndef EHANC_UTILS_ETC_HPP
#define EHANC_UTILS_ETC_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace ehanc {

/* {{{ doc */
/**
 * @brief Checks if all values in `cont` pass some predicate `func`.
 *
 * @tparam Container Container type which provides iterators.
 *
 * @tparam Predicate Unary function which takes the type contained in
 * `cont` and returns a bool.
 *
 * @param cont Container providing values to check.
 *
 * @param pred Condition to check all values in `cont` against.
 */
/* }}} */
template <typename Container, typename Predicate>
bool all_pass(const Container& cont,
              Predicate pred) noexcept(noexcept(pred(*std::cbegin(cont))))
{
  bool check{true};
  std::for_each(
      cont.cbegin(), cont.cend(),
      [&pred, &check](const auto& i) { check = check && pred(i); });
  return check;
}

/* {{{ doc */
/**
 * @brief Checks if all values in iterator range
 * pass some predicate `func`.
 *
 * @tparam Itr Iterator type.
 *
 * @tparam Predicate Unary function which takes the type pointed to by
 * the iterators and returns a bool.
 *
 * @param begin Begin iterator of range to check.
 *
 * @param end End iterator of range to check.
 *
 * @param pred Condition to check all values in cont against.
 */
/* }}} */
template <typename Itr, typename Predicate>
bool all_pass(Itr begin, const Itr end,
              Predicate pred) noexcept(noexcept(pred(*begin)))
{
  bool check{true};
  std::for_each(begin, end, [&pred, &check](const auto& i) {
    check = check && pred(i);
  });
  return check;
}

/* {{{ doc */
/**
 * @brief Base case of recursive overload.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @param cont Container whose size to query.
 *
 * @return Size of container `cont`.
 */
/* }}} */
template <typename Container>
constexpr std::size_t min_size(const Container& cont) noexcept
{
  return cont.size();
}

/* {{{ doc */
/**
 * @brief Returns size of smallest container provided.
 * Calls size method internally.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @tparam Containers Sequence of container types which all provide
 * a `.size()` member function. Types may be mixed, so long as all
 * `.size()` member functions return comparable types which can be
 * passed to `std::min`.
 *
 * @param cont First container.
 *
 * @param conts Pack of remaining containers.
 *
 * @return Size of smallest container.
 */
/* }}} */
template <typename Container, typename... Containers>
constexpr std::size_t min_size(const Container& cont,
                               const Containers&... conts) noexcept
{
  return std::min(::ehanc::min_size(cont), ::ehanc::min_size(conts...));
}

/* {{{ doc */
/**
 * @brief Explicitly makes a copy of the parameter. Inspired by C++23's
 * `auto()` and `auto{}` syntax.
 *
 * @param t Value to be copied.
 *
 * @return Copy of `t`.
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr T
explicit_copy(const T& t) noexcept(std::is_nothrow_constructible_v<T>)
{
  return t;
}

/* {{{ doc */
/**
 * @brief Metafunction to determine the type resulting from
 * addition of the parameter types.
 */
/* }}} */
template <typename... Ts>
struct sum_type {
  using type = decltype((... + std::declval<Ts>()));
};

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
struct peel_first {
  using type = void;
};

/* {{{ doc */
/**
 * @brief Metafunction to return first type in a pack. Empty pack
 * considered `void`. Specialization for non-zero length argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_first<First, Pack...> {
  using type = First;
};

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
struct peel_last {
  using type = void;
};

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for >1 argument.
 */
/* }}} */
template <typename First, typename... Pack>
struct peel_last<First, Pack...> {
  using type = typename ::ehanc::peel_last<Pack...>::type;
};

/* {{{ doc */
/**
 * @brief Metafunction to return last type in a pack. Empty pack considered
 * `void`. Specialization for base case.
 */
/* }}} */
template <typename Last>
struct peel_last<Last> {
  using type = Last;
};

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

namespace literals {
inline namespace size_t_literal {

/* {{{ doc */
/**
 * @brief Makes it possible to declare a `std::size_t` literal.
 *
 * @param i Integer literal to be used as a `std::size_t`
 */
/* }}} */
[[nodiscard]] constexpr auto operator""_z(unsigned long long i) noexcept
    -> std::size_t
{
  return static_cast<std::size_t>(i);
}

} // namespace size_t_literal

} // namespace literals

} // namespace ehanc

#endif
