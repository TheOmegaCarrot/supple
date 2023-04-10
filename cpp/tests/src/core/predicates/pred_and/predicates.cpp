#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto between_3_and_8_inclusive {
    supl::pred_and(supl::greater_eq(3), supl::less_eq(8))};

  results.enforce_true(between_3_and_8_inclusive(5), "3 <= 5 <= 8");
  results.enforce_true(between_3_and_8_inclusive(3), "3 <= 3 <= 8");
  results.enforce_true(between_3_and_8_inclusive(7), "3 <= 7 <= 8");
  results.enforce_true(between_3_and_8_inclusive(4), "3 <= 4 <= 8");
  results.enforce_true(between_3_and_8_inclusive(8), "3 <= 8 <= 8");
  results.enforce_false(between_3_and_8_inclusive(1), "3 <= 1 <= 8");
  results.enforce_false(between_3_and_8_inclusive(10), "3 <= 10 <= 8");

  return results.print_and_return();
}
