#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input_1 {42, 'g', 3.14};
  const std::tuple test_input_2 {2.6F, true, 8L};

  const std::tuple expected_output {42, 2.6F, 'g', true, 3.14, 8L};
  const auto result {supl::tuple::interleave(test_input_1, test_input_2)};

  results.enforce_exactly_equal(result, expected_output);

  return results.print_and_return();
}

