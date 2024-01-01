#include "supl/tuple_algo.hpp"

#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

static auto test_simple_case() -> supl::test_results
{
  supl::test_results results;

  const std::tuple tup1 {42, 'g', true};
  const std::tuple tup2 {18L, false, nullptr, "nope"};
  const std::tuple tup3 {
    'u', std::vector<int> {1, 2, 3},
      8U
  };

  const std::tuple expected1 {42, 18L, 'u'};
  const std::tuple expected2 {
    'g', false, std::vector<int> {1, 2, 3}
  };
  const std::tuple expected3 {true, nullptr, 8U};

  supl::tuple::for_each_all(
    [&, counter = 1](const auto&... args) mutable {
      const std::tuple args_tup {args...};
      switch ( counter ) {
        case 1:
          results.enforce_equal(args_tup, expected1);
          break;
        case 2:
          results.enforce_equal(args_tup, expected2);
          break;
        case 3:
          results.enforce_equal(args_tup, expected3);
          break;
        default:
          results.fail("Unreachable state reached");
          break;
      }
      ++counter;
    },
    tup1,
    tup2,
    tup3);

  return results;
}

static auto test_for_each_all() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::tuple::for_each_all simple case",
                   &test_simple_case);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_for_each_all());

  return runner.run();
}
