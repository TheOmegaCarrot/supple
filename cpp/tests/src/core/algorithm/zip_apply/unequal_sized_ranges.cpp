#include <array>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::array test2_1 {4, 9, 16, 25};
    const std::array test2_2 {2, 3, 4, 5};
    const std::array test2_3 {5, 6, 7, 8, 0x00F};
    std::vector<int> test_output2;
    const std::vector reference_output2 {11, 18, 27, 38};

    supl::zip_apply(
      [&test_output2](const int a, const int b, const int c)
      {
          test_output2.push_back(a + b + c);
      },
      test2_1.begin(), test2_1.end(), test2_2.begin(), test2_2.end(),
      test2_3.begin(), test2_3.end()
    );

    results.enforce_equal(test_output2, reference_output2);

    return results.print_and_return();
}
