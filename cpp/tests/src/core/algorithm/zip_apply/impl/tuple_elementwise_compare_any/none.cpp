#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::tuple test1_1 {1, 2, 3};
  const std::tuple test1_2 {4, 5, 6};
  results.enforce_false(
    supl::impl::tuple_elementwise_compare_any(test1_1, test1_2),
    supl::to_string(test1_1) + " && " + supl::to_string(test1_2));

  return results.print_and_return();
}
