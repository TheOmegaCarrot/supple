#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  int referenced_data {5};
  const std::tuple<bool, int&, int, char> test_input {
    true, referenced_data, 3, 'g'};
  const std::tuple<int&, int, char> expected_output {
    referenced_data, 3, 'g'};

  const auto actual_output {supl::tuple::pop_front(test_input)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
