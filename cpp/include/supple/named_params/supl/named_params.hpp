#ifndef SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP
#define SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP

#include <optional>
#include <tuple>

#include <supl/metaprogramming.hpp>
#include <supl/type_list.hpp>

namespace supl {

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
  // how do I initialize m_params????
  // delegating constructor + pack deduction??
  {
    static_assert(! tl::has_duplicates_v<
                    tl::type_list<remove_cvref_t<Passed_Params>...>>,
                  "Passed parameter list contains duplicates");

    static_assert(
      tl::is_subset_v<tl::type_list<remove_cvref_t<Passed_Params>...>,
                      tl::type_list<Legal_Params...>>,
      "Invalid parameter: passed parameters are not a subset of legal "
      "parameters");

    /* std::tuple<std::optional<remove_cvref_t<Passed_Params>>...> */
    /*   unfortunate_temp {std::forward<Passed_Params>(passed_params)...}; */

    // Populate m_params by moving from unfortunate_temp
    // In C++17, only assignment
    /* ((std::get<std::optional<remove_cvref_t<Passed_Params>>>(m_params) = */
    /*     std::move(std::get<std::optional<remove_cvref_t<Passed_Params>>>( */
    /*       unfortunate_temp))), */
    /*  ...); */
  }
};

}  // namespace supl

#endif
