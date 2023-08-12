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

}  // namespace supl

#endif
