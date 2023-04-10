#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto less_than_6_bicond_equals_3 {
    supl::pred_bicond(supl::less_than(6), supl::equal_to(3))};

  results.enforce_false(less_than_6_bicond_equals_3(5), "5 < 6 ^ 5 == 3");
  results.enforce_true(less_than_6_bicond_equals_3(3), "3 < 6 ^ 3 == 3");
  results.enforce_true(less_than_6_bicond_equals_3(10),
                       "10 < 6 ^ 10 == 3");

  const auto greater_than_8_bicond_equals_3 {
    supl::pred_bicond(supl::greater_than(8), supl::equal_to(3))};

  results.enforce_false(greater_than_8_bicond_equals_3(10),
                        "10 > 8 ^ 10 == 3");
  results.enforce_false(greater_than_8_bicond_equals_3(3),
                        "3 > 8 ^ 3 == 3");
  results.enforce_true(greater_than_8_bicond_equals_3(5),
                       "5 > 8 ^ 5 == 3");

  return results.print_and_return();
}
