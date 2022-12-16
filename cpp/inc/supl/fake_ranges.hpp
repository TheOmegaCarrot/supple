/* {{{ doc */
/**
 * @file fake_ranges.hpp
 *
 * @brief Fake ranges
 *
 * @details This header contains algorithms which take a whole container
 * type, and call the algorithm of the same name, calling `begin`
 * and `end` on the container. Inspired by C++20 ranges,
 * though that has far more capabilities, and thus this is dubbed
 * "fake ranges"
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLEMENTARIES_FAKE_RANGES_HPP
#define SUPPLEMENTARIES_FAKE_RANGES_HPP

// Still a work in progress
// The goal is to have every STL algorithm and `supl` algorithm
// have a `fr` counterpart

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <utility>

#include "algorithm.hpp"

namespace supl::fr {

template <typename Container, typename Pred>
auto all_of(Container&& container, Pred&& pred) noexcept(
    noexcept(std::all_of(std::begin(container), std::end(container),
                         std::forward<Pred>(pred)))) -> bool
{
  return std::all_of(std::begin(container), std::end(container),
                     std::forward<Pred>(pred));
}

template <typename Container, typename Pred>
auto any_of(Container&& container, Pred&& pred) noexcept(
    noexcept(std::any_of(std::begin(container), std::end(container),
                         std::forward<Pred>(pred)))) -> bool
{
  return std::any_of(std::begin(container), std::end(container),
                     std::forward<Pred>(pred));
}

template <typename Container, typename Pred>
auto none_of(Container&& container, Pred&& pred) noexcept(
    noexcept(std::none_of(std::begin(container), std::end(container),
                          std::forward<Pred>(pred)))) -> bool
{
  return std::none_of(std::begin(container), std::end(container),
                      std::forward<Pred>(pred));
}

template <typename Container, typename Func>
auto for_each(Container&& container, Func&& func) noexcept(
    noexcept(std::for_each(std::begin(container), std::end(container),
                           std::forward<Func>(func)))) -> Func
{
  return std::for_each(std::begin(container), std::end(container),
                       std::forward<Func>(func));
}

template <typename Container, typename Size, typename Func>
auto for_each_n(Container&& container, Size n, Func&& func) noexcept(
    noexcept(std::for_each_n(std::begin(container), std::end(container), n,
                             std::forward<Func>(func)))) -> Func
{
  return std::for_each_n(std::begin(container), std::end(container), n,
                         std::forward<Func>(func));
}

template <typename Container, typename T>
auto count(Container&& container, const T& value) noexcept(noexcept(
    std::count(std::begin(container), std::end(container), value))) ->
    typename std::iterator_traits<
        decltype(std::begin(container))>::difference_type
{
  return std::count(std::begin(container), std::end(container), value);
}

template <typename Container, typename Pred>
auto count_if(Container&& container, Pred&& pred) noexcept(
    noexcept(std::count(std::begin(container), std::end(container), pred)))
    -> typename std::iterator_traits<
        decltype(std::begin(container))>::difference_type
{
  return std::count_if(std::begin(container), std::end(container),
                       std::forward<Pred>(pred));
}

/* {{{ doc */
/**
 * @brief Container-like class which wraps a sequence of values.
 * The range is [begin, end).
 * Values are created on-the-fly by the iterators.
 * Safe to use as a prvalue.
 * Usable at compile-time.
 * Iterators can be created directly.
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
    return iterator {end_value};
  }

  [[nodiscard]] constexpr auto end() const -> iterator
  {
    return const_iterator {end_value};
  }

  [[nodiscard]] constexpr auto cend() const -> iterator
  {
    return const_iterator {end_value};
  }
};

} // namespace supl::fr

#endif
