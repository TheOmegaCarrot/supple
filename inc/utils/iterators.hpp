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

  std::size_t distance{::ehanc::forward_distance(begin, end) - 1};
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
 * @brief Simple wrapper around increment operator. Modifies in-place.
 */
/* }}} */
template <typename T>
constexpr void increment(T& t)
{
  ++t;
}

/* {{{ doc */
/**
 * @brief Simple wrapper around decrement operator. Modifies in-place.
 */
/* }}} */
template <typename T>
constexpr void decrement(T& t)
{
  --t;
}

/* {{{ doc */
/**
 * @brief Iterator class to wrap around iteration over a sequence of
 * values. Only stores the current value.
 */
/* }}} */
template <typename T>
class sequence_iterator
{
public:

  using value_type = T;

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
  template <typename U, typename Incr = std::function<void(value_type&)>>
  constexpr sequence_iterator(U init,
                              Incr func = ::ehanc::increment<value_type>)
      // clang-format off
      noexcept(noexcept(U(init)) && noexcept(U(std::move(init))))
      // clang-format on
      : m_val{std::forward<U>(init)}
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
    this->operator++();
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
   * @brief Inequality comparison operator.
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
}; // class sequence_iterator

template <typename T, typename F>
sequence_iterator(T, F) -> sequence_iterator<std::decay_t<T>>;

template <typename T>
sequence_iterator(T) -> sequence_iterator<std::decay_t<T>>;

/* {{{ doc */
/**
 * @brief Class which provides sequence iterators to allow for easy
 * iteration over a sequence of integers `[begin, end)` in contexts where a
 * container would be typical (Range-for, etc). May be used safely as a
 * prvalue.
 */
/* }}} */
template <typename T>
class sequence
{
public:

  using value_type = T;

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
   *
   * @param begin Beginning value.
   *
   * @param end End value.
   *
   * @param func Increment function.
   */
  /* }}} */
  template <typename U, typename Incr = std::function<void(value_type&)>>
  sequence(U&& begin, U&& end,
           Incr&& func = ::ehanc::increment<value_type>)
      : m_begin(std::forward<U>(begin))
      , m_end(std::forward<U>(end))
      , m_inc(std::forward<Incr>(func))
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
  constexpr auto begin() noexcept -> ::ehanc::sequence_iterator<value_type>
  {
    return ::ehanc::sequence_iterator(m_begin, m_inc);
  }

  /* {{{ doc */
  /**
   * @brief Provide cbegin for compatibility.
   */
  /* }}} */
  constexpr auto cbegin() noexcept
      -> ::ehanc::sequence_iterator<value_type>
  {
    return this->begin();
  }

  /* {{{ doc */
  /**
   * @brief Provide end sequence iterator.
   */
  /* }}} */
  constexpr auto end() noexcept -> ::ehanc::sequence_iterator<value_type>
  {
    return ::ehanc::sequence_iterator(m_end, m_inc);
  }

  /* {{{ doc */
  /**
   * @brief Provide cbegin for compatibility.
   */
  /* }}} */
  constexpr auto cend() noexcept -> ::ehanc::sequence_iterator<value_type>
  {
    return this->end();
  }
}; // class sequence

template <typename T, typename F>
sequence(T, T, F) -> sequence<std::decay_t<T>>;

template <typename T>
sequence(T, T) -> sequence<std::decay_t<T>>;

/* {{{ doc */
/**
 * @brief Iterator class to wrap around iteration over the repeated
 * application of an argumentless stateful function. Only stores the
 * current value. Useful if you want to iterate over a generated sequence,
 * but have no need to store the entire sequence. The two below functions
 * should achieve the same results.
 *
 * @code
 *
 * void container() {
 *    std::array<int, MAXIMUM> arr;
 *    std::generate(arr.begin(), arr.end(), generator);
 *    std::for_each(arr.begin(), arr.end(), some_func);
 * }
 *
 * void generative() {
 *    std::for_each(ehanc::generative_iterator(generator),
 *    ehanc::generative_iterator(MAXIMUM), some_func);
 * }
 *
 * @endcode
 */
/* }}} */
template <typename T>
class generative_iterator
{
public:

  using value_type = T;

private:

  /* {{{ doc */
  /**
   * @brief Function which is used to generate the sequence of values.
   */
  /* }}} */
  std::function<value_type()> m_gen;

  /* {{{ doc */
  /**
   * @brief Current value in the sequence.
   */
  /* }}} */
  value_type m_val;

  /* {{{ doc */
  /**
   * @brief Current "index" of the generated sequence.
   */
  /* }}} */
  std::size_t m_count;

  /* {{{ doc */
  /**
   * @brief For use in sentinel-type generative_iterator. Maximum number
   * of iterations allowed.
   */
  /* }}} */
  std::size_t m_sentinel;

public:

  /* {{{ doc */
  /**
   * @brief Must be constructed with a function.
   */
  /* }}} */
  generative_iterator() = delete;

  /* {{{ doc */
  /**
   * @brief Must be constructed with a function.
   *
   * @param func Function to be used to generate a sequence.
   */
  /* }}} */
  template <typename Func = std::function<value_type()>,
            typename      = std::void_t<decltype(std::declval<Func>()())>>
  constexpr generative_iterator(Func&& func)
      : m_gen(std::forward<Func>(func))
      , m_val{m_gen()}
      , m_count{0}
      , m_sentinel{0}
  {}

