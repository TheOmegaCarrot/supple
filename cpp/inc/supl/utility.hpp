/* {{{ doc */
/**
 * @file utility.hpp
 *
 * @brief Uncategorized utilities
 *
 * @details This header contains various utilities
 * which do not make sense to put in other headers,
 * but did not warrant being placed in their own header.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public Licence
 */
/* }}} */

#ifndef SUPPLEMENTARIES_UTILITY_HPP
#define SUPPLEMENTARIES_UTILITY_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <type_traits>

#include "iterators.hpp"
#include "metaprogramming.hpp"
#include "tuple_algo.hpp"

namespace supl {

/* {{{ doc */
/**
 * @brief Explicitly makes a copy of the parameter. Inspired by C++23's
 * `auto()` and `auto{}` syntax.
 *
 * @pre Parameter must be copiable.
 * If this precondition is not satisfied, it is a compile-time error.
 *
 * @param t Value to be copied.
 *
 * @return Copy of `t`.
 */
/* }}} */
template <typename T>
[[nodiscard]] constexpr auto
explicit_copy(const T& t) noexcept(std::is_nothrow_constructible_v<T>) -> T
{
  return t;
}

///////////////////////////////////////////// to_stream and related

/* {{{ doc */
/**
 * @brief When passed a `std::ostream`, its `fmtflags` are saved.
 * When an object of this type goes out of scope,
 * the `ostream`'s `fmtflags` which were saved are restored.
 */
/* }}} */
class ostream_state_restorer
// Tested by functioning properly in to_stream
// as a function-local class
{
private:

  std::ostream& m_stream;
  std::ios_base::fmtflags m_flags;

public:

  explicit ostream_state_restorer(std::ostream& stream)
      : m_stream {stream}
      , m_flags {stream.flags()}
  {}

  ostream_state_restorer(const ostream_state_restorer&) = delete;
  ostream_state_restorer(ostream_state_restorer&&)      = delete;
  auto operator=(const ostream_state_restorer&)
      -> ostream_state_restorer& = delete;
  auto operator=(ostream_state_restorer&&)
      -> ostream_state_restorer& = delete;

  ~ostream_state_restorer()
  {
    m_stream.flags(m_flags);
  }
};

/* {{{ doc */
/**
 * @brief Determines if `T` has a `to_stream` const non-static member function
 */
/* }}} */
template <typename T, typename = void>
struct has_to_stream : std::false_type {};

template <typename T>
struct has_to_stream<
    T, std::void_t<decltype(std::declval<const T&>().to_stream(
           std::declval<std::ostream&>()))>> : std::true_type {};

template <typename T>
constexpr inline bool has_to_stream_v = has_to_stream<T>::value;

template <typename T, typename = void>
struct has_empty_member_function : std::false_type {};

template <typename T>
struct has_empty_member_function<
    T, std::void_t<decltype(std::declval<const T&>().empty())>>
    : std::true_type {};

template <typename T>
constexpr inline bool has_empty_member_function_v =
    has_empty_member_function<T>::value;

/* {{{ doc */
/**
 * @brief Determines if a type can be called with `supl::to_stream`
 */
/* }}} */
template <typename T>
struct is_to_stream_valid
    : std::disjunction<
          has_to_stream<remove_cvref_t<T>>,
          is_printable<remove_cvref_t<T>>, is_tuple<remove_cvref_t<T>>,
          is_pair<remove_cvref_t<T>>, is_iterable<remove_cvref_t<T>>> {};

template <typename T>
constexpr inline bool is_to_stream_valid_v = is_to_stream_valid<T>::value;

// forward declaration for the impl functions to recurse into
// to_stream
template <typename T>
void to_stream(std::ostream& out, const T& value) noexcept;

namespace impl {

template <typename T>
void to_stream_tuple_impl(std::ostream& out, const T& value) noexcept
{
  if constexpr ( std::tuple_size<T>::value > 1 ) {

    out << "( ";
    for_each_in_subtuple<0, std::tuple_size_v<T> - 1>(
        value, [&out](const auto& i) {
          to_stream(out, i);
          out << ", ";
        });
    to_stream(out, std::get<std::tuple_size_v<T> - 1>(value));
    out << " )";

  } else if constexpr ( std::tuple_size_v<T> == 1 ) {
    out << "( ";
    to_stream(out, std::get<0>(value));
    out << " )";
  } else if constexpr ( std::tuple_size_v<T> == 0 ) {
    out << "( )";
  }
}

template <typename T>
void to_stream_pair_impl(std::ostream& out, const T& value) noexcept
{
  out << "( ";
  to_stream(out, value.first);
  out << ", ";
  to_stream(out, value.second);
  out << " )";
}

template <typename T>
void to_stream_iterable_impl(std::ostream& out, const T& value) noexcept
{
  const bool is_empty {[&]() {
    if constexpr ( has_empty_member_function_v<T> ) {
      return value.empty();
    } else {
      return std::begin(value) == std::end(value);
    }
  }()}; // IILE

  if ( is_empty ) {
    out << "[ ]";
  } else {
    out << "[ ";
    std::for_each(std::begin(value), supl::last(value),
                  [&out](const auto& i) {
                    to_stream(out, i);
                    out << ", ";
                  });
    to_stream(out, *supl::last(value));
    out << " ]";
  }
}

} // namespace impl

/* {{{ doc */
/**
 * @brief A singular function to output many things to streams.
 *
 * @details Sets `std::ios::boolalpha` internally, and resets flags
 * before returning.
 *
 * @pre A parameter type `T` is valid if any of the below are true:
 * - `T::to_stream(std::ostream&)` is defined as a const non-static member function
 * - `operator<<(std::ostream&, const T&)` is defined
 * - `T` is a tuple or pair of only valid types
 * - `T` provides an iterator pair which dereference to a valid type
 * If this precondition is not satisfied, it is a compile-time error.
 *
 * @tparam T Type to be formatted to a stream
 *
 * @param out `ostream` to output to
 *
 * @param value Value to be formatted to a stream
 */
/* }}} */
template <typename T>
void to_stream(std::ostream& out, const T& value) noexcept
{
  static_assert(
      is_to_stream_valid_v<T>,
      "Attempting to call supl::to_stream with an unsupported type");

  ostream_state_restorer restorer(out);

  out << std::boolalpha;

  if constexpr ( has_to_stream_v<T> ) {

    value.to_stream(out);

  } else if constexpr ( is_printable_v<T> ) {

    out << value;

  } else if constexpr ( is_tuple_v<T> ) {

    impl::to_stream_tuple_impl(out, value);

  } else if constexpr ( is_pair_v<T> ) {

    impl::to_stream_pair_impl(out, value);

  } else if constexpr ( is_iterable_v<T> ) {

    impl::to_stream_iterable_impl(out, value);
  }
}

/* {{{ doc */
/**
 * @brief Allows for outputting many types to ostreams.
 *
 * @details Supports any type supported by `supl::to_stream`.
 * Makes no copies.
 * Safe to use as a prvalue.
 * Contains a reference to its constructor argument.
 * 
 * ex. `an_ostream << supl::stream_adapter(a_vector)` << '\n';
 */
/* }}} */
template <typename T>
class stream_adapter
{
private:

