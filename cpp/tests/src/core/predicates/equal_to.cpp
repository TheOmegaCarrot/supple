#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto equal_to_5 {supl::equal_to(5)};

  results.enforce_exactly_equal(equal_to_5(3), false, "(3 == 5) == false");
  results.enforce_exactly_equal(equal_to_5(7), false, "(7 == 5) == false");
  results.enforce_exactly_equal(equal_to_5(5), true, "(5 == 5) == true");

  return results.print_and_return();
}
