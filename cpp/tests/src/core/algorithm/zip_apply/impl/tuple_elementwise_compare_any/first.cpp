#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::tuple test2_1 {1, 2, 3};
    const std::tuple test2_2 {1, 4, 5};
    results.enforce_true(
      supl::impl::tuple_elementwise_compare_any(test2_1, test2_2),
      supl::to_string(test2_1) + " && " + supl::to_string(test2_2)
    );

    return results.print_and_return();
}
