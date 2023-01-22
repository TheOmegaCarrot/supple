/* {{{ doc */
/**
 * @file test_runner.hpp
 *
 * @brief `test_runner` class
 *
 * @details This header contains the definition of the
 * `test_runner` class.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_TESTING_TEST_RUNNER_HPP
#define SUPPLE_TESTING_TEST_RUNNER_HPP

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#include <supl/test_results.hpp>
#include <supl/test_section.hpp>

namespace supl {

class test_runner
{
private:

  std::vector<test_section> m_sections;

public:

  void add_section(test_section&& section) noexcept
  {
    m_sections.emplace_back(std::move(section));
  }

  [[nodiscard]] auto run() noexcept -> int
  {
    std::size_t num_test_functions {0};
    std::size_t num_failing_test_functions {0};

    for ( const test_section& section : m_sections ) {
      const std::vector<test_results> result_vector {section.run()};
      for ( const test_results& result : result_vector ) {
        num_test_functions += 1;
        if ( ! result.test_passes() ) {
          num_failing_test_functions += 1;
        }
      }
    }

    std::cout << "\n\nTotal tests:  " << num_test_functions << '\n'
              << "Failed tests: " << num_failing_test_functions << '\n';

    if ( num_failing_test_functions == 0 ) {
      return EXIT_SUCCESS;
    } else {
      return EXIT_FAILURE;
    }
  }
};

}  // namespace supl

#endif
