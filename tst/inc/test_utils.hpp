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

constexpr inline const int TEST_OUTPUT_WIDTH         = 60;
constexpr inline const std::string_view HEADER_COLOR = ehanc::FG_RED;

namespace ehanc {

class test
{
private:
  int m_case_index{};
  bool m_pass{true};
  std::vector<std::string> m_cases{};

public:
  test() = default;

  template <typename T>
  inline void add_case(const T& result, const T& expected,
                       const std::string_view message = "")
  {
    ++m_case_index;
    if ( result != expected ) {
      m_pass = false;
      std::stringstream detail;

      detail << std::boolalpha << std::left << std::setw(10) << FG_RED
             << "Case " << m_case_index << '\t' << message
             << "\n\n\tExpected:\n"
             << RESET << '\t' << expected << FG_RED << "\n\n\tGot:\n"
             << RESET << '\t' << result << '\n'
             << '\n';

      m_cases.push_back(detail.str());
    } else {
      m_cases.push_back("");
    }
  }

  inline const std::vector<std::string>& cases()
  {
    return m_cases;
  }

  inline bool pass() const
  {
    return m_pass;
  }

}; // class test

inline void run_test(const std::string_view name,
                     const std::function<test()>& test_func)
{
  test result = test_func();

  if ( result.pass() ) {
    std::cout << std::left << std::setw(TEST_OUTPUT_WIDTH)
              << std::setfill('.') << name << FG_GREEN << "PASS" << RESET
              << '\n';
  } else {
    std::cout << std::left << std::setw(TEST_OUTPUT_WIDTH)
              << std::setfill('.') << name << FG_RED << "FAIL" << RESET
              << '\n'
              << '\n';

    for ( const auto& details : result.cases() ) {
      std::cout << details;
    }
  }
}

inline void test_section(const std::string_view section_name,
                         const std::function<void()>& section_func)
{
  std::cout << HEADER_COLOR << section_name << ':' << RESET << '\n';
  section_func();
  std::cout << '\n' << '\n';
}

} // namespace ehanc

#endif
