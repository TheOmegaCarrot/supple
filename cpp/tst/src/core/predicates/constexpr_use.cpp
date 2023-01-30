#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  constexpr static auto simple_pred {supl::greater_than(3)
                                     && supl::less_than(8)};

  constexpr static bool t {simple_pred(5)};
  constexpr static bool f {simple_pred(10)};

  static_assert(t);
  static_assert(not f);
  results.enforce_true(t);
  results.enforce_false(f);

  return results.print_and_return();
}

