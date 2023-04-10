#include <array>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  constexpr std::array input {1, 4, 9};

  constexpr static std::array expected_output {4, 16, 36};

  constexpr static std::array output {[&input]() {
    std::array<int, 3> tmp {};
    supl::transform(
      input.begin(), input.end(), tmp.begin(), [](auto&& arg) {
        return arg * 4;
      });
    return tmp;
  }()};

  results.enforce_equal(output, expected_output);

  return results.print_and_return();
}

