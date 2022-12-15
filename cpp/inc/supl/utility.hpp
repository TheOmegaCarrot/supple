#ifndef SUPPLEMENTARIES_UTILITY_HPP
#define SUPPLEMENTARIES_UTILITY_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <sstream>

#include "metaprogramming.hpp"
#include "tuple_algo.hpp"

namespace supl {

/* {{{ doc */
/**
 * @brief Explicitly makes a copy of the parameter. Inspired by C++23's
 * `auto()` and `auto{}` syntax.
 *
 * @pre Parameter must be copiable.
 * If this precondition is not satisfied, it is a compile-time error.
 *
 * @param t Value to be copied.
 *
 * @return Copy of `t`.
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto
explicit_copy(const T& t) noexcept(std::is_nothrow_constructible_v<T>) -> T
{
  return t;
}

template <typename T>
void to_stream(std::ostream& out, const T& value) noexcept
{

  using decayT = remove_cvref_t<T>;

  static_assert(
      std::disjunction_v<is_printable<decayT>, is_tuple<decayT>,
                         is_pair<decayT>, is_iterable<decayT>>,
      "Attempting to call supl::to_string with an unsupported type");

  auto flags {out.flags()};
  out << std::boolalpha;

  if constexpr ( is_printable_v<T> ) {

    out << value;

  } else if constexpr ( is_tuple_v<T> ) {

    if constexpr ( std::tuple_size_v<T> == 0 ) {
      out << "( )";
      out.setf(flags);
      return;
    }

    out << "( ";
    for_each_in_tuple(value, [&out](const auto& i) {
      to_stream(out, i);
      out << ", ";
    });
    // Move "write head" 2 characters before the end, and overwrite from
    // there Much less jank way of removing trailing ", "
    out.seekp(-2, std::ios_base::end);
    out << " )";

  } else if constexpr ( is_pair_v<T> ) {

    out << "( ";
    to_stream(out, value.first);
    out << ", ";
    to_stream(out, value.second);
    out << " )";

  } else if constexpr ( is_iterable_v<T> ) {

    if ( value.empty() ) {
      out << "[ ]";
    } else {
      out << "[ ";
      std::for_each(std::begin(value), std::end(value),
                    [&out](const auto& i) {
                      to_stream(out, i);
                      out << ", ";
                    });
      out.seekp(-2, std::ios_base::end);
      out << " ]";
    }
  }

  out.setf(flags);
}

/* {{{ doc */
/**
 * @brief A singular function to convert many things to strings.
 *
 * @pre A parameter type `T` is valid if any of the below are true:
 * - `operator<<(std::ostream&, T)` is defined
 * - `T` is a tuple or pair of only valid types
 * - `T` provides an iterator pair which dereference to a valid type
 * If this precondition is not satisfied, it is a compile-time error.
 *
 * @tparam T Type to be formatted to a string
 *
 * @param value Value to be formatted to a string
 *
 * @return String representation of `value`
 */
/* }}} */
template <typename T>
[[nodiscard]] auto to_string(const T& value) -> std::string
{
  std::stringstream out;
  to_stream(out, value);
  return out.str();
}

inline namespace literals {
inline namespace size_t_literal {

/* {{{ doc */
/**
 * @brief Makes it possible to declare a `std::size_t` literal.
 *
 * @param i Integer literal to be used as a `std::size_t`
 */
/* }}} */
// NOLINTNEXTLINE(google-runtime-int)
[[nodiscard]] constexpr auto operator""_z(unsigned long long i) noexcept
    -> std::size_t
{
  return static_cast<std::size_t>(i);
}

} // namespace size_t_literal

} // namespace literals

} // namespace supl

#endif
