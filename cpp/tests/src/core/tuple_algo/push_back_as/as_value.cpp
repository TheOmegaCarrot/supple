#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::tuple test_input {3, 3.14, 'd'};
    const int new_data {81};
    const std::tuple expected_output {3, 3.14, 'd', 81};
    const auto actual_output {
      supl::tuple::push_back_as<int>(test_input, new_data)
    };

    results.enforce_exactly_equal(actual_output, expected_output);

    return results.print_and_return();
}
