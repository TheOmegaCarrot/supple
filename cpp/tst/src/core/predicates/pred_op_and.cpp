#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto between_3_and_8 {supl::greater_than(3) && supl::less_than(8)};

  results.enforce_false(between_3_and_8(0), "3 < 0 < 8");
  results.enforce_false(between_3_and_8(1), "3 < 1 < 8");
  results.enforce_false(between_3_and_8(2), "3 < 2 < 8");
  results.enforce_false(between_3_and_8(3), "3 < 3 < 8");
  results.enforce_true(between_3_and_8(4), "3 < 4 < 8");
  results.enforce_true(between_3_and_8(5), "3 < 5 < 8");
  results.enforce_true(between_3_and_8(6), "3 < 6 < 8");
  results.enforce_true(between_3_and_8(7), "3 < 7 < 8");
  results.enforce_false(between_3_and_8(8), "3 < 8 < 8");
  results.enforce_false(between_3_and_8(9), "3 < 9 < 8");
  results.enforce_false(between_3_and_8(10), "3 < 10 < 8");

  return results.print_and_return();
}

