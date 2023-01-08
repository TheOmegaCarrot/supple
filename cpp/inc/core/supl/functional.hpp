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
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg) -> bool {
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
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg) -> bool {
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
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg) -> bool {
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
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg) -> bool {
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
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg) -> bool {
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
  return [parent_arg = std::forward<T>(arg)](const auto& new_arg) -> bool {
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
[[nodiscard]] constexpr auto unary_conjunction(Preds&&... preds) noexcept
{
  return [pred_tup {std::tuple<std::remove_reference_t<Preds>...> {
           std::forward<Preds>(preds)...}}](auto&& arg) {
    return tuple::call_as_pack(pred_tup, [&arg](auto&&... inner_preds) {
      return (inner_preds(std::forward<decltype(arg)>(arg)) && ...);
    });
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
[[nodiscard]] constexpr auto unary_disjunction(Preds&&... preds) noexcept
{
  return [pred_tup {std::tuple<std::remove_reference_t<Preds>...> {
           std::forward<Preds>(preds)...}}](auto&& arg) {
    return tuple::call_as_pack(pred_tup, [&arg](auto&&... inner_preds) {
      return (inner_preds(std::forward<decltype(arg)>(arg)) || ...);
    });
  };
}

}  // namespace supl

#endif
