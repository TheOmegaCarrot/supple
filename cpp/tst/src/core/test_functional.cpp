#include <functional>
#include <memory>

#include <supl/fake_ranges.hpp>
#include <supl/functional.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>

auto test_functional() -> supl::test_section
{
  supl::test_section section;

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_functional());

  return runner.run();
}
