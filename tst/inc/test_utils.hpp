#ifndef EHANC_TEST_UTILS_HPP
#define EHANC_TEST_UTILS_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>

#include "utils/term_colors.h"

static const int TEST_OUTPUT_WIDTH = 60;

namespace ehanc {

class test
{
private:
  int case_index{};
  bool m_pass{true};

public:
  std::vector<std::string> cases{};

  test() = default;

  template <typename T>
  inline void add_case(const T& result, const T& expected,
                       const std::string_view message = "")
  {
    ++case_index;
    if ( result != expected ) {
      m_pass = false;
      std::stringstream detail;

      detail << std::boolalpha << std::left << std::setw(10) << FG_RED
             << "Case " << case_index << '\t' << message
             << "\n\n\tExpected:\n"
             << RESET << '\t' << expected << FG_RED << "\n\n\tGot:\n"
             << RESET << '\t' << result << '\n';

      cases.push_back(detail.str());
    }
  }

  inline bool pass()
  {
    return m_pass;
  }

}; // class test

inline void run_test(const std::string_view name,
                     std::function<test()> test_func)
{
  test result = test_func();

  if ( result.pass() ) {
    std::cout << std::left << std::setw(TEST_OUTPUT_WIDTH)
              << std::setfill('.') << name << FG_GREEN << "PASS" << RESET
              << '\n';
  } else {
    std::cout << std::left << std::setw(TEST_OUTPUT_WIDTH)
              << std::setfill('.') << name << FG_RED << "FAIL" << RESET
              << '\n';

    for ( const auto& details : result.cases ) {
      std::cout << details << '\n';
    }
  }
}

} // namespace ehanc

#endif
