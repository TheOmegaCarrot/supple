/* {{{ doc */
/**
 * @file integer_sequence.hpp
 *
 * @brief This header contains various std::integer_sequence operations.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_INTEGER_SEQUENCE_HPP
#define SUPPLE_CORE_INTEGER_SEQUENCE_HPP

#include <cstddef>
#include <utility>

namespace supl {

template <typename T, T... seq>
struct integer_sequence {
  integer_sequence() = default;

  constexpr integer_sequence(std::integer_sequence<T, seq...>)
  { }

  constexpr inline static std::size_t size = sizeof...(seq);
};

template <typename T, T... seq>
integer_sequence(std::integer_sequence<T, seq...>)
  -> integer_sequence<T, seq...>;

template <typename lhs, typename rhs, lhs... lhs_seq, rhs... rhs_seq>
constexpr auto operator+(integer_sequence<lhs, lhs_seq...>,
                         integer_sequence<rhs, rhs_seq...>) noexcept
  -> integer_sequence<decltype(std::declval<lhs>() + std::declval<rhs>()),
                      (lhs_seq + rhs_seq)...>
{
  static_assert(
    sizeof...(lhs_seq) == sizeof...(rhs_seq),
    "Cannot do arithmetic on integer sequences of unequal length");
  return {};
}

}  // namespace supl

#endif
