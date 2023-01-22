#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const int test_val_3 {81};
  const std::tuple expected3 {81, 3, 3.14, 'd'};
  const auto result3 {supl::tuple::push_front_as<int>(test1, test_val_3)};

  results.enforce_exactly_equal(result3, expected3);

  return results.print_and_return();
}

