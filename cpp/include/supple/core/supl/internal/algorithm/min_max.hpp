#ifndef SUPPLE_CORE_INTERNAL_MIN_MAX_HPP
#define SUPPLE_CORE_INTERNAL_MIN_MAX_HPP

#include <algorithm>
#include <utility>

namespace supl {
template <typename T>
constexpr auto min(T&& head) noexcept
{
  return std::forward<T>(head);
}

/* {{{ doc */
/**
   * @brief Variadic min
   *
   * @return Minimum argument, as determined by `std::min`
   */
/* }}} */
template <typename T, typename... Ts>
constexpr auto min(T&& head, Ts&&... tail) noexcept
{
  return std::min(std::forward<T>(head),
                  ::supl::min(std::forward<Ts>(tail)...));
}

template <typename T>
constexpr auto max(T&& head) noexcept
{
  return std::forward<T>(head);
}

/* {{{ doc */
/**
   * @brief Variadic max
   *
   * @return Maximum argument, as determined by `std::min`
   */
/* }}} */
template <typename T, typename... Ts>
constexpr auto max(T&& head, Ts&&... tail) noexcept
{
  return std::max(std::forward<T>(head),
                  ::supl::max(std::forward<Ts>(tail)...));
}
}  // namespace supl

#endif
