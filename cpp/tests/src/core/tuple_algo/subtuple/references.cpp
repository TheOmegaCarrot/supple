#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    int referenced_data {42};

    const std::tuple<char, bool, int&, double, char> test_input {
      'j', true, referenced_data, 3.14, 'l'
    };

    const std::tuple<bool, int&, double> expected {
      true, referenced_data, 3.14
    };

    const auto result {supl::tuple::subtuple<1, 4>(test_input)};

    results.enforce_exactly_equal(result, expected);

    return results.print_and_return();
}
