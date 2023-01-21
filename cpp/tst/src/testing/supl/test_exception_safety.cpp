#include <supl/exception_safety.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>

auto test_throws_on_copy() -> supl::test_results
{
  supl::test_results results;

  const supl::throws_on_copy thrower {};

  try {
    // NOLINTNEXTLINE
    [[maybe_unused]] const supl::throws_on_copy a_copy {thrower};
    results.fail("Did not throw after copy construction");
  } catch ( const supl::guaranteed_exception& ) { }

  try {
    supl::throws_on_copy to_be_assigned_to {};
    to_be_assigned_to = thrower;
    results.fail("Did not throw after copy assignment");
  } catch ( const supl::guaranteed_exception& ) { }

  try {
    supl::throws_on_copy another_thrower {};
    [[maybe_unused]] const supl::throws_on_copy move_constructed {
      std::move(another_thrower)};
  } catch ( const supl::guaranteed_exception& ) {
    results.fail("Threw after move construction");
  }

  try {
    supl::throws_on_copy another_thrower {};
    supl::throws_on_copy to_be_move_assigned {};
    to_be_move_assigned = std::move(another_thrower);
  } catch ( const supl::guaranteed_exception& ) {
    results.fail("Threw after move assignment");
  }

  return results;
}

auto test_exception_safety() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::throws_on_copy", &test_throws_on_copy);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_exception_safety());

  return runner.run();
}

