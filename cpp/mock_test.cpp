supl::test_results testfunc()
{
  supl::test_results results;

  results.enforce_exactly_equal(2, 2, "message");

  return results;
}

supl::test_section section1()
{
  supl::test_section retval;
  retval.add_test(&testfunc);
  return retval;
}

supl::test_section section2()
{
  supl::test_section retval;
  retval.add_test(&testfunc);
  return retval;
}

int main()
{
  supl::test_runner runner;
  runner.add_section(section1);
  runner.add_section(section2);

  return runner.run();
}
