#include <supl/test_runner.hpp>

auto testfunc1() -> supl::test_results
{
  supl::test_results results;

  results.enforce_exactly_equal(2, 2, "pass");

  return results;
}

auto section1() -> supl::test_section
{
  supl::test_section section;
  section.add_test("test 1", &testfunc1);
  return section;
}

auto testfunc2() -> supl::test_results
{
  supl::test_results results;

  results.enforce_exactly_equal(2, 3, "fail");

  return results;
}

auto section2() -> supl::test_section
{
  supl::test_section section;
  section.add_test("test 2", &testfunc2);
  return section;
}

auto main() -> int
{
  supl::test_runner runner;
  runner.add_section(section1());
  runner.add_section(section2());

  return runner.run();
}
