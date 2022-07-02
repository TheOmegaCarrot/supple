#ifndef EHANC_UTILS_ITERATORS_HPP
#define EHANC_UTILS_ITERATORS_HPP

#include <cstddef>
#include <functional>
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

template <typename T>
constexpr void increment(T& t)
{
  ++t;
}

template <typename T>
constexpr void decrement(T& t)
{
  --t;
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
  std::function<void(value_type&)> m_inc;

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
  template <typename T, typename Incr = std::function<void(value_type&)>>
  constexpr sequence_iterator(
      T init,
      Incr func = ::ehanc::increment<
          value_type>) noexcept(noexcept(T(init)) && noexcept(T(std::
                                                                    move(
                                                                        init))))
      : m_val{std::forward<T>(init)}
      , m_inc{std::forward<Incr>(func)}
  {}

  ~sequence_iterator() = default;

  sequence_iterator(const sequence_iterator&) = default;

  sequence_iterator(sequence_iterator&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  sequence_iterator& operator=(const sequence_iterator&) = default;

  sequence_iterator& operator=(sequence_iterator&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  /* {{{ doc */
  /**
   * @brief Pre-increment operator. Increments iterator to provide next
   * value.
   *
   * @return Reference to self after incrementing.
   */
  /* }}} */
  constexpr sequence_iterator&
  operator++() noexcept(noexcept(m_inc(m_val)))
  {
    m_inc(m_val);
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
    m_inc(m_val);
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

template <typename T, typename F>
sequence_iterator(T, F) -> sequence_iterator<std::decay_t<T>>;

template <typename T>
sequence_iterator(T) -> sequence_iterator<std::decay_t<T>>;

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
  std::function<void(value_type&)> m_inc;

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
  template <typename T, typename Incr = std::function<void(value_type)>>
  sequence(T&& begin, T&& end, Incr func = ::ehanc::increment<value_type>)
      : m_begin(std::forward<T>(begin))
      , m_end(std::forward<T>(end))
      , m_inc{std::forward<Incr>(func)}
  {}

  ~sequence() = default;

  sequence(const sequence&) = default;

  sequence(sequence&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  sequence& operator=(const sequence&) = default;

  sequence& operator=(sequence&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

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

template <typename T, typename F>
sequence(T, T, F) -> sequence<std::decay_t<T>>;

template <typename T>
sequence(T, T) -> sequence<std::decay_t<T>>;

} // namespace ehanc

#endif
