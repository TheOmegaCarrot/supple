#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  int referenced_data {5};
  const std::tuple<int&, int, char, bool> test_input {
    referenced_data, 3, 'g', true};
  const std::tuple<int&, int, char> expected_output {
    referenced_data, 3, 'g'};

  const auto actual_output {supl::tuple::pop_back(test_input)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
