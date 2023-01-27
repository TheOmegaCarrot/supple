#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, true, 3.14};

  const std::tuple expected_output {42, true, 'B'};

  const auto actual_output {
    supl::tuple::replace<2, char>(test_input, 'B')};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
