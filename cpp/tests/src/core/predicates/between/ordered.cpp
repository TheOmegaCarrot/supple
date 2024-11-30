#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const auto between_3_and_8 {supl::between(3, 8)};

    results.enforce_false(between_3_and_8(0), "0");
    results.enforce_false(between_3_and_8(1), "1");
    results.enforce_false(between_3_and_8(2), "2");
    results.enforce_true(between_3_and_8(3), "3");
    results.enforce_true(between_3_and_8(4), "4");
    results.enforce_true(between_3_and_8(5), "5");
    results.enforce_true(between_3_and_8(6), "6");
    results.enforce_true(between_3_and_8(7), "7");
    results.enforce_true(between_3_and_8(8), "8");
    results.enforce_false(between_3_and_8(9), "9");
    results.enforce_false(between_3_and_8(10), "10");

    return results.print_and_return();
}
