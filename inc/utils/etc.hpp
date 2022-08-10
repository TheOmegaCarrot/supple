#ifndef EHANC_UTILS_ETC_HPP
#define EHANC_UTILS_ETC_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <sstream>

#include "utils/algorithm.hpp"
#include "utils/iterators.hpp"
#include "utils/metaprogramming.hpp"

namespace ehanc {

/* {{{ doc */
/**
 * @brief Explicitly makes a copy of the parameter. Inspired by C++23's
 * `auto()` and `auto{}` syntax.
 *
 * @param t Value to be copied.
 *
 * @return Copy of `t`.
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto
explicit_copy(const T& t) noexcept(std::is_nothrow_constructible_v<T>) -> T
{
  return t;
}

template <typename T>
auto to_string([[maybe_unused]] const T& value) -> std::string
{
  std::stringstream out;
  if constexpr ( ::ehanc::is_tuple_v<T> ) {

    out << "( ";
    ::ehanc::for_each_in_tuple(
        value, [&out](const auto& i) { out << i << ", "; });
    out << "\b\b )";

  } else if constexpr ( ::ehanc::is_pair_v<T> ) {

    out << "( " << ::ehanc::to_string(value.first) << ", "
        << ::ehanc::to_string(value.second) << " )";

  } else if constexpr ( ::ehanc::is_iterable_v<T> ) {

    out << "[ ";
    std::for_each(value.cbegin(), value.cend(),
                  [&out](const auto& i) { out << i << ", "; });
    out << "\b\b ]";

  } else {

    out << value;
  }

  return out.str();
}

inline namespace literals {
inline namespace size_t_literal {

/* {{{ doc */
/**
 * @brief Makes it possible to declare a `std::size_t` literal.
 *
 * @param i Integer literal to be used as a `std::size_t`
 */
/* }}} */
[[nodiscard]] constexpr auto operator""_z(unsigned long long i) noexcept
    -> std::size_t
{
  return static_cast<std::size_t>(i);
}

} // namespace size_t_literal

} // namespace literals

} // namespace ehanc

#endif
