#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"
#include "supl/utility.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::array<int, 5> test1 {};
  const std::array<int, 8> test2 {};
  const std::array<int, 3> test3 {};
  const std::array<int, 6> test4 {};

  using supl::literals::operator""_z;
  results.enforce_exactly_equal(supl::min_size(test1, test2, test3, test4),
                                3_z);

  return results.print_and_return();
}
