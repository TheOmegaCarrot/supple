#ifndef EHANC_TEST_UTILS_HPP
#define EHANC_TEST_UTILS_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "supl/term_colors.h"
#include "supl/utility.hpp"

constexpr inline int test_output_width         = 60;
constexpr inline std::string_view HEADER_COLOR = supl::FG_RED;

namespace ehanc {

class test
{
private:

  int m_case_index {};
  bool m_pass {true};
  std::vector<std::string> m_cases {};

public:

  explicit test() = default;

  template <typename T>
  inline void add_case(const T& result, const T& expected,
                       const std::string_view message = "")
  {
    ++m_case_index;
    if ( result != expected ) {
      m_pass = false;
      std::stringstream detail;

      detail << std::boolalpha << std::left << std::setw(10)
             << supl::FG_RED << "Case " << m_case_index << '\t' << message
             << "\n\n\tExpected:\n"
             << supl::RESET << '\t' << supl::stream_adapter(expected)
             << supl::FG_RED << "\n\n\tGot:\n"
             << supl::RESET << '\t' << supl::stream_adapter(result) << '\n'
             << '\n';

      m_cases.push_back(detail.str());
    } else {
      m_cases.emplace_back();
    }
  }

  inline auto cases() -> const std::vector<std::string>&
  {
    return m_cases;
  }

  [[nodiscard]] inline auto pass() const -> bool
  {
    return m_pass;
  }

}; // class test

template <typename Test_Func>
inline void run_test(const std::string_view name, Test_Func&& test_func)
{
  static_assert(std::is_invocable_r_v<test, Test_Func>,
                "Test function must have correct signature");

  test result = test_func();

  if ( result.pass() ) {
    std::cout << std::left << std::setw(test_output_width)
              << std::setfill('.') << name << supl::FG_GREEN << "PASS"
              << supl::RESET << '\n';
  } else {
    std::cout << std::left << std::setw(test_output_width)
              << std::setfill('.') << name << supl::FG_RED << "FAIL"
              << supl::RESET << '\n'
              << '\n';

    for ( const auto& details : result.cases() ) {
      std::cout << details;
    }
  }
}

template <typename Section_Func>
inline void test_section(const std::string_view section_name,
                         Section_Func&& section_func)
{
  static_assert(std::is_invocable_r_v<void, Section_Func>);

  std::cout << '\n';
  std::cout << HEADER_COLOR << section_name << ':' << supl::RESET << '\n';
  section_func();
  std::cout << '\n';
}

} // namespace ehanc

#endif
