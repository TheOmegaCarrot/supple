#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  constexpr static std::tuple test_input {3, true, 3.14};

  constexpr static std::tuple expected_output {3, true};

  constexpr static auto actual_output {supl::tuple::erase<2>(test_input)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
