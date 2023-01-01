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

int main()
{
  return section1().run() + section2().run();
}
