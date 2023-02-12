#include <deque>
#include <vector>

#include <supl/algorithm.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::vector input_range1 {1, 3, 5, 7};
  const std::deque input_range2 {4.0, 6.0, 8.0};
  const std::vector expected_output {1, 3, 5, 7, 4, 6, 8};
  std::vector<int> output_range;

  supl::for_each_chain(
    [&output_range](const auto& arg) {
      output_range.push_back(static_cast<int>(arg));
    },
    input_range1.begin(),
    input_range1.end(),
    input_range2.begin(),
    input_range2.end());

  results.enforce_equal(output_range, expected_output);

  return results.print_and_return();
}

