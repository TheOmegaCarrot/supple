#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  results.enforce_true(5 | supl::less_than(10));
  results.enforce_true(5 | (supl::less_than(10) && supl::greater_than(0)));
  results.enforce_true(5.32
                       | (supl::less_than(10) && supl::greater_than(0)));
  results.enforce_false(20
                        | (supl::less_than(10) && supl::greater_than(0)));

  return results.print_and_return();
}
