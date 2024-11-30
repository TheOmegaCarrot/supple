#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    int referenced_existing_element {5};
    const std::tuple<int&, int, char, bool> test_input {
      referenced_existing_element, 3, 'g', true
    };

    double referenced_pushed_data {3.14};
    const std::tuple<double&, int&, int, char, bool> expected_output {
      referenced_pushed_data, referenced_existing_element, 3, 'g', true
    };

    const auto actual_output {supl::tuple::push_front_as<double&>(
      test_input, referenced_pushed_data
    )};

    results.enforce_exactly_equal(actual_output, expected_output);

    return results.print_and_return();
}
