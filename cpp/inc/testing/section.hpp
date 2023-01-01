#ifndef TESTING_SECTION_HPP
#define TESTING_SECTION_HPP

#include <iomanip>
#include <iostream>
#include <vector>

#include "test.hpp"

namespace supl {

/* {{{ doc */
/**
 * @brief Type containing a set of related tests
 *
 * @details A section-defining function is a function which
 * has the signature: `supl::section()`.
 * The section-defining function constructs a `section`,
 * calls the `add_test` member function with
 * function pointers to the test functions to add tests to be run,
 * and returns that `section` object.
 */
/* }}} */
class section
{
private:

  using test_function_t = test (*)();

  struct test_data_t {
    std::string_view name;
    test_function_t function;
  };

  std::vector<test_data_t> m_test_functions {};

  constexpr static inline int test_output_width {60};

public:

  section() = default;

  /* {{{ doc */
  /**
   * @brief Append a function to the list of tests in a section
   *
   * @param test_name Name of the test
   *
   * @param test_function Test function to be ran
   */
  /* }}} */
  inline void add_test(std::string_view test_name,
                       test_function_t test_function) noexcept
  {
    m_test_functions.emplace_back(test_name, test_function);
  }

  [[nodiscard]] auto run() const noexcept -> std::size_t
  {
    std::size_t num_fails {0};
    for ( const auto& test_data : m_test_functions ) {
      test test_result {test_data.function()};
      if ( !test_result.test_passes() ) {
        num_fails += 1;
        std::cout << std::left << std::setw(test_output_width)
                  << std::setfill('.') << test_data.name << "FAIL" << '\n';
      }
    }
    return num_fails;
  }
};

} // namespace supl

#endif
