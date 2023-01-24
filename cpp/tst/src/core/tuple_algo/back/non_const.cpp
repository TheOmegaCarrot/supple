#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  std::tuple not_const {3.14, 'g', 42};

  static_assert(
    std::is_same_v<int&, decltype(supl::tuple::back(not_const))>);
  results.enforce_equal(supl::tuple::back(not_const), 42);

  return results.print_and_return();
}

