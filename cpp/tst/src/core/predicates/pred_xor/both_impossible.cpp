#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto greater_than_8_xor_equals_3 {
    supl::pred_xor(supl::greater_than(8), supl::equal_to(3))};

  results.enforce_true(greater_than_8_xor_equals_3(10),
                       "10 > 8 ^ 10 == 3");
  results.enforce_true(greater_than_8_xor_equals_3(3), "3 > 8 ^ 3 == 3");
  results.enforce_false(greater_than_8_xor_equals_3(5), "5 > 8 ^ 5 == 3");

  return results.print_and_return();
}
