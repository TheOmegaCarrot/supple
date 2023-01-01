

supl::test_results testfunc1()
{
  supl::test_results results;

  results.enforce_exactly_equal(2, 2, "pass");

  return results;
}

supl::test_section section1()
{
  supl::test_section section;
  section.add_test("test 1", &testfunc1);
  return section;
}

supl::test_results testfunc2()
{
  supl::test_results results;

  results.enforce_exactly_equal(2, 3, "fail");

  return results;
}

supl::test_section section2()
{
  supl::test_section section;
  section.add_test("test 2", &testfunc2);
  return section;
}

int main()
{
  supl::test_runner runner;
  runner.add_section(section1());
  runner.add_section(section2());

  return runner.run();
}
