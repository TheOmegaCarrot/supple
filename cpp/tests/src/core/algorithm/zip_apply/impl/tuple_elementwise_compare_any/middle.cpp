#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::tuple test3_1 {1, 2, 3};
  const std::tuple test3_2 {4, 2, 5};
  results.enforce_true(
    supl::impl::tuple_elementwise_compare_any(test3_1, test3_2),
    supl::to_string(test3_1) + " && " + supl::to_string(test3_2));

  return results.print_and_return();
}
