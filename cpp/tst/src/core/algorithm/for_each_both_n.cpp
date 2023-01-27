#include <supl/algorithm.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::array<int, 5> test_input_1 {1, 2, 3, 4, 5};
  const std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  const std::vector reference_output {11, 22};

  supl::for_each_both_n(test_input_1.cbegin(),
                        test_input_2.cbegin(),
                        2,
                        [&test_output](const auto& i, const auto& j) {
                          test_output.push_back(i + j);
                        });

  results.enforce_exactly_equal(test_output, reference_output);

  return results.print_and_return();
}
