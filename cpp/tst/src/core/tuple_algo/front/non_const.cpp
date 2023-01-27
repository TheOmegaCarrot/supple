#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  std::tuple not_const {42, 'g', 3.14};

  static_assert(
    std::is_same_v<int&, decltype(supl::tuple::front(not_const))>);
  results.enforce_equal(supl::tuple::front(not_const), 42);

  return results.print_and_return();
}
