#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::string a_string {"reorder"};
  const std::vector a_vector {1, 2, 3, 4};
  const std::tuple test_input {3, 3.14, true, a_string, a_vector};
  const std::tuple expected {3.14, a_string, 3, 3.14, a_vector, true};

  const auto result {supl::tuple::reorder<1, 3, 0, 1, 4, 2>(test_input)};

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}
