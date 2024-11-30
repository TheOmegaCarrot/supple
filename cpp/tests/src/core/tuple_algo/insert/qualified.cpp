#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    int referenced_data {42};

    const std::tuple<const char, int&, double> test_input {
      'g', referenced_data, 3.14
    };

    const std::tuple<const char, int&, bool, char, double>
      expected_output {'g', referenced_data, false, '*', 3.14};

    const auto actual_output {
      supl::tuple::insert<2>(test_input, false, '*')
    };

    results.enforce_exactly_equal(actual_output, expected_output);

    return results.print_and_return();
}
