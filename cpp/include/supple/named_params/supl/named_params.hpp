#ifndef SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP
#define SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP

#include <tuple>
/* #include <memory> */

#include <supl/metaprogramming.hpp>
#include <supl/type_list.hpp>

namespace supl {

/* {{{ doc */
/**
 * @brief Determines if type `T` appears in pack `Pack`.
 * Removes cv-qualification and removes references from types before comparison.
 */
/* }}} */
template <typename T, typename... Pack>
struct in_param_list : is_type_in_pack<T, remove_cvref_t<Pack>...> { };

template <typename T, typename... Pack>
constexpr inline bool in_param_list_v = in_param_list<T, Pack...>::value;

class missing_named_parameter_exception final : public std::exception
{
  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return "Attempt to access missing named parameter";
  }
};

namespace impl {

  template <typename T, typename = void>
  struct has_value_member : std::false_type { };

  template <typename T>
  struct has_value_member<T,
                          std::void_t<decltype(std::declval<T>().value)>>
      : std::true_type { };

  template <typename T>
  constexpr inline bool has_value_member_v = has_value_member<T>::value;

  template <typename T>
  [[nodiscard]] constexpr auto
  unwrap_value_struct_or_pass_enum(T&& arg) noexcept
  {
    static_assert(std::is_enum_v<std::remove_reference_t<T>>
                    || has_value_member_v<std::remove_reference_t<T>>,
                  "Named parameter type must be an enum or an object with "
                  "a public member 'value'");

    if constexpr ( std::is_enum_v<std::remove_reference_t<T>> ) {
      return arg;
    } else if constexpr ( has_value_member_v<
                            std::remove_reference_t<T>> ) {
      return arg.value;
    }
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Named parameters wrapper object.
 * Type template parameters define the legal set of argument types.
 */
/* }}} */
template <typename... Params>
class named_params
{
public:
private:

  std::tuple<Params...> m_params;

public:

  template <typename... Legal_Set, typename... Passed_Params>
  explicit constexpr named_params(
    tl::type_list<Legal_Set...>,
    Passed_Params&&... passed_params) noexcept
      : m_params {std::forward<Passed_Params>(passed_params)...}
  {

    static_assert(! tl::has_duplicates_v<tl::type_list<Legal_Set...>>,
                  "Named parameter type list contains duplicate types");

    static_assert(
      (std::is_same_v<Legal_Set, remove_cvref_t<Legal_Set>> && ...),
      "Named parameter type list Legal_Set must be cv-unqualified and "
      "must not be references");

    static_assert(! tl::has_duplicates_v<
                    tl::type_list<remove_cvref_t<Passed_Params>...>>,
                  "Passed parameter list contains duplicates");

    static_assert(
      tl::is_subset_v<tl::type_list<remove_cvref_t<Passed_Params>...>,
                      tl::type_list<Legal_Set...>>,
      "Invalid parameter: passed parameters are not a subset of legal "
      "parameters");
  }

  /* {{{ doc */
  /**
   * @brief Determines if parameter type `T` was passed as an argument.
   * May perform a runtime check, but is simple to use.
   * For a guaranteed compile-time check, use `supl::in_param_list`.
   */
  /* }}} */
  template <typename T>
  [[nodiscard]] constexpr auto was_passed() const noexcept -> bool
  {
    return tl::contains_v<T, tl::type_list<Params...>>;
  }

  /* {{{ doc */
  /**
   * @brief Unchecked get. Compile-time error if parameter was not passed.
   */
  /* }}} */
  template <typename T>
  [[nodiscard]] constexpr auto get() const
  {
    static_assert(tl::contains_v<T, tl::type_list<Params...>>,
                  "No parameter of that type");

    return impl::unwrap_value_struct_or_pass_enum(std::get<T>(m_params));
  }

  /* {{{ doc */
  /**
   * @brief Checked get with a fallback value to be used if no value is available.
   *
   * @param fallback Value to be returned if no value exists.
   */
  /* }}} */
  template <typename Param_Type, typename Fallback_Type>
  [[nodiscard]] constexpr auto
  get_or(Fallback_Type&& fallback) const noexcept
  {
    if constexpr ( tl::contains_v<Param_Type, tl::type_list<Params...>> ) {
      return impl::unwrap_value_struct_or_pass_enum(
        std::get<Param_Type>(m_params));
    } else {
      return static_cast<
        remove_cvref_t<decltype(impl::unwrap_value_struct_or_pass_enum(
          std::declval<Param_Type>()))>>(fallback);
    }
  }
};

template <typename... Legal_Set, typename... Passed_Params>
named_params(tl::type_list<Legal_Set...>, Passed_Params...)
  -> named_params<remove_cvref_t<Passed_Params>...>;

}  // namespace supl

#endif