  const T& m_value;

public:

  explicit stream_adapter(const T& t)
      : m_value {t}
  {}

  friend inline auto operator<<(std::ostream& out,
                                const stream_adapter<T>& rhs) noexcept
      -> std::ostream&
  {
    to_stream(out, rhs.m_value);
    return out;
  }
};

/* {{{ doc */
/**
 * @brief A singular function to convert many things to strings.
 *
 * @pre A parameter type `T` is valid if any of the below are true:
 * - `operator<<(std::ostream&, T)` is defined
 * - `T` is a tuple or pair of only valid types
 * - `T` provides an iterator pair which dereference to a valid type
 * If this precondition is not satisfied, it is a compile-time error.
 *
 * @tparam T Type to be formatted to a string
 *
 * @param value Value to be formatted to a string
 *
 * @return String representation of `value`
 */
/* }}} */
template <typename T>
[[nodiscard]] auto to_string(const T& value) -> std::string
{
  std::stringstream out;
  to_stream(out, value);
  return out.str();
}

///////////////////////////////////////////// end to_stream and related

inline namespace literals {
inline namespace size_t_literal {

/* {{{ doc */
/**
 * @brief Makes it possible to declare a `std::size_t` literal.
 *
 * @param i Integer literal to be used as a `std::size_t`
 */
/* }}} */
// NOLINTNEXTLINE(google-runtime-int)
[[nodiscard]] constexpr auto operator""_z(unsigned long long i) noexcept
    -> std::size_t
{
  return static_cast<std::size_t>(i);
}

} // namespace size_t_literal

} // namespace literals

} // namespace supl

#endif
