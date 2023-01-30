#include <supl/fake_ranges.hpp>
#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  for ( const int i : supl::fr::iota(-50, 51) ) {
    results.enforce_false(supl::false_pred(i));
  }

  return results.print_and_return();
}
