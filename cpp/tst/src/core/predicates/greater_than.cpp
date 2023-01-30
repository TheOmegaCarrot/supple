#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto greater_than_5 {supl::greater_than(5)};

  results.enforce_false(greater_than_5(3), "3 > 5 == false");
  results.enforce_true(greater_than_5(7), "7 > 5 == true");
  results.enforce_false(greater_than_5(5), "5 > 5 == false");

  return results.print_and_return();
}
