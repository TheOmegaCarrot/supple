#include <functional>

#include "supl/functional.hpp"
#include "supl/test_functional.h"
#include "test_utils.hpp"

auto test_equal_to() -> ehanc::test
{
  ehanc::test results;

  auto equal_to_5 {supl::equal_to(5)};

  results.add_case(equal_to_5(3), false, "(3 == 5) == false");
  results.add_case(equal_to_5(7), false, "(7 == 5) == false");
  results.add_case(equal_to_5(5), true, "(5 == 5) == true");

  return results;
}

auto test_not_equal_to() -> ehanc::test
{
  ehanc::test results;

  auto not_equal_to_5 {supl::not_equal_to(5)};

  results.add_case(not_equal_to_5(3), true, "(3 != 5) == true");
  results.add_case(not_equal_to_5(7), true, "(7 != 5) == true");
  results.add_case(not_equal_to_5(5), false, "(5 != 5) == false");

  return results;
}

auto test_greater_than() -> ehanc::test
{
  ehanc::test results;

  auto greater_than_5 {supl::greater_than(5)};

  results.add_case(greater_than_5(3), false, "3 > 5 == false");
  results.add_case(greater_than_5(7), true, "7 > 5 == true");
  results.add_case(greater_than_5(5), false, "5 > 5 == false");

  return results;
}

auto test_greater_eq_than() -> ehanc::test
{
  ehanc::test results;

  auto greater_eq_than_5 {supl::greater_eq_than(5)};

  results.add_case(greater_eq_than_5(3), false, "3 >= 5 == false");
  results.add_case(greater_eq_than_5(7), true, "7 >= 5 == true");
  results.add_case(greater_eq_than_5(5), true, "5 >= 5 == true");

  return results;
}

auto test_less_than() -> ehanc::test
{
  ehanc::test results;

  auto less_than_5 {supl::less_than(5)};

  results.add_case(less_than_5(3), true, "3 < 5 == true");
  results.add_case(less_than_5(7), false, "7 < 5 == false");
  results.add_case(less_than_5(5), false, "5 < 5 == false");

  return results;
}

auto test_less_eq_than() -> ehanc::test
{
  ehanc::test results;

  auto less_eq_than_5 {supl::less_eq_than(5)};

  results.add_case(less_eq_than_5(3), true, "3 <= 5 == true");
  results.add_case(less_eq_than_5(7), false, "7 <= 5 == false");
  results.add_case(less_eq_than_5(5), true, "5 <= 5 == true");

  return results;
}

void test_functional()
{
  ehanc::run_test("supl::equal_to", &test_equal_to);
  ehanc::run_test("supl::not_equal_to", &test_not_equal_to);
  ehanc::run_test("supl::greater_than", &test_greater_than);
  ehanc::run_test("supl::greater_eq_than", &test_greater_eq_than);
  ehanc::run_test("supl::less_than", &test_less_than);
  ehanc::run_test("supl::less_eq_than", &test_less_eq_than);
}
