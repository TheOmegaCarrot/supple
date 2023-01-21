#ifndef SUPPLE_TESTING_EXCEPTION_SAFETY_HPP
#define SUPPLE_TESTING_EXCEPTION_SAFETY_HPP

#include <exception>

namespace supl {

class guaranteed_exception : public std::exception
{
public:

  [[nodiscard]] auto what() const noexcept -> const char* override
  {
    return "Guaranteed error from supple testing type.";
  }
};

/* {{{ doc */
/**
 * @brief Type which is guaranteed to throw on copy.
 *
 * @details Immediately throws a `supl::guaranteed_error` upon copy construction
 * or copy assignment. Move construction and assignment is defaulted.
*/
/* }}} */
struct throws_on_copy {
  throws_on_copy() = default;

  [[noreturn]] throws_on_copy(const throws_on_copy&)
  {
    throw guaranteed_exception {};
  }

  [[noreturn]] auto operator=(const throws_on_copy&) -> throws_on_copy&
  {
    throw guaranteed_exception {};
  }

  throws_on_copy(throws_on_copy&&) = default;
  auto operator=(throws_on_copy&&) -> throws_on_copy& = default;

  ~throws_on_copy() = default;
};

}  // namespace supl

#endif
