#include <string>
#include <vector>

#include "supl/test_results.hpp"

template <typename... Ls>
struct overload : Ls...
{
    using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

auto main() -> int
{
    supl::test_results results;
    using supl::literals::size_t_literal::operator""_z;

    const std::tuple<std::string, std::vector<int>> test_input {
      "Hello", {3, 9, 2}
    };

    const auto test_output {supl::tuple::transform(
      test_input,
      overload {
        [](const std::string& str)
        {
            return str.length();
        },
        [](const std::vector<int>& vec)
        {
            return vec.size();
        }
      }
    )};

    results.enforce_exactly_equal(
      std::get<0>(test_output), 5_z, "out2 : 0"
    );
    results.enforce_exactly_equal(
      std::get<1>(test_output), 3_z, "out2 : 1"
    );

    return results.print_and_return();
}
