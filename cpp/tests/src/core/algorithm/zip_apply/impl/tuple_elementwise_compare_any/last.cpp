#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::tuple test4_1 {1, 2, 3};
    const std::tuple test4_2 {4, 5, 3};
    results.enforce_true(
      supl::impl::tuple_elementwise_compare_any(test4_1, test4_2),
      supl::to_string(test4_1) + " && " + supl::to_string(test4_2)
    );

    return results.print_and_return();
}
