#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  std::tuple<const int, char, double> const_element {42, 'g', 3.14};

  static_assert(
    std::is_same_v<const int&,
                   decltype(supl::tuple::front(const_element))>);
  results.enforce_equal(supl::tuple::front(const_element), 42);

  return results.print_and_return();
}
