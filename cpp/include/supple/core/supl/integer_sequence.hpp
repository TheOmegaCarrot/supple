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

#define integer_sequence_binary_operation(OP)                             \
  template <typename lhs, typename rhs, lhs... lhs_seq, rhs... rhs_seq>   \
  constexpr auto operator OP(                                             \
    ::supl::integer_sequence<lhs, lhs_seq...>,                            \
    ::supl::integer_sequence<rhs, rhs_seq...>) noexcept                   \
    -> ::supl::integer_sequence<decltype(std::declval<lhs>()              \
                                           OP std::declval<rhs>()),       \
                                (lhs_seq OP rhs_seq)...>                  \
  {                                                                       \
    static_assert(                                                        \
      sizeof...(lhs_seq) == sizeof...(rhs_seq),                           \
      "Cannot perform operation on integer sequences of unequal length"); \
    return {};                                                            \
  }

// clang-format off
integer_sequence_binary_operation(+)
integer_sequence_binary_operation(-)
integer_sequence_binary_operation(*)
integer_sequence_binary_operation(/)
integer_sequence_binary_operation(%)
integer_sequence_binary_operation(&)
integer_sequence_binary_operation(|)
integer_sequence_binary_operation(^)
integer_sequence_binary_operation(>)
integer_sequence_binary_operation(<)
integer_sequence_binary_operation(>=)
integer_sequence_binary_operation(<=)
integer_sequence_binary_operation(==)
integer_sequence_binary_operation(!=)
// clang-format on

#undef integer_sequence_binary_operation

#define integer_sequence_unary_operation(OP)                              \
  template <typename T, T... seq>                                         \
  constexpr auto operator OP(                                             \
    ::supl::integer_sequence<T, seq...>) noexcept                         \
    -> ::supl::integer_sequence<decltype(OP std::declval<T>()),           \
                                (OP seq)...>                              \
  {                                                                       \
    return {};                                                            \
  }

  // clang-format off
integer_sequence_unary_operation(+)
integer_sequence_unary_operation(-)
integer_sequence_unary_operation(!)
integer_sequence_unary_operation(~)
// clang-format on

#undef integer_sequence_unary_operation

}  // namespace supl

#endif
