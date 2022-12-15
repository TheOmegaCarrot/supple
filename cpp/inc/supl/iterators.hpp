#ifndef SUPPLEMENTARIES_ITERATORS_HPP
#define SUPPLEMENTARIES_ITERATORS_HPP

#include <cstddef>
#include <functional>
#include <iterator>
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

/* {{{ doc */
/**
 * @brief Container-like class which wraps a sequence of values.
 * The range is [begin, end]. Note this is not a half-open range.
 * Values are created on-the-fly by the iterators.
 *
 * @tparam T Type of the sequence. Must support pre-increment and equality comparison.
 * It is strongly advised that `T` be cheap to copy.
 */
/* }}} */
template <typename T>
struct iota {

  const T begin_value;
  const T end_value;

  /* {{{ iterator_base */
  template <bool is_const>
  struct iterator_base {

    using value_type        = std::conditional_t<is_const, const T, T>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = std::conditional_t<is_const, const T*, T*>;
    using reference         = std::conditional_t<is_const, const T&, T&>;
    using iterator_category = std::bidirectional_iterator_tag;

    value_type value;

    constexpr auto operator++() noexcept -> iterator_base&
    {
      ++value;
      return *this;
    }

    [[nodiscard]] constexpr auto operator++(int) noexcept -> iterator_base
    {
      iterator_base copy {value};
      this->operator++();
      return copy;
    }

    constexpr auto operator--() noexcept -> iterator_base&
    {
      --value;
      return *this;
    }

    [[nodiscard]] constexpr auto operator--(int) noexcept -> iterator_base
    {
      iterator_base copy {value};
      this->operator--();
      return copy;
    }

    [[nodiscard]] constexpr auto operator*() noexcept -> reference
    {
      return value;
    }

    [[nodiscard]] constexpr auto
    operator==(const iterator_base& rhs) noexcept -> bool
    {
      return this->value == rhs.value;
    }

    [[nodiscard]] constexpr auto
    operator!=(const iterator_base& rhs) noexcept -> bool
    {
      return this->value != rhs.value;
    }

    [[nodiscard]] constexpr auto operator->() noexcept -> pointer
    {
      return &value;
    }
  };

  /* }}} */

  using iterator       = iterator_base<false>;
  using const_iterator = iterator_base<true>;

  [[nodiscard]] constexpr auto begin() -> iterator
  {
    return iterator {begin_value};
  }

  [[nodiscard]] constexpr auto begin() const -> iterator
  {
    return const_iterator {begin_value};
  }

  [[nodiscard]] constexpr auto cbegin() const -> iterator
  {
    return const_iterator {begin_value};
  }

  [[nodiscard]] constexpr auto end() -> iterator
  {
    return iterator {end_value + 1};
  }

  [[nodiscard]] constexpr auto end() const -> iterator
  {
    return const_iterator {end_value + 1};
  }

  [[nodiscard]] constexpr auto cend() const -> iterator
  {
    return const_iterator {end_value + 1};
  }
};

} // namespace supl

#endif
