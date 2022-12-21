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

#ifndef SUPPLEMENTARIES_FUNCTIONAL_HPP
#define SUPPLEMENTARIES_FUNCTIONAL_HPP

#include <functional>
#include <memory>
#include <type_traits>
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

namespace impl {

template <typename Invocable, typename Object, typename... Args>
constexpr auto invoke_member_pointer(
    Invocable&& invocable, Object&& object,
    Args&&... args) noexcept(std::is_nothrow_invocable_v<Invocable,
                                                         Args...>)
    -> decltype(auto)
{
  if constexpr ( std::is_member_function_pointer_v<Invocable> ) {
    if constexpr ( std::is_pointer_v<Object> ) {

      return (std::forward<Object>(object)->*invocable)(
          std::forward<Args>(args)...);
    } else {
      return (std::forward<Object>(object)
              .*invocable)(std::forward<Args>(args)...);
    }
  }

  else {
    // member object pointer
    return (std::forward<Object>(object)->*invocable)();
    if constexpr ( std::is_pointer_v<Object> ) {

      return (std::forward<Object>(object))->*invocable;
    } else {
      return (std::forward<Object>(object)).*invocable;
    }
  }
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Constexpr equivalant to `std::invoke`
 *
 * @details If this function differs in any way from `std::invoke` other
 * than being usable in a constexpr context, that is a bug.
 *
 * @bug Does not work directly with smart pointers
 * when using pointer-to-member.
 *
 */
/* }}} */
template <typename Invocable, typename... Args>
constexpr auto invoke(Invocable&& invocable, Args&&... args) noexcept(
    std::is_nothrow_invocable_v<Invocable, Args...>) -> decltype(auto)
{
  if constexpr ( std::is_member_pointer_v<Invocable> ) {
    return impl::invoke_member_pointer(std::forward<Invocable>(invocable),
                                       std::forward<Args>(args)...);
  } else {
    return invocable(std::forward<Args>(args)...);
  }
}

} // namespace supl

#endif
