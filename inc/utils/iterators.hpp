#ifndef EHANC_UTILS_ITERATORS_HPP
#define EHANC_UTILS_ITERATORS_HPP

#include <cstddef>
#include <iterator>
#include <utility>

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

/* {{{ doc */
/**
 * @brief Iterator class to wrap around iteration over a sequence of
 * values. Only stores current value.
 */
/* }}} */
template <typename value_type>
class sequence_iterator
{
private:
  value_type m_val;

public:
  /* {{{ doc */
  /**
   * @brief Must be constructed with a value.
   */
  /* }}} */
  sequence_iterator() = delete;

  /* {{{ doc */
  /**
   * @brief Must be constructed with a value.
   */
  /* }}} */
  constexpr sequence_iterator(value_type t) noexcept(
      noexcept(value_type(t)))
      : m_val{std::move(t)}
  {}

  sequence_iterator(const sequence_iterator&) = default;
  sequence_iterator(sequence_iterator&&)      = default;
  ~sequence_iterator()                        = default;
  sequence_iterator& operator=(const sequence_iterator&) = default;
  sequence_iterator& operator=(sequence_iterator&&) = default;

  /* {{{ doc */
  /**
   * @brief Pre-increment operator. Increments iterator to provide next
   * value.
   *
   * @return Reference to self after incrementing.
   */
  /* }}} */
  constexpr sequence_iterator& operator++() noexcept(noexcept(++m_val))
  {
    ++m_val;
    return *this;
  }

  /* {{{ doc */
  /**
   * @brief Post-increment operator. Increments iterator to provide next
   * value.
   *
   * @return Copy of self from before incrementing.
   */
  /* }}} */
  constexpr sequence_iterator operator++(int) noexcept(noexcept(++m_val))
  {
    sequence_iterator tmp{*this};
    this->operator++();
    return tmp;
  }

  /* {{{ doc */
  /**
   * @brief Pre-decrement operator. Increments iterator to provide previous
   * value.
   *
   * @return Reference to self after decrementing.
   */
  /* }}} */
  constexpr sequence_iterator& operator--() noexcept(noexcept(--m_val))
  {
    --m_val;
    return *this;
  }

  /* {{{ doc */
  /**
   * @brief Post-decrement operator. Increments iterator to provide
   * previous value.
   *
   * @return Copy of self from before decrementing.
   */
  /* }}} */
  constexpr sequence_iterator operator--(int) noexcept(noexcept(--m_val))
  {
    sequence_iterator tmp{*this};
    this->operator--();
    return tmp;
  }

  /* {{{ doc */
  /**
   * @brief Dereference operator. Provides read-only access to contained
   * value.
   *
   * @return Const reference to contained value.
   */
  /* }}} */
  constexpr const value_type& operator*() noexcept
  {
    return m_val;
  }

  /* {{{ doc */
  /**
   * @brief Equality comparison operator.
   */
  /* }}} */
  constexpr bool operator==(const sequence_iterator& rhs)
  {
    return m_val == rhs.m_val;
  }

  /* {{{ doc */
  /**
   * @brief Non-equality comparison operator.
   */
  /* }}} */
  constexpr bool operator!=(const sequence_iterator& rhs)
  {
    return m_val != rhs.m_val;
  }

  /* {{{ doc */
  /**
   * @brief Less-than comparison operator.
   */
  /* }}} */
  constexpr bool operator<(const sequence_iterator& rhs)
  {
    return m_val < rhs.m_val;
  }
};

/* {{{ doc */
/**
 * @brief Class which provides sequence iterators to allow for easy
 * iteration over a sequence of integers `[begin, end)` in contexts where a
 * container would be typical. (Range-for, etc)
 */
/* }}} */
template <typename value_type>
class sequence
{
private:
  value_type m_begin;
  value_type m_end;

public:
  /* {{{ doc */
  /**
   * @brief Must be constructed with values.
   */
  /* }}} */
  sequence() = delete;

  /* {{{ doc */
  /**
   * @brief Specify range over which to iterate. Will provide sequence
   * iterators which will provide the range `[begin, end)`.
   */
  /* }}} */
  sequence(value_type begin, value_type end)
      : m_begin(std::move(begin))
      , m_end(std::move(end))
  {}

  sequence(const sequence&) = default;
  sequence(sequence&&)      = default;
  ~sequence()               = default;
  sequence& operator=(const sequence&) = default;
  sequence& operator=(sequence&&) = default;

  /* {{{ doc */
  /**
   * @brief Provide begin sequence iterator.
   */
  /* }}} */
  constexpr sequence_iterator<value_type> begin() noexcept
  {
    return sequence_iterator(m_begin);
  }

  /* {{{ doc */
  /**
   * @brief Provide cbegin for compatibility.
   */
  /* }}} */
  constexpr sequence_iterator<value_type> cbegin() noexcept
  {
    return this->begin();
  }

  /* {{{ doc */
  /**
   * @brief Provide end sequence iterator.
   */
  /* }}} */
  constexpr sequence_iterator<value_type> end() noexcept
  {
    return sequence_iterator(m_end);
  }

  /* {{{ doc */
  /**
   * @brief Provide cbegin for compatibility.
   */
  /* }}} */
  constexpr sequence_iterator<value_type> cend() noexcept
  {
    return this->end();
  }
};

} // namespace ehanc

#endif
