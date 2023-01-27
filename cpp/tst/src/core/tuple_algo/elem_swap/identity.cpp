#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, 'g', 3.14, true, 18UL, 2.7F};
  const std::tuple expected {42, 'g', 3.14, true, 18UL, 2.7F};
  const auto result {supl::tuple::elem_swap<3, 3>(test_input)};

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}
