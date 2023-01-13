/* {{{ doc */
/**
 * @file test_section.hpp
 *
 * @brief `test_section` class
 *
 * @details This header contains the definition of the
 * `test_section` class.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_TESTING_TEST_SECTION_HPP
#define SUPPLE_TESTING_TEST_SECTION_HPP

#include <iomanip>
#include <iostream>
#include <vector>

#include <supl/test_results.hpp>

namespace supl {

namespace constants {
  constexpr inline int test_output_width {60};
  constexpr inline char fill_char {'.'};
}  // namespace constants

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
class test_section
{
private:

  using test_function_t = test_results (*)();

  struct test_function_data_t {
    std::string_view name;
    test_function_t function;
  };

  std::vector<test_function_data_t> m_test_functions {};

public:

  test_section() = default;

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
    m_test_functions.push_back(
      test_function_data_t {test_name, test_function});
  }

  /* {{{ doc */
  /**
   * @brief Runs the tests
   */
  /* }}} */
  [[nodiscard]] auto run() const noexcept -> std::vector<test_results>
  {
    std::vector<test_results> results;
    for ( auto&& test_data : m_test_functions ) {
      const test_results test_result {test_data.function()};
      if ( ! test_result.test_passes() ) {
        std::cout << std::left << std::setw(constants::test_output_width)
                  << std::setfill(constants::fill_char) << test_data.name
                  << "FAIL" << '\n';
        test_result.print_case_details();
      }
      results.push_back(test_result);
    }
    return results;
  }
};

}  // namespace supl

#endif
