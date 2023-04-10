#include "supl/algorithm.hpp"
#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  results.enforce_true(supl::all_of(supl::greater_than(3), 4, 8, 3.14, 42),
                       "All pass case");

  results.enforce_false(supl::all_of(supl::greater_than(3), 4, 8, 3.14, 1),
                        "One argument fails");

  results.enforce_true(supl::all_of(supl::less_than(8)),
                       "No argument case");

  return results.print_and_return();
}
