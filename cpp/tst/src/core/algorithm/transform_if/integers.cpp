#include <supl/algorithm.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> test_output;
  const std::vector<int> reference_output {6, 12, 18, 24};
  const auto is_even {[](int value) {
    return value % 2 == 0;
  }};
  const auto times_three {[](auto value) -> decltype(value) {
    return value * 3;
  }};

  supl::transform_if(test_input.cbegin(),
                     test_input.cend(),
                     std::back_inserter(test_output),
                     is_even,
                     times_three);

  results.enforce_exactly_equal(test_output, reference_output);

  return results.print_and_return();
}
