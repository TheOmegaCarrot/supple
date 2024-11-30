#include <array>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    constexpr std::array input {3, 6, 1, 9, 5, 12};
    constexpr std::array expected_output {3, 6, 1, 9};
    constexpr static std::array output {
      [&input]()
      {
          std::array<int, 4> tmp {};
          supl::copy_n(input.begin(), 4, tmp.begin());
          return tmp;
      }()
    };

    results.enforce_equal(output, expected_output);

    return results.print_and_return();
}
