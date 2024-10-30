#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    using supl::literals::size_t_literal::operator""_z;

    constexpr static std::tuple test_input {42, 3.14, true};

    constexpr static std::tuple expected {42.0, 3_z, 1};

    constexpr static auto result {
      supl::tuple::convert<double, std::size_t, int>(test_input)
    };

    results.enforce_exactly_equal(result, expected);

    return results.print_and_return();
}
