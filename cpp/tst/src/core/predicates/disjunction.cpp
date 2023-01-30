#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto less_than_4_or_greater_than_10 {
    supl::disjunction(supl::less_than(4), supl::greater_than(10))};

  results.enforce_true(less_than_4_or_greater_than_10(1),
                       "1 < 4 || 1 > 10");
  results.enforce_true(less_than_4_or_greater_than_10(15),
                       "15 < 4 || 15 > 10");
  results.enforce_true(less_than_4_or_greater_than_10(-8),
                       "-8 < 4 || -8 > 10");
  results.enforce_false(less_than_4_or_greater_than_10(7),
                        "7 < 4 || 7 > 10");

  return results.print_and_return();
}
