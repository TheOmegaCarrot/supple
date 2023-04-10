#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  int referenced_data {5};

  const std::tuple<int&, int, char, bool> test_input {
    referenced_data, 3, 'g', true};

  const std::tuple<int&, int, char, bool, double> expected_output {
    referenced_data, 3, 'g', true, 3.14};

  const auto actual_output {supl::tuple::push_back(test_input, 3.14)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
