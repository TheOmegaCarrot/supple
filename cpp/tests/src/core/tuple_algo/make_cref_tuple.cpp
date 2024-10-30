#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const int a {5};
    const char b {'b'};
    const bool c {true};

    const std::tuple<const int&, const char&, const bool&> expected {
      a, b, c
    };

    const auto result {supl::tuple::make_cref_tuple(a, b, c)};

    results.enforce_exactly_equal(result, expected);

    return results.print_and_return();
}
