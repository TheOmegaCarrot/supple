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

#include <utility>

namespace supl {

/* {{{ doc */
/**
 * @brief Re-implementation of `std::identity` from C++20
 *
 * @details A function object whose call operator takes one argument,
 * and forwards it back.
 */
/* }}} */
struct identity {
  template <typename T>
  constexpr auto operator()(T&& t) const noexcept -> decltype(auto)
  {
    return std::forward<T>(t);
  }
};

template <auto N, template <typename> typename OP>
struct op_bind_left {
  constexpr inline static auto op = OP {};

  template <typename T>
  constexpr auto operator()(T&& arg) const
    noexcept(noexcept(op(N, std::forward<T>(arg))))
  {
    return op(N, std::forward<T>(arg));
  }
};

template <template <typename> typename OP, auto N>
struct op_bind_right {
  constexpr inline static auto op = OP {};

  template <typename T>
  constexpr auto operator()(T&& arg) const
    noexcept(noexcept(op(std::forward<T>(arg), N)))
  {
    return op(std::forward<T>(arg), N);
  }
};

}  // namespace supl

#endif
