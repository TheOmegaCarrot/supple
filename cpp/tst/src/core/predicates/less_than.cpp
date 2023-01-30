#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto less_than_5 {supl::less_than(5)};

  results.enforce_true(less_than_5(3), "3 < 5 == true");
  results.enforce_false(less_than_5(7), "7 < 5 == false");
  results.enforce_false(less_than_5(5), "5 < 5 == false");

  return results.print_and_return();
}
