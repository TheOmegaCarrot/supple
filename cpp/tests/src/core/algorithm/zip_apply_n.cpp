#include <array>
#include <vector>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::array test1 {4, 9, 16, 25};
  const std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  const std::vector reference_output {2, 3, 4};

  supl::zip_apply_n(
    [&test_output](const int a, const int b) {
      test_output.push_back(a / b);
    },
    3,
    test1.cbegin(),
    test2.cbegin());

  results.enforce_exactly_equal(test_output, reference_output);

  return results.print_and_return();
}
