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

#ifndef SUPPLE_CORE_UTILITY_HPP
#define SUPPLE_CORE_UTILITY_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

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

/* {{{ doc */
/**
 * @brief Like `std::tie`, but returns a tuple of const references
 *
 * @return Tuple of const references to arguments
 */
/* }}} */
template <typename... Args>
[[nodiscard]] constexpr auto ctie(const Args&... args) noexcept
  -> std::tuple<const Args&...>
{
  return {args...};
}

template <typename T>
class range_wrapper
{
private:

  T m_begin;
  T m_end;

public:

  constexpr explicit range_wrapper(T begin, T end)
      : m_begin {begin}
      , m_end {end}
  { }

  constexpr explicit range_wrapper(std::pair<T, T> itr_pair)
      : m_begin {std::move(itr_pair.first)}
      , m_end {std::move(itr_pair.second)}
  { }

  [[nodiscard]] constexpr auto begin() const -> T
  {
    return m_begin;
  }

  [[nodiscard]] constexpr auto cbegin() const -> T
  {
    return m_begin;
  }

  [[nodiscard]] constexpr auto end() const -> T
  {
    return m_end;
  }

  [[nodiscard]] constexpr auto cend() const -> T
  {
    return m_end;
  }
};

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
{
private:

  // NOLINTNEXTLINE(*ref*)
  std::ostream& m_stream;
  std::ios_base::fmtflags m_flags;

public:

  explicit ostream_state_restorer(std::ostream& stream)
      : m_stream {stream}
      , m_flags {stream.flags()}
  { }

  ostream_state_restorer(const ostream_state_restorer&) = delete;
  ostream_state_restorer(ostream_state_restorer&&) = delete;
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
struct has_to_stream : std::false_type { };

template <typename T>
struct has_to_stream<
  T,
  std::void_t<decltype(std::declval<const T&>().to_stream(
    std::declval<std::ostream&>()))>> : std::true_type { };

template <typename T>
constexpr inline bool has_to_stream_v = has_to_stream<T>::value;

template <typename T, typename = void>
struct has_empty_member_function : std::false_type { };

template <typename T>
struct has_empty_member_function<
  T,
  std::void_t<decltype(std::declval<const T&>().empty())>>
    : std::true_type { };

template <typename T>
constexpr inline bool has_empty_member_function_v =
  has_empty_member_function<T>::value;

namespace impl {
  template <typename T>
  struct is_std_monostate_impl : std::false_type { };

  template <>
  struct is_std_monostate_impl<std::monostate> : std::true_type { };
}  // namespace impl

template <typename T>
struct is_std_monostate
    : impl::is_std_monostate_impl<remove_cvref_t<T>> { };

template <typename T>
constexpr inline bool is_std_monostate_v = is_std_monostate<T>::value;

namespace impl {
  template <typename T>
  struct is_variant_impl : std::false_type { };

  template <typename... Ts>
  struct is_variant_impl<std::variant<Ts...>> : std::true_type { };
}  // namespace impl

template <typename T>
struct is_variant : impl::is_variant_impl<remove_cvref_t<T>> { };

template <typename T>
constexpr inline bool is_variant_v = is_variant<T>::value;

namespace impl {
  template <typename T>
  struct is_optional_impl : std::false_type { };

  template <typename T>
  struct is_optional_impl<std::optional<T>> : std::true_type { };
}  // namespace impl

template <typename T>
struct is_optional : impl::is_optional_impl<remove_cvref_t<T>> { };

template <typename T>
constexpr inline bool is_optional_v = is_optional<T>::value;

/* {{{ doc */
/**
 * @brief Determines if a type can be called with `supl::to_stream`
 */
/* }}} */
template <typename T>
struct is_to_stream_valid
    : std::disjunction<has_to_stream<remove_cvref_t<T>>,
                       is_printable<remove_cvref_t<T>>,
                       is_tuple<remove_cvref_t<T>>,
                       is_pair<remove_cvref_t<T>>,
                       is_iterable<remove_cvref_t<T>>,
                       is_std_monostate<remove_cvref_t<T>>,
                       is_variant<remove_cvref_t<T>>,
                       is_optional<remove_cvref_t<T>>> { };

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
    if constexpr ( std::tuple_size_v<T> > 1 ) {

      out << "( ";
      tuple::for_each_in_subtuple<0, std::tuple_size_v<T> - 1>(
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
    }()};  // IILE

    if ( is_empty ) {
      out << "[ ]";
    } else {
      out << "[ ";
      std::for_each(
        std::begin(value), supl::last(value), [&out](const auto& i) {
          to_stream(out, i);
          out << ", ";
        });
      to_stream(out, *supl::last(value));
      out << " ]";
    }
  }

  template <typename... Ts>
  void to_stream_variant_impl(std::ostream& out,
                              const std::variant<Ts...>& variant) noexcept
  {
    try {
      std::visit(
        [&out](const auto& alternative) {
          to_stream(out, alternative);
        },
        variant);
    } catch ( std::bad_variant_access& ) {
      out << "<valueless_by_exception>";
    }
  }

  template <typename T>
  void to_stream_optional_impl(std::ostream& out,
                               const T& optional) noexcept
  {
    if ( optional.has_value() ) {
      to_stream(out, optional.value());
    } else {
      to_stream(out, "<empty optional>");
    }
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief A singular function to output many things to streams.
 *
 * @details Sets `std::ios::boolalpha` internally, and resets flags
 * before returning.
 * The case of an empty container is handled.
 * The case of an empty tuple is handled.
 * The case of a variant which is valueless_by_exception is handled.
 *
 * @details `to_stream` supports being specialized.
 * Specializations must be marked `noexcept`,
 * and must return `void`.
 * After specializing `to_stream` for type `T`,
 * `to_string` and `stream_adapter` also support `T`.
 * Of course, be aware of the potential foot gun of
 * visibility of your specialization,
 * and beware of ODR violations.
 *
 * Example:
 * ```
 * struct an_example { };
 *
 * namespace supl {
 * template <>
 * void to_stream<an_example>(std::ostream& out,
 *                                  const an_example&) noexcept
 * {
 *   out << "<an_example>";
 * }
 * } // namespace supl
 *
 * void foo()
 * {
 *    an_example example{};
 *    supl::to_stream(std::cout, example);
 *    std::cout << supl::stream_adapter(example);
 *    std::string example_string {supl::to_string(example)};
 * }
 * ```
 *
 * @pre A parameter type `T` is valid if any of the below are true:
 * - `T::to_stream(std::ostream&)` is defined as a const member function
 * - `operator<<(std::ostream&, const T&)` is defined
 * - `T` is a tuple or pair of only valid types
 * - `T` provides an iterator pair which dereference to a valid type
 * - `T` is `std::monostate`
 * - `T` is an `std::variant` where every possible alternative is a valid type
 * - `T` is an `std::optional` of a valid type
 * - `to_stream` has been specialized for T
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

  const ostream_state_restorer restorer(out);

  out << std::boolalpha;

  if constexpr ( has_to_stream_v<T> ) {

    value.to_stream(out);

  } else if constexpr ( is_printable_v<T> ) {

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    out << value;

  } else if constexpr ( is_tuple_v<T> ) {

    impl::to_stream_tuple_impl(out, value);

  } else if constexpr ( is_pair_v<T> ) {

    impl::to_stream_pair_impl(out, value);

  } else if constexpr ( is_iterable_v<T> ) {

    impl::to_stream_iterable_impl(out, value);

  } else if constexpr ( is_std_monostate_v<T> ) {

    out << "<std::monostate>";

  } else if constexpr ( is_variant_v<T> ) {

    impl::to_stream_variant_impl(out, value);

  } else if constexpr ( is_optional_v<T> ) {

    impl::to_stream_optional_impl(out, value);
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
 * ex. `an_ostream << supl::stream_adapter(a_vector)`;
 */
/* }}} */
template <typename T>
class stream_adapter
{
private:

  // NOLINTNEXTLINE(*ref*)
  const T& m_value;

public:

  explicit stream_adapter(const T& t)
      : m_value {t}
  { }

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
    [[nodiscard]] constexpr auto
    operator""_z(unsigned long long i) noexcept -> std::size_t
    {
      return static_cast<std::size_t>(i);
    }

  }  // namespace size_t_literal

  inline namespace ptrdiff_t_literal {

    /* {{{ doc */
    /**
 * @brief Makes it possible to declare a `std::ptrdiff_t` literal.
 *
 * @param i Integer literal to be used as a `std::ptrdiff_t`
 */
    /* }}} */
    [[nodiscard]] constexpr auto
    operator""_pd(unsigned long long i) noexcept -> std::ptrdiff_t
    {
      return static_cast<std::ptrdiff_t>(i);
    }

  }  // namespace ptrdiff_t_literal

}  // namespace literals

}  // namespace supl

#endif
