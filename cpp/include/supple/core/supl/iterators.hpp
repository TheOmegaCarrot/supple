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

#ifndef SUPPLE_CORE_ITERATORS_HPP
#define SUPPLE_CORE_ITERATORS_HPP

#include <cstddef>
#include <exception>
#include <iterator>
#include <memory>
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
struct is_iterator_tag : std::is_base_of<std::forward_iterator_tag, T> { };

template <typename T>
constexpr inline bool is_iterator_tag_v = is_iterator_tag<T>::value;

class bad_iterator_access : public std::exception
{
public:

  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return "Illegal access to null supl::iterator";
  }
};

/* {{{ doc */
/**
 * @brief Type erased wrapper for a bidirectional iterator
 *
 * @details This should work just like any bidirectional iterator.
 * Dereference, arrow operator, pre- and post-increment and decrement,
 * and equality comparison all work as expected.
 *
 * It is templated on the value_type of the erased iterator,
 * and cannot be reassigned to an iterator of a different value_type.
 * This is checked by a static_assert.
 *
 * At template instatiation time, this wrapper is either a const_iterator
 * or a non-const iterator.
 * This cannot be changed after the fact.
 * If it is a const_iterator, a non-const iterator may be assigned to it,
 * however it will remain a const_iterator.
 * If it is a non-const iterator, a const_iterator may not be assigned to it,
 * as that would violate const-correctness.
 * This is checked by a static_assert.
 *
 * Post-incement and decrement are especially inefficient, as those
 * operations require heap allocation.
 *
 * Equality comparison between two of these type erased iterators
 * which are contain iterators of different underlying type is guaranteed
 * to return false and not throw.
 *
 * This class is wildly inefficient in comparison to raw iterators,
 * as type erasure necessitates heap allocation.
 * This class would seldom be appropriate, and this was written entirely
 * as an excercise in implementing nontrivial type erasure.
 *
 * If the `supl::iterator` is null, any attempt to access
 * ( operator++(), operator++(int), operator--(), operator--(int),
 *   operator*(), operator->(), operator==(), operator!=() ),
 * will result in throwing a `supl::bad_iterator_access`.
 *
 * @tparam Value_Type Type being iterated over.
 * If non-const, this wrapper behaves as a non-const iterator.
 * If const, this wrapper behaves as a const iterator.
 * If CTAD is performed, the correct type will be deduced.
 *
 * `iterator<int>` is the result of initializing with `std::vector<int>::iterator`
 * `iterator<const int>` is the result of initializing with `std::vector<int>::const_iterator`
 */
/* }}} */
template <typename Value_Type>
class iterator
{
private:

  class Iterator_Concept
  {
  public:

    Iterator_Concept() noexcept = default;
    Iterator_Concept(const Iterator_Concept&) noexcept = default;
    Iterator_Concept(Iterator_Concept&&) noexcept = default;
    auto operator=(const Iterator_Concept&) noexcept
      -> Iterator_Concept& = default;
    auto operator=(Iterator_Concept&&) noexcept
      -> Iterator_Concept& = default;
    virtual ~Iterator_Concept() noexcept = default;

    virtual void operator++() noexcept = 0;
    virtual void operator--() noexcept = 0;
    virtual auto operator*() noexcept -> Value_Type& = 0;
    virtual auto operator->() noexcept -> Value_Type* = 0;
    virtual auto operator==(const iterator& rhs) const noexcept
      -> bool = 0;
    virtual auto operator!=(const iterator& rhs) const noexcept
      -> bool = 0;
    [[nodiscard]] virtual auto iterator_impl_clone() const noexcept
      -> std::unique_ptr<Iterator_Concept> = 0;
  };  // Iterator_Concept

  template <typename Erased_Iterator_Type>
  class Iterator_Model : public Iterator_Concept
  {
  private:

    Erased_Iterator_Type m_erased;

  public:

    Iterator_Model() noexcept = default;
    Iterator_Model(const Iterator_Model&) noexcept = default;
    Iterator_Model(Iterator_Model&&) noexcept = default;
    auto operator=(const Iterator_Model&) noexcept
      -> Iterator_Model& = default;
    auto operator=(Iterator_Model&&) noexcept -> Iterator_Model& = default;
    ~Iterator_Model() noexcept override = default;

    template <typename Type,
              typename = std::enable_if<
                ! std::is_same_v<std::decay_t<Type>, Iterator_Model>>>
    explicit Iterator_Model(Type&& value) noexcept
        : m_erased {std::forward<Type>(value)}
    { }

    void operator++() noexcept override
    {
      ++m_erased;
    }

    void operator--() noexcept override
    {
      --m_erased;
    }

    auto operator*() noexcept -> Value_Type& override
    {
      return *m_erased;
    }