  /* {{{ doc */
  /**
   * @brief Constructor for creating a sentinel iterator.
   * This overload requires explicit template parameter.
   *
   * @param sentinel Maximum number of iterations.
   */
  /* }}} */
  constexpr generative_iterator(std::size_t sentinel)
      : m_gen{}
      , m_val{}
      , m_count{0}
      , m_sentinel{sentinel}
  {}

  /* {{{ doc */
  /**
   * @brief Constructor for creating a sentinel iterator.
   * This overload requires explicit template parameter.
   *
   * @param sentinel Maximum number of iterations.
   */
  /* }}} */
  constexpr generative_iterator(int sentinel)
      : m_gen{}
      , m_val{}
      , m_count{}
      , m_sentinel{static_cast<std::size_t>(sentinel)}
  {}

  /* {{{ doc */
  /**
   * @brief Constructor for creating a sentinel iterator.
   *
   * @param dummy Generative iterator used for deducing the type.
   *
   * @param sentinel Maximum number of iterations.
   */
  /* }}} */
  constexpr generative_iterator(
      [[maybe_unused]] const generative_iterator& dummy,
      std::size_t sentinel)
      : m_gen{}
      , m_val{}
      , m_count{0}
      , m_sentinel{sentinel}
  {}

  /* {{{ doc */
  /**
   * @brief Constructor for creating a sentinel iterator.
   *
   * @param dummy Generative iterator used for deducing the type.
   *
   * @param sentinel Maximum number of iterations.
   */
  /* }}} */
  constexpr generative_iterator(
      [[maybe_unused]] const generative_iterator& dummy, int sentinel)
      : m_gen{}
      , m_val{}
      , m_count{}
      , m_sentinel{static_cast<std::size_t>(sentinel)}
  {}

  ~generative_iterator() = default;

  generative_iterator(const generative_iterator&) = default;

  generative_iterator(generative_iterator&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  generative_iterator& operator=(const generative_iterator&) = default;

  generative_iterator& operator=(generative_iterator&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  /* {{{ doc */
  /**
   * @brief Pre-increment operator. Increments iterator to provide next
   * value.
   *
   * @return Reference to self after incrementing.
   */
  /* }}} */
  constexpr generative_iterator& operator++() noexcept(noexcept(m_gen()))
  {
    m_val = m_gen();
    ++m_count;
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
  constexpr generative_iterator operator++(int) noexcept(noexcept(m_gen()))
  {
    generative_iterator tmp{*this};
    this->operator++();
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
   * @brief Equality comparison operator. Compares current number
   * of iterations against rhs's sentinel value.
   */
  /* }}} */
  constexpr bool operator==(const generative_iterator& rhs)
  {
    return m_count == rhs.m_sentinel;
  }

  /* {{{ doc */
  /**
   * @brief Inequality comparison operator. Compares current number
   * of iterations against rhs's sentinel value.
   */
  /* }}} */
  constexpr bool operator!=(const generative_iterator& rhs)
  {
    return m_count != rhs.m_sentinel;
  }

  /* {{{ doc */
  /**
   * @brief Less-than comparison operator.
   */
  /* }}} */
  constexpr bool operator<(const generative_iterator& rhs)
  {
    return m_count < rhs.m_sentinel;
  }
}; // class generative_iterator

template <typename F>
generative_iterator(F) -> generative_iterator<std::invoke_result_t<F>>;

template <typename G, typename S>
generative_iterator(G, S)
    -> generative_iterator<decltype(*std::declval<G>())>;

template <typename T>
class generative_sequence
{
public:

  using value_type = T;

private:

  std::function<value_type()> m_gen;
  std::size_t m_max;

public:

  /* {{{ doc */
  /**
   * @brief Must be constructed with values.
   */
  /* }}} */
  generative_sequence() = delete;

  /* {{{ doc */
  /**
   * @brief Specify a maximum number of iterations and
   * a generating function.
   *
   * @param max Maximum number of iterations
   *
   * @param func Generating function.
   */
  /* }}} */
  template <typename I, typename Func = std::function<value_type()>,
            typename = std::enable_if_t<std::is_integral_v<I>>,
            typename = std::void_t<decltype(std::declval<Func>()())>>
  generative_sequence(const I max, Func&& func)
      : m_gen(std::forward<Func>(func))
      , m_max(static_cast<std::size_t>(max))
  {}

  ~generative_sequence() = default;

  generative_sequence(const generative_sequence&) = default;

  generative_sequence(generative_sequence&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  generative_sequence& operator=(const generative_sequence&) = default;

  generative_sequence& operator=(generative_sequence&&) noexcept(noexcept(
      value_type(std::move(std::declval<value_type>())))) = default;

  constexpr auto begin() noexcept
      -> ::ehanc::generative_iterator<value_type>
  {
    return ::ehanc::generative_iterator(m_gen);
  }

  constexpr auto cbegin() noexcept
      -> ::ehanc::generative_iterator<value_type>
  {
    return this->begin();
  }

  constexpr auto end() noexcept -> ::ehanc::generative_iterator<value_type>
  {
    return ::ehanc::generative_iterator<value_type>(m_max);
  }

  constexpr auto cend() noexcept
      -> ::ehanc::generative_iterator<value_type>
  {
    return this->end();
  }

}; // class generative_sequence

template <typename T, typename F>
generative_sequence(T, F) -> generative_sequence<std::invoke_result_t<F>>;

} // namespace ehanc

#endif
