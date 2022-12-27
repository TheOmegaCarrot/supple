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
struct is_iterator_tag : std::is_base_of<std::forward_iterator_tag, T> {};

template <typename T>
constexpr inline bool is_iterator_tag_v = is_iterator_tag<T>::value;

template <typename Value_Type>
class iterator
{
private:

  class Iterator_Concept
  {
  public:

    Iterator_Concept()                                           = default;
    Iterator_Concept(const Iterator_Concept&)                    = default;
    Iterator_Concept(Iterator_Concept&&) noexcept                = default;
    auto operator=(const Iterator_Concept&) -> Iterator_Concept& = default;
    auto operator=(Iterator_Concept&&) noexcept
        -> Iterator_Concept&    = default;
    virtual ~Iterator_Concept() = default;

    virtual void operator++() = 0;
    /* virtual auto operator++(int) -> iterator       = 0; */
    virtual void operator--() = 0;
    /* virtual auto operator--(int) -> iterator       = 0; */
    virtual auto operator*() -> Value_Type&  = 0;
    virtual auto operator->() -> Value_Type* = 0;
    /* virtual auto operator==(const iterator& rhs) const -> bool = 0; */
    /* virtual auto operator!=(const iterator& rhs) const -> bool = 0; */
    virtual auto iterator_impl_clone() const
        -> std::unique_ptr<Iterator_Concept> = 0;
  };

  template <typename T>
  class Iterator_Model : public Iterator_Concept
  {
  private:

    T m_erased;

  public:

    Iterator_Model()                                             = default;
    Iterator_Model(const Iterator_Model&)                        = default;
    Iterator_Model(Iterator_Model&&) noexcept                    = default;
    auto operator=(const Iterator_Model&) -> Iterator_Model&     = default;
    auto operator=(Iterator_Model&&) noexcept -> Iterator_Model& = default;
    virtual ~Iterator_Model()                                    = default;

    template <typename Type, typename = std::enable_if<!std::is_same_v<
                                 std::decay_t<Type>, Iterator_Model>>>
    explicit Iterator_Model(Type&& value)
        : m_erased {std::forward<Type>(value)}
    {}

    void operator++() override
    {
      ++m_erased;
    }

    /* auto operator++(int) -> iterator override */
    /* { */
    /*   ++m_erased; */
    /* } */

    void operator--() override
    {
      /* return iterator {--m_erased}; */
      --m_erased;
    }

    /* auto operator--(int) -> iterator override */
    /* { */
    /*   T tmp = m_erased; */
    /*   --m_erased; */
    /*   return iterator {tmp}; */
    /* } */

    auto operator*() -> Value_Type& override
    {
      return *m_erased;
    }

    auto operator->() -> Value_Type* override
    {
      return &*m_erased;
    }

    /* auto operator==(const iterator& rhs) const -> bool override */
    /* { */
    /*   return m_erased == rhs.m_value->m_erased; */
    /* } */

    /* auto operator!=(const iterator& rhs) const -> bool override */
    /* { */
    /*   return !this->operator==(rhs); */
    /* } */

    auto iterator_impl_clone() const
        -> std::unique_ptr<Iterator_Concept> override
    {
      return std::make_unique<Iterator_Model>(m_erased);
    }
  };

  std::unique_ptr<Iterator_Concept> m_value;

  template <typename T>
  explicit iterator(Iterator_Model<T>& src)
      : m_value {std::make_unique<Iterator_Model<T>>(src)}
  {}

public:

  iterator() = default;

  iterator(const iterator& src) noexcept
      : m_value {src.m_value->iterator_impl_clone()}
  {}

  iterator(iterator&&) noexcept = default;

  auto operator=(const iterator& rhs) -> iterator&
  {
    if ( this != &rhs ) {
      m_value = rhs.m_value->iterator_impl_clone();
    }
    return *this;
  }

  auto operator=(iterator&&) noexcept -> iterator& = default;
  ~iterator()                                      = default;

  template <typename T, typename = std::enable_if_t<
                            !std::is_same_v<std::decay_t<T>, iterator>>>
  explicit iterator(T&& value)
      : m_value(std::make_unique<Iterator_Model<std::decay_t<T>>>(
          std::forward<T>(value)))
  {}

  template <typename T, typename = std::enable_if_t<
                            !std::is_same_v<std::decay_t<T>, iterator>>>
  auto operator=(T&& rhs) -> iterator&
  {
    m_value = std::make_unique<Iterator_Model<std::decay_t<T>>>(
        std::forward<T>(rhs));
    return *this;
  }

  auto operator++() -> iterator&
  {
    m_value->operator++();
    return *this;
  }

  auto operator++(int) -> iterator
  {
    iterator tmp = *this;
    m_value->operator++();
    return tmp;
  }

  auto operator--() -> iterator&
  {
    m_value->operator--();
    return *this;
  }

  auto operator--(int) -> iterator
  {
    iterator tmp = *this;
    m_value->operator--();
    return tmp;
  }

  auto operator*() -> Value_Type&
  {
    return m_value->operator*();
  }

  auto operator->() -> Value_Type*
  {
    return m_value->operator->();
  }

  /* auto operator==(const iterator& rhs) const -> bool */
  /* { */
  /*   return this->m_value == rhs.m_value; */
  /* } */

  /* auto operator!=(const iterator& rhs) const -> bool */
  /* { */
  /*   return !this->operator==(rhs); */
  /* } */
};

template <typename T>
iterator(T) -> iterator<typename std::iterator_traits<T>::value_type>;

} // namespace supl

#endif
