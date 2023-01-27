#include <supl/algorithm.hpp>
#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  results.enforce_false(
    supl::none_of(supl::greater_than(42), 2, 18, 1, 4.82, 85.2, 3),
    "One pass case");

  results.enforce_true(
    supl::none_of(supl::greater_than(42), 8, 3.14, 18, 27),
    "None pass case");

  results.enforce_true(supl::none_of(supl::less_than(8)),
                       "No argument case");

  return results.print_and_return();
}
