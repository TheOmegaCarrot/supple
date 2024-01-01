#include "supl/algorithm.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

static auto test_min() -> supl::test_results
{
  supl::test_results results;

  results.enforce_equal(supl::min(1, 2, 3, 4, 5), 1);
  results.enforce_equal(supl::min(5, 4, 3, 2, 1), 1);
  results.enforce_equal(supl::min(5, 1, 3, 2, 4), 1);
  results.enforce_equal(supl::min(5, 2, 3, 1, 4), 1);
  results.enforce_equal(supl::min(5, 2, 3, 1, 4), 1);
  results.enforce_equal(supl::min(5, 2, 3, 1, 4, 1), 1);
  results.enforce_equal(supl::min(1, 5, 2, 3, 1, 4), 1);
  results.enforce_equal(supl::min(1, 5, 2, 1, 2, 8, 10, 3, 1, 4), 1);
  results.enforce_equal(supl::min(5, 2, 3, 2, 4), 2);

  return results;
}

static auto test_max() -> supl::test_results
{
  supl::test_results results;

  results.enforce_equal(supl::max(5, 2, 3, 4, 1), 5);
  results.enforce_equal(supl::max(1, 4, 3, 2, 5), 5);
  results.enforce_equal(supl::max(1, 5, 3, 2, 4), 5);
  results.enforce_equal(supl::max(1, 2, 3, 5, 4), 5);
  results.enforce_equal(supl::max(1, 2, 3, 5, 4), 5);
  results.enforce_equal(supl::max(1, 2, 3, 5, 4, 5), 5);
  results.enforce_equal(supl::max(5, 1, 2, 3, 5, 4), 5);
  results.enforce_equal(supl::max(5, 1, 2, 5, 2, 2, 1, 3, 5, 4), 5);
  results.enforce_equal(supl::max(1, 4, 3, 2, 4), 4);

  return results;
}

static auto test_min_max() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::min", &test_min);
  section.add_test("supl::max", &test_max);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_min_max());

  return runner.run();
}
