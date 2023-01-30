#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto greater_eq_5 {not supl::less_than(5)};

  results.enforce_false(greater_eq_5(0), "0 >= 5");
  results.enforce_false(greater_eq_5(1), "1 >= 5");
  results.enforce_false(greater_eq_5(2), "2 >= 5");
  results.enforce_false(greater_eq_5(3), "3 >= 5");
  results.enforce_false(greater_eq_5(4), "4 >= 5");
  results.enforce_true(greater_eq_5(5), "5 >= 5");
  results.enforce_true(greater_eq_5(6), "6 >= 5");
  results.enforce_true(greater_eq_5(7), "7 >= 5");
  results.enforce_true(greater_eq_5(8), "8 >= 5");
  results.enforce_true(greater_eq_5(9), "9 >= 5");

  return results.print_and_return();
}

