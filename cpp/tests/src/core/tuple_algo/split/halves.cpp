#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::string a_string {"split"};
    const std::vector a_vector {1, 2, 3, 4};
    const std::tuple test_input {3, 3.14, a_vector, true, 'f', a_string};

    const std::tuple expected_first {3, 3.14, a_vector};
    const std::tuple expected_second {true, 'f', a_string};
    const auto result {supl::tuple::split<3>(test_input)};

    results.enforce_exactly_equal(result.first, expected_first, "First");
    results.enforce_exactly_equal(
      result.second, expected_second, "Second"
    );

    return results.print_and_return();
}
