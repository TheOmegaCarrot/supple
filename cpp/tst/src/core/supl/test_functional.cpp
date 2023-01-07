#include <functional>
#include <memory>

#include <supl/functional.hpp>

#include <supl/test_runner.hpp>

static auto test_equal_to() -> supl::test_results
{
  supl::test_results results;

  auto equal_to_5 {supl::equal_to(5)};

  results.enforce_exactly_equal(equal_to_5(3), false, "(3 == 5) == false");
  results.enforce_exactly_equal(equal_to_5(7), false, "(7 == 5) == false");
  results.enforce_exactly_equal(equal_to_5(5), true, "(5 == 5) == true");

  return results;
}

static auto test_not_equal_to() -> supl::test_results
{
  supl::test_results results;

  auto not_equal_to_5 {supl::not_equal_to(5)};

  results.enforce_exactly_equal(
    not_equal_to_5(3), true, "(3 != 5) == true"
  );
  results.enforce_exactly_equal(
    not_equal_to_5(7), true, "(7 != 5) == true"
  );
  results.enforce_exactly_equal(
    not_equal_to_5(5), false, "(5 != 5) == false"
  );

  return results;
}

static auto test_greater_than() -> supl::test_results
{
  supl::test_results results;

  auto greater_than_5 {supl::greater_than(5)};

  results.enforce_exactly_equal(
    greater_than_5(3), false, "3 > 5 == false"
  );
  results.enforce_exactly_equal(greater_than_5(7), true, "7 > 5 == true");
  results.enforce_exactly_equal(
    greater_than_5(5), false, "5 > 5 == false"
  );

  return results;
}

static auto test_greater_eq_than() -> supl::test_results
{
  supl::test_results results;

  auto greater_eq_than_5 {supl::greater_eq_than(5)};

  results.enforce_exactly_equal(
    greater_eq_than_5(3), false, "3 >= 5 == false"
  );
  results.enforce_exactly_equal(
    greater_eq_than_5(7), true, "7 >= 5 == true"
  );
  results.enforce_exactly_equal(
    greater_eq_than_5(5), true, "5 >= 5 == true"
  );

  return results;
}

static auto test_less_than() -> supl::test_results
{
  supl::test_results results;

  auto less_than_5 {supl::less_than(5)};

  results.enforce_exactly_equal(less_than_5(3), true, "3 < 5 == true");
  results.enforce_exactly_equal(less_than_5(7), false, "7 < 5 == false");
  results.enforce_exactly_equal(less_than_5(5), false, "5 < 5 == false");

  return results;
}

static auto test_less_eq_than() -> supl::test_results
{
  supl::test_results results;

  auto less_eq_than_5 {supl::less_eq_than(5)};

  results.enforce_exactly_equal(less_eq_than_5(3), true, "3 <= 5 == true");
  results.enforce_exactly_equal(
    less_eq_than_5(7), false, "7 <= 5 == false"
  );
  results.enforce_exactly_equal(less_eq_than_5(5), true, "5 <= 5 == true");

  return results;
}

static auto test_unary_conjunction() -> supl::test_results
{
  supl::test_results results;

  auto between_3_and_8_inclusive {supl::unary_conjunction(
    supl::greater_eq_than(3), supl::less_eq_than(8)
  )};

  results.enforce_true(between_3_and_8_inclusive(5), "3 <= 5 <= 8");
  results.enforce_true(between_3_and_8_inclusive(3), "3 <= 3 <= 8");
  results.enforce_true(between_3_and_8_inclusive(7), "3 <= 7 <= 8");
  results.enforce_true(between_3_and_8_inclusive(4), "3 <= 4 <= 8");
  results.enforce_true(between_3_and_8_inclusive(8), "3 <= 8 <= 8");
  results.enforce_false(between_3_and_8_inclusive(1), "not 3 <= 1 <= 8");
  results.enforce_false(between_3_and_8_inclusive(10), "not 3 <= 10 <= 8");

  return results;
}

auto test_functional() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::equal_to", &test_equal_to);
  section.add_test("supl::not_equal_to", &test_not_equal_to);
  section.add_test("supl::greater_than", &test_greater_than);
  section.add_test("supl::greater_eq_than", &test_greater_eq_than);
  section.add_test("supl::less_than", &test_less_than);
  section.add_test("supl::less_eq_than", &test_less_eq_than);
  section.add_test("supl::unary_conjunction", &test_unary_conjunction);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_functional());

  return runner.run();
}
