/* {{{ doc */
/**
 * @file iterators.hpp
 *
 * @details This header contains functions relating to iterators.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLEMENTARIES_ITERATORS_HPP
#define SUPPLEMENTARIES_ITERATORS_HPP

#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"

namespace supl {

/* {{{ doc */
/**
 * @brief Returns an iterator to the last element of the container.
 * In short, `++supl::last(container) == std::end(container)`
 *
 * @tparam Iterable Type which provides iterators.
 *
 * @param container Container which supports at least forward iterators.
 *
 * @return Iterator to the last element of the container.
 */
/* }}} */
template <typename Iterable>
[[nodiscard]] constexpr auto last(const Iterable& container) noexcept
    -> decltype(std::begin(container))
{
  auto begin {std::begin(container)};
  auto end {std::end(container)};

  if ( begin == end ) {
    return begin;
  }

  if constexpr ( is_bidirectional_v<decltype(end)> ) {
    return --end;
  } else {

    auto distance {
        static_cast<std::size_t>(std::distance(begin, end) - 1)};
    static_assert(std::is_same_v<decltype(distance), std::size_t>);

    for ( std::size_t i {0}; i != distance; ++i ) {
      ++begin;
    }

    return begin;
  }
}

/* {{{ doc */
/**
 * @brief Returns a const iterator to the last element of the container.
 * In short, `++supl::clast(container) == std::end(container)`
 *
 * @param container Container which supports at least forward iterators.
 *
 * @return Const iterator to the last element of the container.
 */
/* }}} */
template <typename Iterable>
[[nodiscard]] constexpr auto clast(const Iterable& container) noexcept
    -> decltype(std::cbegin(container))
{
  return static_cast<decltype(std::cbegin(container))>(last(container));
}

template <typename T>
struct is_iterator_tag : std::is_base_of<std::forward_iterator_tag, T> {};

template <typename T>
constexpr inline bool is_iterator_tag_v = is_iterator_tag<T>::value;

} // namespace supl

#endif
