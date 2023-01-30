#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto is_integral {supl::type_pred<std::is_integral>()};

  results.enforce_true(is_integral(5));
  results.enforce_false(is_integral(5.54));

  return results.print_and_return();
}
