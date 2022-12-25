/* {{{ doc */
/**
 * @file crtp.hpp
 *
 * @brief Base classes to be inherited from
 * in a crtp manner
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public Licence
 */
/* }}} */

#ifndef SUPPLEMENTARIES_CRTP_HPP
#define SUPPLEMENTARIES_CRTP_HPP

#include <sstream>
#include <string>

namespace supl {

/* {{{ doc */
/**
 * @brief Call-site transparent relative comparison operations
 *
 * @details Inspired by `std::rel_ops`, but I found the standard
 * rel_ops too intrusive at the call site.
 * The provided relative operations are constexpr-capable.
 *
 * Sample use:
 *
 * ```
 * class consumer : rel_ops<consumer>
 * {
 *   // class stuff
 *
 *   constexpr bool operator==(const consumer& rhs) const noexcept
 *   {
 *    // correct implementation
 *   }
 *   constexpr bool operator<(const consumer& rhs) const noexcept
 *   {
 *    // correct implementation
 *   }
 * }
 * ```
 */
/* }}} */
template <typename T>
struct rel_ops {
  constexpr auto operator!=(const T& rhs) const noexcept -> bool
  {
    return !(rhs == (*static_cast<const T*>(this)));
  }

  constexpr auto operator<=(const T& rhs) const noexcept -> bool
  {
    return ((*static_cast<const T*>(this)) < rhs)
           || (rhs == (*static_cast<const T*>(this)));
  }

  constexpr auto operator>(const T& rhs) const noexcept -> bool
  {
    return !this->operator<=(rhs);
  }

  constexpr auto operator>=(const T& rhs) const noexcept -> bool
  {
    return this->operator>(rhs) || !this->operator!=(rhs);
  }
};

/* {{{ doc */
/**
 * @brief Add a `to_string` const member function
 *
 * @details Requires implementing `T::to_stream(std::ostream&) const`
 * as a non-static member function
 *
 * Sample use:
 *
 * ```
 * class consumer : supl::add_to_string<consumer>
 * {
 *   // class stuff
 *
 *   void to_stream(std::ostream& out) const noexcept
 *   {
 *     // implementation
 *   }
 * }
 * ```
 */
/* }}} */
template <typename T>
struct add_to_string {
  [[nodiscard]] auto to_string() const noexcept -> std::string
  {
    std::stringstream str;
    static_cast<const T*>(this)->to_stream(str);
    return str.str();
  }
};

} // namespace supl

#endif
