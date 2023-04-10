#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {3, 3.14, 'd'};

  const bool referenced_data {true};
  const std::tuple<const bool&, int, double, char> expected_output {
    referenced_data, 3, 3.14, 'd'};

  const auto actual_output {
    supl::tuple::push_front_as<const bool&>(test_input, referenced_data)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
