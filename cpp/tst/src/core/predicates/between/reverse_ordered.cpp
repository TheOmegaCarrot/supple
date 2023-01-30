#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto between_8_and_3 {supl::between(8, 3)};

  results.enforce_false(between_8_and_3(0), "0");
  results.enforce_false(between_8_and_3(1), "1");
  results.enforce_false(between_8_and_3(2), "2");
  results.enforce_true(between_8_and_3(3), "3");
  results.enforce_true(between_8_and_3(4), "4");
  results.enforce_true(between_8_and_3(5), "5");
  results.enforce_true(between_8_and_3(6), "6");
  results.enforce_true(between_8_and_3(7), "7");
  results.enforce_true(between_8_and_3(8), "8");
  results.enforce_false(between_8_and_3(9), "9");
  results.enforce_false(between_8_and_3(10), "10");

  return results.print_and_return();
}
