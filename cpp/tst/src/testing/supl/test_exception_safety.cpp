#include <supl/exception_safety.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>
#include <type_traits>

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
    supl::throws_on_copy to_be_copy_assigned {};
    to_be_copy_assigned = thrower;
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

static_assert(
  not std::is_nothrow_copy_constructible_v<supl::throws_on_copy>);
static_assert(not std::is_nothrow_copy_assignable_v<supl::throws_on_copy>);
static_assert(std::is_nothrow_move_constructible_v<supl::throws_on_copy>);
static_assert(std::is_nothrow_move_assignable_v<supl::throws_on_copy>);

auto test_throws_on_move() -> supl::test_results
{
  supl::test_results results;

  const supl::throws_on_move thrower {};

  try {
    // NOLINTNEXTLINE
    supl::throws_on_move another_thrower {};
    [[maybe_unused]] const supl::throws_on_move move_constructed {
      std::move(another_thrower)};
    results.fail("Did not throw after move construction");
  } catch ( const supl::guaranteed_exception& ) { }

  try {
    supl::throws_on_move another_thrower {};
    supl::throws_on_move to_be_move_assigned {};
    to_be_move_assigned = std::move(another_thrower);
    results.fail("Did not throw after move assignment");
  } catch ( const supl::guaranteed_exception& ) { }

  try {
    [[maybe_unused]] const supl::throws_on_move a_copy {thrower};
  } catch ( const supl::guaranteed_exception& ) {
    results.fail("Threw after copy construction");
  }

  try {
    supl::throws_on_move to_be_copy_assigned {};
    to_be_copy_assigned = thrower;
  } catch ( const supl::guaranteed_exception& ) {
    results.fail("Threw after copy assignment");
  }

  return results;
}

static_assert(std::is_nothrow_copy_constructible_v<supl::throws_on_move>);
static_assert(std::is_nothrow_copy_assignable_v<supl::throws_on_move>);
static_assert(
  not std::is_nothrow_move_constructible_v<supl::throws_on_move>);
static_assert(not std::is_nothrow_move_assignable_v<supl::throws_on_move>);

auto test_exception_safety() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::throws_on_copy", &test_throws_on_copy);
  section.add_test("supl::throws_on_move", &test_throws_on_move);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_exception_safety());

  return runner.run();
}

