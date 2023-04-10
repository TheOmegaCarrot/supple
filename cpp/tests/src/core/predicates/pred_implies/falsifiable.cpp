#include "supl/fake_ranges.hpp"
#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto equals_3_imples_less_than_6 {
    supl::pred_implies(supl::equal_to(3), supl::less_than(6))};

  for ( const int i : supl::fr::iota(0, 11) ) {
    results.enforce_true(equals_3_imples_less_than_6(i),
                         "Equal to 3 implies less than 6: "
                           + std::to_string(i));
  }

  return results.print_and_return();
}
