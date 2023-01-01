#ifndef TESTING_TEST_HPP
#define TESTING_TEST_HPP

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <supl/utility.hpp>

namespace supl {

/* {{{ doc */
/**
 * @brief Type containing data from a single test function.
 *
 * @details A test function is defined as a free function with the signature:
 * `supl::test_results()`. The test function constructs a `test`,
 * calls `enforce*` member functions to provide it data,
 * and returns that `test` object.
 */
/* }}} */
class test_results
{
private:

  std::size_t m_case_count {0};
  std::size_t m_fail_count {0};

  // passing test is empty string,
  // failing test contains details of failure
  //
  // each call to an `enforce*` member function
  // is one case
  //
  // m_case_details.size() == <# of calls to `enforce*` member functions>
  std::vector<std::string> m_case_details {};

public:

  test_results() = default;

  /* {{{ doc */
  /**
   * @brief Enforce that two values are equal and of the same type.
   *
   * @tparam T Type of expected value.
   * Must be a class which is a valid input to `supl::to_stream`.
   * See documentation for `to_stream` for details.
   *
   * @param result Value produced by code under test
   *
   * @param expected Known-good value which `result` must equal
   *
   * @param message A string which will be printed if the test fails
   */
  /* }}} */
  template <typename T>
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  void enforce_exactly_equal(const T& result, const T& expected,
                             const std::string& message = {})
  {
    m_case_count += 1;

    std::stringstream details;

    if ( result != expected ) {
      m_fail_count += 1;

      details << std::left << std::setw(10) << "Case " << m_case_count
              << '\t' << message << "\n\n\tExpected:\n\t"
              << supl::stream_adapter(expected) << "\n\n\tGot:\n\t"
              << supl::stream_adapter(result) << "\n\n";
      m_case_details.push_back(details.str());
    } else {
      m_case_details.emplace_back();
    }
  }

  /* {{{ doc */
  /**
   * @brief Get total number of test cases.
   */
  /* }}} */
  [[nodiscard]] auto case_count() const noexcept -> std::size_t
  {
    return m_case_count;
  }

  /* {{{ doc */
  /**
   * @brief Get total number of failed tests
   */
  /* }}} */
  [[nodiscard]] auto fail_count() const noexcept -> std::size_t
  {
    return m_fail_count;
  }

  [[nodiscard]] auto test_passes() const noexcept -> bool
  {
    return m_fail_count == 0;
  }
};

} // namespace supl

#endif
