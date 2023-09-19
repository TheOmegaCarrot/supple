#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"

static auto test_use_defaults() -> supl::test_results
{
  supl::test_results results;

  return results;
}

auto test_named_params() -> supl::test_section
{
  supl::test_section section;

  section.add_test("use defaults", &test_use_defaults);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_named_params());

  return runner.run();
}
