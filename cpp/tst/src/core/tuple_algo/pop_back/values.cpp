#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {3, 3.14, 'd'};
  const std::tuple expected_output {3, 3.14};

  const auto actual_output {supl::tuple::pop_back(test_input)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}

