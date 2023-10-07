#ifndef SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP
#define SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP

#include <optional>
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
template <typename... Legal_Params>
class named_params
{
public:

  static_assert(! tl::has_duplicates_v<tl::type_list<Legal_Params...>>,
                "Named parameter type list contains duplicate types");

  static_assert((std::is_same_v<Legal_Params, remove_cvref_t<Legal_Params>>
                 && ...),
                "Named parameter type list must be cv-unqualified and "
                "must not be references");

private:

  std::tuple<std::optional<Legal_Params>...> m_params;

public:

  template <typename... Passed_Params>
  explicit constexpr named_params(
    Passed_Params&&... passed_params) noexcept
  /* : m_params{ */
  /*   [inner_param_ptrs{std::tuple<remove_cvref_t<Passed_Params>*...>{std::addressof(passed_params)...}}]() { */
  // maybe I can do some wacky tuple manipulation
  // to save a copy or move of each parameter,
  // but really, prvalues of trivial types are expected to be
  // the typical arguments
  /*   } */
  /* } */
  {
    static_assert(! tl::has_duplicates_v<
                    tl::type_list<remove_cvref_t<Passed_Params>...>>,
                  "Passed parameter list contains duplicates");

    static_assert(
      tl::is_subset_v<tl::type_list<remove_cvref_t<Passed_Params>...>,
                      tl::type_list<Legal_Params...>>,
      "Invalid parameter: passed parameters are not a subset of legal "
      "parameters");

    // suboptimal, but correct storage of parameters

    std::tuple<std::optional<remove_cvref_t<Passed_Params>>...>
      unfortunate_temp {std::forward<Passed_Params>(passed_params)...};

    // Populate m_params by moving from unfortunate_temp
    // In C++17, std::optional only has constexpr assignment if RHS is an optional of the same type
    ((std::get<std::optional<remove_cvref_t<Passed_Params>>>(m_params) =
        std::move(std::get<std::optional<remove_cvref_t<Passed_Params>>>(
          unfortunate_temp))),
     ...);

    // I want to eliminate the above abomination,
    // though it is the simplest way I could see to populate m_params
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
    static_assert(
      tl::contains_v<T, tl::type_list<Legal_Params...>>,
      "Argument to was_passed() is not an element of the set of valid "
      "parameters as specified in template parameters to named_params");
    return std::get<std::optional<T>>(m_params).has_value();
  }

  /* {{{ doc */
  /**
   * @brief Unchecked get. Throws if parameter was not passed.
   */
  /* }}} */
  template <typename T>
  [[nodiscard]] constexpr auto get() const
  {
    if ( this->was_passed<T>() ) {
      return impl::unwrap_value_struct_or_pass_enum(
        std::get<std::optional<T>>(m_params).value());
    } else {
      throw missing_named_parameter_exception {};
    }
  }

  /* {{{ doc */
  /**
   * @brief Checked get with a fallback value to be used if no value is available.
   *
   * @param fallback Value to be returned if no value exists.
   */
  /* }}} */
  template <typename Param_Type,
            typename Fallback_Type,
            typename RET = std::remove_reference_t<
              decltype(impl::unwrap_value_struct_or_pass_enum(
                std::get<std::optional<Param_Type>>(m_params).value()))>>
  [[nodiscard]] constexpr auto
  get_or(Fallback_Type&& fallback) const noexcept -> RET
  {
    if ( this->was_passed<Param_Type>() ) {
      return impl::unwrap_value_struct_or_pass_enum(
        std::get<std::optional<Param_Type>>(m_params).value());
    } else {
      return static_cast<RET>(fallback);
    }
  }
};

}  // namespace supl

#endif
