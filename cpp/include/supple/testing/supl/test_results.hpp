/* {{{ doc */
/**
 * @file test_results.hpp
 *
 * @brief `test_results` class
 *
 * @details This header contains the definition of the
 * `test_results` class.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_TESTING_TEST_RESULTS_HPP
#define SUPPLE_TESTING_TEST_RESULTS_HPP

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
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
   * Must be equality comparable.
   * Must be a type which is a valid input to `supl::to_stream`.
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
  void enforce_exactly_equal(const T& result,
                             const T& expected,
                             const std::string_view message = "") noexcept
  {
    this->enforce_equal(result, expected, message);
  }

  /* {{{ doc */
  /**
   * @brief Enforce that two values are equal.
   * Values are permitted to be of different type,
   * so long as they are equality comparable,
   * with `T` as the left-hand side of operator `==`.
   *
   * @tparam T Type of value under test.
   * `T == U` must be valid.
   * Must be a class which is a valid input to `supl::to_stream`.
   * See documentation for `to_stream` for details.
   *
   * @tparam U Type of expected value.
   * `T == U` must be valid.
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
  template <typename T, typename U>
  void enforce_equal(const T& result,
                     const U& expected,
                     const std::string_view message = "") noexcept
  {
    m_case_count += 1;

    std::stringstream details;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    if ( result == expected ) {
      m_case_details.emplace_back();
    } else {
      m_fail_count += 1;

      details << "\nCase " << m_case_count << ":\t" << message
              << "\n\n\tExpected:\n\t" << supl::stream_adapter(expected)
              << "\n\n\tGot:\n\t" << supl::stream_adapter(result)
              << "\n\n";
      m_case_details.push_back(details.str());
    }
  }

  /* {{{ doc */
  /**
   * @brief Enforce that a condition is true.
   *
   * @param result Value produced by code under test
   *
   * @param message A string which will be printed if the test fails
   */
  /* }}} */
  inline void enforce_true(bool result,
                           const std::string_view message = "") noexcept
  {
    m_case_count += 1;

    std::stringstream details;

    if ( ! result ) {
      m_fail_count += 1;

      details << "\nCase " << m_case_count << ":\t" << message
              << "\n\n\tExpected true\n\n";
      m_case_details.push_back(details.str());
    } else {
      m_case_details.emplace_back();
    }
  }

  /* {{{ doc */
  /**
   * @brief Enforce that a condition is false.
   *
   * @param result Value produced by code under test
   *
   * @param message A string which will be printed if the test fails
   */
  /* }}} */
  inline void enforce_false(bool result,
                            const std::string_view message = "") noexcept
  {
    m_case_count += 1;

    std::stringstream details;

    if ( result ) {
      m_fail_count += 1;

      details << "\nCase " << m_case_count << ":\t" << message
              << "\n\n\tExpected false\n\n";
      m_case_details.push_back(details.str());
    } else {
      m_case_details.emplace_back();
    }
  }

  /* {{{ doc */
  /**
   * @brief Enforce that two floating-point values are
   * close enough to equal.
   *
   * @param result Value produced by code under test
   *
   * @param expected Known-good value which `result`
   * must be close enough to
   *
   * @param tolerance Definition of "close enough"
   *
   * @param message A string which will be printed if the test fails
   */
  /* }}} */
  inline void enforce_floating_point_approx(
    double result,
    double expected,
    double tolerance = 0.001,
    const std::string_view message = "") noexcept
  {
    m_case_count += 1;

    std::stringstream details;

    if ( std::abs(result - expected) > tolerance ) {
      m_fail_count += 1;

      details << "\nCase " << m_case_count << ":\t" << message
              << "\n\n\tExpected value within "
              << supl::stream_adapter(tolerance) << " of:\n\t"
              << supl::stream_adapter(expected) << "\n\n\tGot:\n\t"
              << supl::stream_adapter(result) << "\n\n";
      m_case_details.push_back(details.str());
    } else {
      m_case_details.emplace_back();
    }
  }

  /* {{{ doc */
  /**
   * @brief Fail with a message
   *
   * @param message String to print
   */
  /* }}} */
  inline void fail(const std::string_view message) noexcept
  {
    m_case_count += 1;
    m_fail_count += 1;

    std::stringstream details;

    details << "\nCase " << m_case_count << ":\n\n" << message << "\n\n";

    m_case_details.push_back(details.str());
  }

  [[nodiscard]] auto test_passes() const noexcept -> bool
  {
    return m_fail_count == 0;
  }

  void print_case_details() const noexcept
  {
    for ( const std::string& detail : m_case_details ) {
      std::cout << detail;
    }
  }

  [[nodiscard]] auto print_and_return() const noexcept -> int
  {
    this->print_case_details();
    if ( this->test_passes() ) {
      return EXIT_SUCCESS;
    } else {
      return EXIT_FAILURE;
    }
  }
};

}  // namespace supl

#endif
