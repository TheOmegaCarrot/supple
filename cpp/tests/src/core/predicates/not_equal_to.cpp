#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto not_equal_to_5 {supl::not_equal_to(5)};

  results.enforce_true(not_equal_to_5(3), "(3 != 5) == true");
  results.enforce_true(not_equal_to_5(7), "(7 != 5) == true");
  results.enforce_false(not_equal_to_5(5), "(5 != 5) == false");

  return results.print_and_return();
}
