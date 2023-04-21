#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input_1 {
    42, 'g', 3.14, 81, 5.31F, 4UL, false, '7'};
  const std::tuple test_input_2 {
    2.6F,
    true,
    8L,
    8.3F,
    2U,
    'u',
    std::string {"yes"},
    std::vector {3, 5, 7}
  };

  const std::tuple expected_output {
    42,
    2.6F,
    'g',
    true,
    3.14,
    8L,
    81,
    8.3F,
    5.31F,
    2U,
    4UL,
    'u',
    false,
    std::string {"yes"},
    '7',
    std::vector {3, 5, 7}
  };
  const auto result {supl::tuple::interleave(test_input_1, test_input_2)};

  results.enforce_exactly_equal(result, expected_output);

  return results.print_and_return();
}
