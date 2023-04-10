#include "supl/fake_ranges.hpp"
#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto not_less_than_6 {supl::pred_not(supl::less_than(6))};

  for ( const int i : supl::fr::iota(1, 6) ) {
    results.enforce_false(not_less_than_6(i),
                          std::to_string(i) + " not less than 6");
  }

  for ( const int i : supl::fr::iota(6, 10) ) {
    results.enforce_true(not_less_than_6(i),
                         std::to_string(i) + " not less than 6");
  }

  return results.print_and_return();
}
