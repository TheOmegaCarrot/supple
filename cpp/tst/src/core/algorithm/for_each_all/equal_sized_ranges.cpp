#include <supl/algorithm.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::array test1_1 {4, 9, 16, 25};
  const std::array test1_2 {2, 3, 4, 5};
  const std::array test1_3 {5, 6, 7, 8};
  std::vector<int> test_output1;
  const std::vector reference_output1 {11, 18, 27, 38};

  supl::for_each_all(
    [&test_output1](const int a, const int b, const int c) {
      test_output1.push_back(a + b + c);
    },
    test1_1.begin(),
    test1_1.end(),
    test1_2.begin(),
    test1_2.end(),
    test1_3.begin(),
    test1_3.end());

  results.enforce_equal(test_output1, reference_output1);

  return results.print_and_return();
}
