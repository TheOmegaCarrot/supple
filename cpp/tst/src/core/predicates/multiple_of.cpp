#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const auto is_even {supl::multiple_of(2)};

  for ( int i {0}; i < 20; i += 2 ) {
    results.enforce_true(is_even(i), std::to_string(i));
    results.enforce_false(is_even(i + 1), std::to_string(i + 1));
  }

  return results.print_and_return();
}
