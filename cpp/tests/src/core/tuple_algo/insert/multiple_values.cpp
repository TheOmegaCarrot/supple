#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {3, 3.14};
  const std::string a_string {"neat"};
  const std::vector a_vector {1, 2, 3, 4};

  const std::tuple expected_output {3, 42069, a_string, a_vector, 3.14};
  const auto actual_output {
    supl::tuple::insert<1>(test_input, 42069, a_string, a_vector)};

  results.enforce_exactly_equal(actual_output, expected_output);

  return results.print_and_return();
}
