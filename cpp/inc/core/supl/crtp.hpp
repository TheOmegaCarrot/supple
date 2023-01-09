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

#ifndef SUPPLE_CORE_CRTP_HPP
#define SUPPLE_CORE_CRTP_HPP

#include <sstream>
#include <string>

#include <supl/utility.hpp>

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
    return ! (rhs == (*static_cast<const T*>(this)));
  }

  constexpr auto operator<=(const T& rhs) const noexcept -> bool
  {
    return ((*static_cast<const T*>(this)) < rhs)
        || (rhs == (*static_cast<const T*>(this)));
  }

  constexpr auto operator>(const T& rhs) const noexcept -> bool
  {
    return ! this->operator<=(rhs);
  }

  constexpr auto operator>=(const T& rhs) const noexcept -> bool
  {
    return this->operator>(rhs) || ! this->operator!=(rhs);
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

namespace impl {

  template <typename T>
  void to_stream_for_add_ostream_impl(
    std::ostream& out,
    const T& value
  ) noexcept
  {
    const ostream_state_restorer restorer(out);

    out << std::boolalpha;

    if constexpr ( has_to_stream_v<T> ) {

      value.to_stream(out);

    } else if constexpr ( is_iterable_v<T> ) {

      impl::to_stream_iterable_impl(out, value);
    }
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Generate ostream insertion operator
 *
 * @details If a class either provides iterators, or has a `to_stream`
 * non-static const member function, CRTP inheriting from this
 * class will add a hidden friend ostream insertion operator.
 * If both are provided, the `to_stream` member function
 * is used.
 *
 * A valid `to_stream` member function must have the signature:
 * `any_return_type to_stream(std::ostream&) const`.
 * It is permitted to be marked `noexcept`, however, it must not be marked
 * `[[nodiscard]]`
 */
/* }}} */
template <typename T>
struct add_ostream {
  friend inline auto operator<<(std::ostream& out, const T& value) noexcept
    -> std::ostream&
  {
    impl::to_stream_for_add_ostream_impl(out, value);
    return out;
  }
};

}  // namespace supl

#endif
