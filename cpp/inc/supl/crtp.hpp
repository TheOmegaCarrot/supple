#ifndef SUPPLEMENTARIES_CRTP_HPP
#define SUPPLEMENTARIES_CRTP_HPP

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
 *
 *   using supl::rel_ops<consumer>::operator!=;
 *   using supl::rel_ops<consumer>::operator<=;
 *   using supl::rel_ops<consumer>::operator>;
 *   using supl::rel_ops<consumer>::operator>=;
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

} // namespace supl

#endif
