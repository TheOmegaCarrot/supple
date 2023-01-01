supl::test testfunc()
{
  supl::test results;

  results.enforce_exactly_equal(2, 2, "message");

  return results;
}

supl::section section1()
{
  supl::section retval;
  retval.add_test(&testfunc);
  return retval;
}

supl::section section2()
{
  supl::section retval;
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
