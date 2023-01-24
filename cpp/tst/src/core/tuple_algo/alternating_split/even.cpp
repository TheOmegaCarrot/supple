#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {1, 'J', true, 3.14, 'c', 42};
  const std::tuple expected_first {1, true, 'c'};
  const std::tuple expected_second {'J', 3.14, 42};

  const auto [result_first,
              result_second] {supl::tuple::alternating_split(test_input)};

  results.enforce_exactly_equal(result_first, expected_first);
  results.enforce_exactly_equal(result_second, expected_second);

  return results.print_and_return();
}

