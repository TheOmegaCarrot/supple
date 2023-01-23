#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, 'g', 3.14, true, 'D'};
  const std::tuple expected {'D', true, 3.14, 'g', 42};
  const auto result {supl::tuple::reverse(test_input)};
  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}

