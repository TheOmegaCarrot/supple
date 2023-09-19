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

  using param_type_list = tl::type_list<Legal_Params...>;
  static_assert(! tl::has_duplicates_v<param_type_list>,
                "Named parameter type list contains duplicate types");

private:

  std::tuple<Legal_Params...> m_params;

public:

  template <typename... Passed_Params,
            typename = std::enable_if_t<
              (is_type_in_pack_v<Passed_Params, Legal_Params...> && ...)> >
  explicit named_params(Passed_Params&&... passed_params)
  {
    // need to init m_params
  }
};

}  // namespace supl::np

#endif
