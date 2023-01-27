#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {3, 3.14, 'd'};
  const std::tuple expected_output {true, 3, 3.14, 'd'};

  const auto actual_output {supl::tuple::push_front(test_input, true)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
