#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6};
  std::vector<int> test_output;
  const std::vector<int> reference_output {3, 5, 7};

  supl::for_each_adjacent_n(
    test_input.begin(), 3, [&test_output](const int i, const int j) {
      test_output.push_back(i + j);
    });

  results.enforce_exactly_equal(test_output, reference_output);

  return results.print_and_return();
}
