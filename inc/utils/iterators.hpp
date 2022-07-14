#ifndef EHANC_UTILS_ITERATORS_HPP
#define EHANC_UTILS_ITERATORS_HPP

#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>

#include "utils/metaprogramming.hpp"

namespace ehanc {

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
  auto begin {std::begin(container)};
  auto end {std::end(container)};

  if ( begin == end ) {
    return begin;
  }

  if constexpr ( ::ehanc::is_bidirectional_v<decltype(end)> ) {
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
class sequence_iterator final
{
public:

  using value_type        = T;
  using difference_type   = std::ptrdiff_t;
  using reference         = const T&;
  using iterator_category = std::input_iterator_tag;

private:

  value_type m_val {};
  std::function<void(value_type&)> m_inc {};

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
  explicit constexpr sequence_iterator(
      U init, Incr func = ::ehanc::increment<value_type>)
      // clang-format off
      noexcept(std::conjunction_v<
               std::is_nothrow_constructible<T>,
               std::is_nothrow_move_constructible<T>>)
      // clang-format on
      : m_val {std::forward<U>(init)}
      , m_inc {std::forward<Incr>(func)}
  {}

  ~sequence_iterator() noexcept(
      std::is_nothrow_destructible_v<value_type>) = default;

  // the noexcept freaks out older gcc versions
  sequence_iterator(const sequence_iterator&) /* noexcept(
      std::is_nothrow_copy_constructible_v<value_type>) */
      = default;

  sequence_iterator(sequence_iterator&&) noexcept(
      std::is_nothrow_move_constructible_v<value_type>) = default;

  // the noexcept freaks out older gcc versions
  auto operator=(const sequence_iterator&) /* noexcept(
      std::is_nothrow_copy_assignable_v<value_type>) */
      -> sequence_iterator& = default;

  auto operator=(sequence_iterator&&) noexcept(
      std::is_nothrow_move_assignable_v<value_type>)
      -> sequence_iterator& = default;

  /* {{{ doc */
  /**
   * @brief Pre-increment operator. Increments iterator to provide next
   * value.
   *
   * @return Reference to self after incrementing.
   */
  /* }}} */
  constexpr auto operator++() noexcept(noexcept(m_inc(m_val)))
      -> sequence_iterator&
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
  constexpr auto operator++(int) noexcept(noexcept(++m_val))
      -> sequence_iterator
  {
    sequence_iterator tmp {*this};
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
  constexpr auto operator*() noexcept -> const value_type&
  {
    return m_val;
  }

  /* {{{ doc */
  /**
   * @brief Equality comparison operator.
   */
  /* }}} */
  constexpr auto operator==(const sequence_iterator& rhs) noexcept -> bool
  {
    return m_val == rhs.m_val;
  }

  /* {{{ doc */
  /**
   * @brief Inequality comparison operator.
   */
  /* }}} */
  constexpr auto operator!=(const sequence_iterator& rhs) noexcept -> bool
  {
    return m_val != rhs.m_val;
  }

  /* {{{ doc */
  /**
   * @brief Less-than comparison operator.
   */
  /* }}} */
  constexpr auto operator<(const sequence_iterator& rhs) noexcept -> bool
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
class sequence final
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
  explicit sequence(U&& begin, U&& end,
                    Incr&& func = ::ehanc::increment<value_type>)
      // clang-format off
                    noexcept(std::conjunction_v<
                             std::is_nothrow_constructible<T>,
                             std::is_nothrow_move_constructible<T>>)
      // clang-format on
      : m_begin(std::forward<U>(begin))
      , m_end(std::forward<U>(end))
      , m_inc(std::forward<Incr>(func))
  {}

  ~sequence() noexcept(std::is_nothrow_destructible_v<value_type>) =
      default;

  // the noexcept freaks out older gcc versions
  sequence(const sequence&) /* noexcept(
      std::is_nothrow_copy_constructible_v<value_type>) */
      = default;

  sequence(sequence&&) noexcept(
      std::is_nothrow_move_constructible_v<value_type>) = default;

  // the noexcept freaks out older gcc versions
  auto operator=(const sequence&) /* noexcept(
      std::is_nothrow_copy_assignable_v<value_type>) */
      -> sequence& = default;

  auto operator=(sequence&&) noexcept(
      std::is_nothrow_move_assignable_v<value_type>)
      -> sequence& = default;

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
class generative_iterator final
{
public:

  using value_type        = T;
  using difference_type   = std::ptrdiff_t;
  using reference         = const T&;
  using iterator_category = std::input_iterator_tag;

private:

  /* {{{ doc */
  /**
   * @brief Function which is used to generate the sequence of values.
   */
  /* }}} */
  std::function<value_type()> m_gen {};

  /* {{{ doc */
  /**
   * @brief Current value in the sequence.
   */
  /* }}} */
  value_type m_val {};

  /* {{{ doc */
  /**
   * @brief Current "index" of the generated sequence.
   */
  /* }}} */
  std::size_t m_count {0};

  /* {{{ doc */
  /**
   * @brief For use in sentinel-type generative_iterator. Maximum number
   * of iterations allowed.
   */
  /* }}} */
  std::size_t m_sentinel {0};

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
  template <typename Func,
            typename = std::enable_if_t<std::is_invocable_v<Func>>>
  explicit constexpr generative_iterator(Func&& func) noexcept(
      std::conjunction_v<std::is_nothrow_copy_constructible<Func>,
                         std::is_nothrow_move_constructible<Func>,
                         std::is_nothrow_constructible<value_type>>)
      : m_gen(std::forward<Func>(func))
      , m_val {m_gen()}

  {}

  template <typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
  explicit constexpr generative_iterator(const I sentinel) noexcept(
      std::is_nothrow_default_constructible_v<value_type>)
      : m_sentinel {static_cast<std::size_t>(sentinel)}
  {}

  template <typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
  explicit constexpr generative_iterator(
      [[maybe_unused]] const generative_iterator& dummy, const I sentinel)
      //clang-format off
      noexcept(std::is_nothrow_default_constructible_v<value_type>)
      //clang-format on
      : m_sentinel {static_cast<std::size_t>(sentinel)}
  {}

  ~generative_iterator() noexcept(
      std::is_nothrow_destructible_v<value_type>) = default;

  // the noexcept freaks out older gcc versions
  generative_iterator(const generative_iterator&) /* noexcept(
      std::is_nothrow_copy_constructible_v<value_type>) */
      = default;

  generative_iterator(generative_iterator&&) noexcept(
      std::is_nothrow_move_constructible_v<value_type>) = default;

  auto operator=(const generative_iterator&) noexcept(
      std::is_nothrow_copy_assignable_v<value_type>)
      -> generative_iterator& = default;

  auto operator=(generative_iterator&&) noexcept(
      std::is_nothrow_move_assignable_v<value_type>)
      -> generative_iterator& = default;

  /* {{{ doc */
  /**
   * @brief Pre-increment operator. Increments iterator to provide next
   * value.
   *
   * @return Reference to self after incrementing.
   */
  /* }}} */
  constexpr auto operator++() noexcept(noexcept(m_gen()))
      -> generative_iterator&
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
  constexpr auto operator++(int) noexcept(
      noexcept(m_gen())
      && std::is_nothrow_copy_constructible_v<decltype(*this)>)
      -> generative_iterator
  {
    generative_iterator tmp {*this};
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
  constexpr auto operator*() noexcept -> const value_type&
  {
    return m_val;
  }

  /* {{{ doc */
  /**
   * @brief Equality comparison operator. Compares current number
   * of iterations against rhs's sentinel value.
   */
  /* }}} */
  constexpr auto operator==(const generative_iterator& rhs) noexcept
      -> bool
  {
    return m_count == rhs.m_sentinel;
  }

  /* {{{ doc */
  /**
   * @brief Inequality comparison operator. Compares current number
   * of iterations against rhs's sentinel value.
   */
  /* }}} */
  constexpr auto operator!=(const generative_iterator& rhs) noexcept
      -> bool
  {
    return m_count != rhs.m_sentinel;
  }

  /* {{{ doc */
  /**
   * @brief Less-than comparison operator.
   */
  /* }}} */
  constexpr auto operator<(const generative_iterator& rhs) noexcept -> bool
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
class generative_sequence final
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
            typename = std::enable_if_t<std::is_invocable_v<Func>>>
  explicit generative_sequence(const I max, Func&& func)
      // clang-format off
      noexcept(std::conjunction_v<
               std::is_nothrow_constructible<T>,
               std::is_nothrow_move_constructible<T>>)
      // clang-format on
      : m_gen(std::forward<Func>(func))
      , m_max(static_cast<std::size_t>(max))
  {}

  ~generative_sequence() noexcept(
      std::is_nothrow_destructible_v<value_type>) = default;

  // the noexcept freaks out older gcc versions
  generative_sequence(const generative_sequence&) /* noexcept(
      std::is_nothrow_copy_constructible_v<value_type>) */
      = default;

  generative_sequence(generative_sequence&&) noexcept(
      std::is_nothrow_move_constructible_v<value_type>) = default;

  // the noexcept freaks out older gcc versions
  auto operator=(const generative_sequence&) /* noexcept(
      std::is_nothrow_copy_assignable_v<value_type>) */
      -> generative_sequence& = default;

  auto operator=(generative_sequence&&) noexcept(
      std::is_nothrow_move_assignable_v<value_type>)
      -> generative_sequence& = default;

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
