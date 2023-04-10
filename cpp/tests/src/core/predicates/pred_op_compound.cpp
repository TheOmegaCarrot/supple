#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto test_pred_1 {(supl::greater_eq(3) && supl::less_eq(7))
                          || (supl::greater_eq(12) && supl::less_eq(17))};

  results.enforce_false(test_pred_1(0), "0 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(1), "1 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(2), "2 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(3), "3 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(4), "4 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(5), "5 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(6), "6 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(7), "7 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(8), "8 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(9), "9 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(10), "10 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(11), "11 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(12), "12 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(13), "13 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(14), "14 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(15), "15 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(16), "16 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(17), "17 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(18), "18 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(19), "19 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(20), "20 not in ([3, 7] U [12, 17])");

  results.enforce_true((supl::greater_than(3) && supl::less_than(8))(5));

  return results.print_and_return();
}
