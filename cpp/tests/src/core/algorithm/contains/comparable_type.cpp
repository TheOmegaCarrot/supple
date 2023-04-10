#include <cstdint>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::vector<std::int64_t> test_input {1, 2, 3, 4, 5, 6};

  results.enforce_exactly_equal(
    supl::contains(test_input.begin(), test_input.end(), 2),
    true,
    "Contains 2");
  results.enforce_exactly_equal(
    supl::contains(test_input.begin(), test_input.end(), 42),
    false,
    "Does not contain 42");

  return results.print_and_return();
}