    auto operator->() noexcept -> Value_Type* override
    {
      return &*m_erased;
    }

    auto operator==(const iterator& rhs) const noexcept -> bool override
    {
      if ( auto* rhs_cast {
             dynamic_cast<Iterator_Model<Erased_Iterator_Type>*>(
               rhs.m_value.get())};
           rhs_cast != nullptr ) {
        return this->m_erased == rhs_cast->m_erased;
      } else {
        return false;
      }
    }

    auto operator!=(const iterator& rhs) const noexcept -> bool override
    {
      return ! this->operator==(rhs);
    }

    [[nodiscard]] auto iterator_impl_clone() const noexcept
      -> std::unique_ptr<Iterator_Concept> override
    {
      return std::make_unique<Iterator_Model>(m_erased);
    }
  };  // Iterator_Model

  std::unique_ptr<Iterator_Concept> m_value;

  void p_throw_if_null() const
  {
    if ( ! m_value ) {
      throw bad_iterator_access {};
    }
  }

public:

  using value_type = std::remove_const_t<Value_Type>;
  using difference_type = std::ptrdiff_t;
  using pointer = Value_Type*;
  using reference = Value_Type&;
  using iterator_category = std::bidirectional_iterator_tag;

  iterator() noexcept = default;

  iterator(const iterator& src) noexcept
      : m_value {src.m_value->iterator_impl_clone()}
  { }

  iterator(iterator&&) noexcept = default;

  auto operator=(const iterator& rhs) noexcept -> iterator&
  {
    if ( this != &rhs ) {
      m_value = rhs.m_value->iterator_impl_clone();
    }
    return *this;
  }

  auto operator=(iterator&&) noexcept -> iterator& = default;
  ~iterator() = default;

  template <typename T,
            typename = std::enable_if_t<
              ! std::is_same_v<std::decay_t<T>, iterator>>>
  explicit iterator(T&& value) noexcept
      : m_value(std::make_unique<Iterator_Model<std::decay_t<T>>>(
        std::forward<T>(value)))
  { }

  template <typename T,
            typename = std::enable_if_t<
              ! std::is_same_v<std::decay_t<T>, iterator>>>
  auto operator=(T&& rhs) noexcept -> iterator&
  {

    // if behaving as non-const iterator,
    // disallow reassigning to a const iterator
    static_assert(
      std::conditional_t<
        // if const, reassign is always ok
        std::is_const_v<Value_Type>,
        std::true_type,
        // if non-const, ensure the rhs is a const_iterator
        std::conditional_t<
          // true if rhs is a const_iterator
          std::is_const_v<std::remove_reference_t<
            typename std::iterator_traits<T>::reference>>,
          std::false_type,
          std::true_type>>::value,
      "const iterator cannot be assigned to non-const iterator");

    // Ensure reassignment is only to an iterator of the same value_type
    static_assert(
      std::is_same_v<value_type,
                     typename std::iterator_traits<T>::value_type>,
      "Can only assign to iterator of the same value type");

    m_value = std::make_unique<Iterator_Model<std::decay_t<T>>>(
      std::forward<T>(rhs));
    return *this;
  }

  auto operator++() -> iterator&
  {
    this->p_throw_if_null();
    m_value->operator++();
    return *this;
  }

  auto operator++(int) -> iterator
  {
    this->p_throw_if_null();
    iterator tmp = *this;
    m_value->operator++();
    return tmp;
  }

  auto operator--() -> iterator&
  {
    this->p_throw_if_null();
    m_value->operator--();
    return *this;
  }

  auto operator--(int) -> iterator
  {
    this->p_throw_if_null();
    iterator tmp = *this;
    m_value->operator--();
    return tmp;
  }

  auto operator*() const -> Value_Type&
  {
    this->p_throw_if_null();
    return m_value->operator*();
  }

  auto operator->() const -> Value_Type*
  {
    this->p_throw_if_null();
    return m_value->operator->();
  }

  auto operator==(const iterator& rhs) const -> bool
  {
    this->p_throw_if_null();
    return m_value->operator==(rhs);
  }

  auto operator!=(const iterator& rhs) const -> bool
  {
    this->p_throw_if_null();
    return ! this->operator==(rhs);
  }

  /* {{{ doc */
  /**
   * @brief Determine if an iterator is held
   *
   * @return True if `iterator` is null, i.e. does not hold
   * an iterator. False if an iterator is held.
   */
  /* }}} */
  [[nodiscard]] auto is_null() const noexcept -> bool
  {
    return ! m_value;
  }
};

template <typename T>
iterator(T) -> iterator<
  std::remove_reference_t<typename std::iterator_traits<T>::reference>>;

}  // namespace supl

#endif
