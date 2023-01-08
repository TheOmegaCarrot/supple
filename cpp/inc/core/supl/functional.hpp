/* {{{ doc */
/**
 * @file functional.hpp
 *
 * @brief Functional utilities
 *
 * @details This header contains higher-order functions to
 * facilitate functional-like programming techniques,
 * excluding those which fit better into other headers
 * (algorithms, tuple algorithms, etc)
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_FUNCTIONAL_HPP
#define SUPPLE_CORE_FUNCTIONAL_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include "invoke.hpp"
#include "tuple_algo.hpp"

namespace supl {

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determiens if its argument is
 * equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto equal_to(T&& arg) noexcept
{
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg
         ) constexpr noexcept -> bool {
    return new_arg == parent_arg;
  };
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is not equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determiens if its argument is
 * not equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto not_equal_to(T&& arg) noexcept
{
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg
         ) constexpr noexcept -> bool {
    return new_arg != parent_arg;
  };
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is greater than `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determiens if its argument is
 * greater than `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto greater_than(T&& arg) noexcept
{
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg
         ) constexpr noexcept -> bool {
    return new_arg > parent_arg;
  };
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is greater than or equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determiens if its argument is
 * greater than or equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto greater_eq_than(T&& arg) noexcept
{
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg
         ) constexpr noexcept -> bool {
    return new_arg >= parent_arg;
  };
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is less than `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determiens if its argument is
 * less than `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto less_than(T&& arg) noexcept
{
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg
         ) constexpr noexcept -> bool {
    return new_arg < parent_arg;
  };
}

/* {{{ doc */
/**
 * @brief Returns a unary predicate which determines if
 * its argument is less than or equal to `arg`
 *
 * @tparam T Type to compare
 *
 * @param arg Right-hand-side of comparison
 *
 * @return Unary predicate which determiens if its argument is
 * less than or equal to `arg`
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto less_eq_than(T&& arg) noexcept
{
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg
         ) constexpr noexcept -> bool {
    return new_arg <= parent_arg;
  };
}

/* {{{ doc */
/**
 * @brief Create a unary predicate which determines if an argument
 * satisfies all of the provided predicates
 *
 * @details The returned predicate takes ownership of
 * the given predicates, do keep this in mind if
 * calling this function with nontrivial function objects.
 *
 * @param preds Pack of unary predicates
 *
 * @return Unary predicate equivalent to `(preds(arg) && ...)`,
 * where `arg` is the argument to the returned predicate.
 */
/* }}} */
template <typename... Preds>
[[nodiscard]] constexpr auto conjunction(Preds&&... preds) noexcept
{
  return [pred_tup {std::tuple<std::remove_reference_t<Preds>...> {
           std::forward<Preds>(preds)...}}](auto&& arg
         ) constexpr noexcept -> bool {
    return std::apply(
      [&arg](auto&&... inner_preds) constexpr noexcept(
        noexcept((inner_preds(std::forward<decltype(arg)>(arg)) && ...))
      ) { return (inner_preds(std::forward<decltype(arg)>(arg)) && ...); },
      pred_tup
    );
  };
}

/* {{{ doc */
/**
 * @brief Create a unary predicate which determines if an argument
 * satisfies any of the provided predicates
 *
 * @details The returned predicate takes ownership of
 * the given predicates, do keep this in mind if
 * calling this function with nontrivial function objects.
 *
 * @param preds Pack of unary predicates
 *
 * @return Unary predicate equivalent to `(preds(arg) || ...)`,
 * where `arg` is the argument to the returned predicate.
 */
/* }}} */
template <typename... Preds>
[[nodiscard]] constexpr auto disjunction(Preds&&... preds) noexcept
{
  return [pred_tup {std::tuple<std::remove_reference_t<Preds>...> {
           std::forward<Preds>(preds)...}}](auto&& arg
         ) constexpr noexcept -> bool {
    return std::apply(
      [&arg](auto&&... inner_preds) constexpr noexcept(
        noexcept((inner_preds(std::forward<decltype(arg)>(arg)) || ...))
      ) { return (inner_preds(std::forward<decltype(arg)>(arg)) || ...); },
      pred_tup
    );
  };
}

/* {{{ doc */
/**
 * @brief Logical negation of a predicate
 *
 * @details Yes, this is redundant to `std::not_fn`,
 * this exists for naming consistency with the remaining
 * `pred_*` functions.
 *
 * | pred | ret |
 * |:----:|:---:|
 * |  T   |  F  |
 * |  F   |  T  |
 */
/* }}} */
template <typename Pred>
[[nodiscard]] constexpr auto pred_not(Pred&& pred) noexcept
{
  return [inner_pred {std::forward<Pred>(pred)}](auto&& arg
         ) constexpr noexcept {
    return not inner_pred(std::forward<decltype(arg)>(arg));
  };
}

/* {{{ doc */
/**
 * @brief `conjunction`, but accepts exactly 2 predicates
 * to reduce overhead
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  F  |
 * | F  | T  |  F  |
 * | F  | F  |  F  |
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto
pred_and(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return [inner_pred1 {pred1},
          inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    return inner_pred1(std::forward<decltype(arg)>(arg))
        && inner_pred2(std::forward<decltype(arg)>(arg));
  };
}

/* {{{ doc */
/**
 * @brief `disjunction`, but accepts exactly 2 predicates
 * to reduce overhead
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  T  |
 * | F  | T  |  T  |
 * | F  | F  |  F  |
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto pred_or(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return [inner_pred1 {pred1},
          inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    return inner_pred1(std::forward<decltype(arg)>(arg))
        || inner_pred2(std::forward<decltype(arg)>(arg));
  };
}

/* {{{ doc */
/**
 * @brief Logical xor of two predicates
 *
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  F  |
 * | T  | F  |  T  |
 * | F  | T  |  T  |
 * | F  | F  |  F  |
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto
pred_xor(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return [inner_pred1 {pred1},
          inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    return (inner_pred1(std::forward<decltype(arg)>(arg))
            + inner_pred2(std::forward<decltype(arg)>(arg)))
        == 1;
  };
}

/* {{{ doc */
/**
 * @brief Logical implication of two predicates
 *
 * 
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  F  |
 * | F  | T  |  T  |
 * | F  | F  |  T  |
 * 
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto
pred_implies(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return [inner_pred1 {pred1},
          inner_pred2 {pred2}](auto&& arg) constexpr noexcept {
    if ( not inner_pred1(std::forward<decltype(arg)>(arg)) ) {
      return true;
    } else {
      return inner_pred2(std::forward<decltype(arg)>(arg));
    }
  };
}

/* {{{ doc */
/**
 * @brief Logical biconditional of two predicates
 *
 * 
 * | p1 | p2 | ret |
 * |:--:|:--:|:---:|
 * | T  | T  |  T  |
 * | T  | F  |  F  |
 * | F  | T  |  F  |
 * | F  | F  |  T  |
 * 
 */
/* }}} */
template <typename Pred1, typename Pred2>
[[nodiscard]] constexpr auto
pred_bicond(Pred1&& pred1, Pred2&& pred2) noexcept
{
  return pred_not(
    pred_xor(std::forward<Pred1>(pred1), std::forward<Pred2>(pred2))
  );
}

}  // namespace supl

#endif
