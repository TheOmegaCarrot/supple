#ifndef EHANC_UTILS_ITERATORS_HPP
#define EHANC_UTILS_ITERATORS_HPP

#include <cstddef>
#include <iterator>

namespace ehanc {

/* {{{ doc */
/**
 * @brief Computes distance between two iterators.
 * It is assumed `begin` is before `end`, meaning that incrementing
 * `begin` will eventually yield an iterator equal to `end`; if this
 * is not the case, behavior is undefined.
 *
 * @tparam Itr Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @param begin Beginning iterator.
 *
 * @param end End iterator.
 *
 * @return Distance between begin and end. Distance being defined as
 * the number of times `begin` must be incremented to equal `end`.
 */
/* }}} */
template <typename Itr>
[[nodiscard]] constexpr auto forward_distance(Itr begin,
                                              const Itr end) noexcept
    -> std::size_t
{
  std::size_t count{0};
  while ( begin != end ) {
    ++begin;
    ++count;
  }
  return count;
}

/* {{{ doc */
/**
 * @brief Returns an iterator to the last element of the container.
 * In short, `++ehanc::last(container) == std::end(container)`
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
  auto begin{std::begin(container)};
  auto end{std::end(container)};

  if ( begin == end ) {
    return begin;
  }

  std::size_t distance{forward_distance(begin, end) - 1};
  for ( std::size_t i{0}; i != distance; ++i ) {
    ++begin;
  }

  return begin;
}

/* {{{ doc */
/**
 * @brief Returns a const iterator to the last element of the container.
 * In short, `++ehanc::clast(container) == std::end(container)`
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

} // namespace ehanc

#endif
