#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto with_a_lambda {
    supl::pred_and(supl::greater_eq(3), [](auto&& arg) {
      return arg <= 8;
    })};

  results.enforce_true(with_a_lambda(5), "with lambda: 3 <= 5 <= 8");
  results.enforce_true(with_a_lambda(3), "with lambda: 3 <= 3 <= 8");
  results.enforce_true(with_a_lambda(7), "with lambda: 3 <= 7 <= 8");
  results.enforce_true(with_a_lambda(4), "with lambda: 3 <= 4 <= 8");
  results.enforce_true(with_a_lambda(8), "with lambda: 3 <= 8 <= 8");
  results.enforce_false(with_a_lambda(1), "with lambda: 3 <= 1 <= 8");
  results.enforce_false(with_a_lambda(10), "with lambda: 3 <= 10 <= 8");

  return results.print_and_return();
}
