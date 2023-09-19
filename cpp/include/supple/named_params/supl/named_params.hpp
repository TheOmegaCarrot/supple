#ifndef SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP
#define SUPPLE_NAMED_PARAMS_NAMED_PARAMS_HPP

#include <tuple>

#include <supl/type_list.hpp>

namespace supl::np {

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

  static_assert(! tl::has_duplicates_v<tl::type_list<Legal_Params...> >,
                "Named parameter type list contains duplicate types");

private:

  std::tuple<Legal_Params...> m_params;

public:

  template <typename... Passed_Params>
  explicit named_params(Passed_Params&&... passed_params)
  {
    static_assert(! tl::has_duplicates_v<tl::type_list<Passed_Params...> >,
                  "Passed parameter type list contains duplicates");

    /* static_assert(tl::is_subset_v<tl::type_list<Passed_Params...>, */
    /*                               tl::type_list<Legal_Params...> >, */
    /*               "Passed parameters are not a subset of legal parameters"); */

  }
};

}  // namespace supl::np

#endif
