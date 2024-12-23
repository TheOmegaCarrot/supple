#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    constexpr static std::tuple referenced_data {3, true, 3.14};

    constexpr static std::tuple<int, decltype(referenced_data)&, bool>
      test_input {42, referenced_data, false};

    constexpr static std::tuple<int, decltype(referenced_data)&>
      expected_output {42, referenced_data};

    constexpr static auto actual_output {supl::tuple::erase<2>(test_input)
    };

    results.enforce_exactly_equal(actual_output, expected_output);

    return results.print_and_return();
}
