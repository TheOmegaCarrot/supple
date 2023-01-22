#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, false, 3.14, 'w'};

  const std::tuple expected_output {false, 3.14, 'w', 42};

  const auto actual_output {supl::tuple::rotate_left(test_input)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}

