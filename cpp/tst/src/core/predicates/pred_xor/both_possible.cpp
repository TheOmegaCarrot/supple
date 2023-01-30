#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto less_than_6_xor_equals_3 {
    supl::pred_xor(supl::less_than(6), supl::equal_to(3))};

  results.enforce_true(less_than_6_xor_equals_3(5), "5 < 6 ^ 5 == 3");
  results.enforce_false(less_than_6_xor_equals_3(3), "3 < 6 ^ 3 == 3");
  results.enforce_false(less_than_6_xor_equals_3(10), "10 < 6 ^ 10 == 3");

  return results.print_and_return();
}
