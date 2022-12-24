#ifndef SUPPLEMENTARIES_CRTP_HPP
#define SUPPLEMENTARIES_CRTP_HPP

namespace supl {

template <typename T>
struct rel_ops {
  auto operator!=(const T& rhs) const noexcept -> bool
  {
    return !(rhs == (*static_cast<const T*>(this)));
  }

  auto operator<=(const T& rhs) const noexcept -> bool
  {
    return ((*static_cast<const T*>(this)) < rhs)
           || (rhs == (*static_cast<const T*>(this)));
  }

  auto operator>(const T& rhs) const noexcept -> bool
  {
    return !this->operator<=(rhs);
  }

  auto operator>=(const T& rhs) const noexcept -> bool
  {
    return this->operator>(rhs) || !this->operator!=(rhs);
  }
};

} // namespace supl

#endif
