#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::string a_string {"subtuple"};
  const std::vector a_vector {1, 2, 3, 4};
  const std::tuple test_input {3, true, a_string, 3.14, a_vector};
  const std::tuple expected {test_input};
  const auto result {
    supl::tuple::subtuple<0, std::tuple_size_v<decltype(test_input)>>(
      test_input)};

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}
