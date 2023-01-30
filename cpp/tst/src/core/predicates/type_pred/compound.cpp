#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto is_integral {supl::type_pred<std::is_integral>()};
  const auto integral_greater_than_5 {is_integral
                                      && supl::greater_than(5)};

  results.enforce_true(integral_greater_than_5(8));
  results.enforce_false(integral_greater_than_5(8.32));
  results.enforce_false(integral_greater_than_5(3));
  results.enforce_false(integral_greater_than_5(3.14));

  return results.print_and_return();
}
