#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto greater_than_6_implies_equals_3 {
    supl::pred_implies(supl::greater_than(6), supl::equal_to(3))};

  results.enforce_true(greater_than_6_implies_equals_3(4),
                       "Greater than 6 implies equal to 3: 4");

  results.enforce_true(greater_than_6_implies_equals_3(5),
                       "Greater than 6 implies equal to 3: 5");

  results.enforce_true(greater_than_6_implies_equals_3(6),
                       "Greater than 6 implies equal to 3: 6");

  results.enforce_false(greater_than_6_implies_equals_3(7),
                        "Greater than 6 implies equal to 3: 7");

  results.enforce_false(greater_than_6_implies_equals_3(8),
                        "Greater than 6 implies equal to 3: 8");

  return results.print_and_return();
}

