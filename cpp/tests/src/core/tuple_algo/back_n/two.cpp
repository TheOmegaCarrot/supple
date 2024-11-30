#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::tuple test_input {42, '&', 3.14};
    const std::tuple expected {'&', 3.14};
    const auto result {supl::tuple::back_n<2>(test_input)};

    results.enforce_exactly_equal(result, expected);

    return results.print_and_return();
}
