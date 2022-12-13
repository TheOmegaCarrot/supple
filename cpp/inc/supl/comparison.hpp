#ifndef SUPPLEMENTARIES_COMPARISON_HPP
#define SUPPLEMENTARIES_COMPARISON_HPP

#include <functional>
#include <utility>

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

} // namespace supl

#endif
