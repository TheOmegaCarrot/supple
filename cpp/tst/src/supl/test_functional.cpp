#include <functional>

#include "supl/functional.hpp"
#include "supl/test_functional.h"
#include "test_utils.hpp"

static auto test_equal_to() -> ehanc::test
{
  ehanc::test results;

  auto equal_to_5 {supl::equal_to(5)};

  results.add_case(equal_to_5(3), false, "(3 == 5) == false");
  results.add_case(equal_to_5(7), false, "(7 == 5) == false");
  results.add_case(equal_to_5(5), true, "(5 == 5) == true");

  return results;
}

static auto test_not_equal_to() -> ehanc::test
{
  ehanc::test results;

  auto not_equal_to_5 {supl::not_equal_to(5)};

  results.add_case(not_equal_to_5(3), true, "(3 != 5) == true");
  results.add_case(not_equal_to_5(7), true, "(7 != 5) == true");
  results.add_case(not_equal_to_5(5), false, "(5 != 5) == false");

  return results;
}

static auto test_greater_than() -> ehanc::test
{
  ehanc::test results;

  auto greater_than_5 {supl::greater_than(5)};

  results.add_case(greater_than_5(3), false, "3 > 5 == false");
  results.add_case(greater_than_5(7), true, "7 > 5 == true");
  results.add_case(greater_than_5(5), false, "5 > 5 == false");

  return results;
}

static auto test_greater_eq_than() -> ehanc::test
{
  ehanc::test results;

  auto greater_eq_than_5 {supl::greater_eq_than(5)};

  results.add_case(greater_eq_than_5(3), false, "3 >= 5 == false");
  results.add_case(greater_eq_than_5(7), true, "7 >= 5 == true");
  results.add_case(greater_eq_than_5(5), true, "5 >= 5 == true");

  return results;
}

static auto test_less_than() -> ehanc::test
{
  ehanc::test results;

  auto less_than_5 {supl::less_than(5)};

  results.add_case(less_than_5(3), true, "3 < 5 == true");
  results.add_case(less_than_5(7), false, "7 < 5 == false");
  results.add_case(less_than_5(5), false, "5 < 5 == false");

  return results;
}

static auto test_less_eq_than() -> ehanc::test
{
  ehanc::test results;

  auto less_eq_than_5 {supl::less_eq_than(5)};

  results.add_case(less_eq_than_5(3), true, "3 <= 5 == true");
  results.add_case(less_eq_than_5(7), false, "7 <= 5 == false");
  results.add_case(less_eq_than_5(5), true, "5 <= 5 == true");

  return results;
}

static auto test_invoke() -> ehanc::test
{
  ehanc::test results;

  constexpr static int result1 {supl::invoke([]() { return 5; })};
  results.add_case(result1, 5);

  results.add_case(result1, 5);

  constexpr static int result2 {supl::invoke([](int x) { return x; }, 5)};

  results.add_case(result2, 5);

  struct Foo {
    int m_value;

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }

    [[nodiscard]] constexpr auto value_plus(int arg) const noexcept -> int
    {
      return m_value + arg;
    }
  };

  constexpr static Foo foo {5};

  constexpr static int result3 {supl::invoke(&Foo::value, &foo)};

  results.add_case(result3, 5);

  constexpr static int result4 {supl::invoke(&Foo::value_plus, &foo, 2)};

  results.add_case(result4, 7);

  int result5 {supl::invoke(&Foo::value, foo)};

  results.add_case(result5, 5);

  int result6 {supl::invoke(&Foo::value_plus, foo, 2)};

  results.add_case(result6, 7);

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
  ehanc::run_test("supl::invoke", &test_invoke);
}
