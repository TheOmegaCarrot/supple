#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"

static auto test_construction() -> supl::test_results
{
  supl::test_results results;

  return results;
}

static auto test_was_passed() -> supl::test_results
{
  supl::test_results results;

  return results;
}

auto test_named_params() -> supl::test_section
{
  supl::test_section section;

  section.add_test("named_params construction", &test_construction);
  section.add_test("named_params::was_passed", &test_was_passed);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_named_params());

  return runner.run();
}
