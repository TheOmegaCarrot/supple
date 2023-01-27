#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  int referenced_data {5};
  const std::tuple<int&, int, char, bool> test_input {
    referenced_data, 3, 'g', true};
  const std::tuple<double, int&, int, char, bool> expected_output {
    3.14, referenced_data, 3, 'g', true};

  const auto actual_output {supl::tuple::push_front(test_input, 3.14)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
