#include <supl/fake_ranges.hpp>
#include <supl/predicates.hpp>
#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>

static auto test_pred_implies() -> supl::test_results
{
  supl::test_results results;

  const auto equals_3_imples_less_than_6 {
    supl::pred_implies(supl::equal_to(3), supl::less_than(6))};

  for ( const int i : supl::fr::iota(0, 11) ) {
    results.enforce_true(equals_3_imples_less_than_6(i),
                         "Equal to 3 implies less than 6: "
                           + std::to_string(i));
  }

  const auto greater_than_6_implies_equals_3 {
    supl::pred_implies(supl::greater_than(6), supl::equal_to(3))};

  results.enforce_true(greater_than_6_implies_equals_3(4),
                       "Greater than 6 implies equal to 3: 4");

  results.enforce_true(greater_than_6_implies_equals_3(5),
                       "Greater than 6 implies equal to 3: 5");

  results.enforce_true(greater_than_6_implies_equals_3(6),
                       "Greater than 6 implies equal to 3: 6");

  results.enforce_false(greater_than_6_implies_equals_3(7),
                        "Greater than 6 implies equal to 3: 7");

  results.enforce_false(greater_than_6_implies_equals_3(8),
                        "Greater than 6 implies equal to 3: 8");

  return results;
}

static auto test_pred_bicond() -> supl::test_results
{
  supl::test_results results;

  const auto less_than_6_bicond_equals_3 {
    supl::pred_bicond(supl::less_than(6), supl::equal_to(3))};

  results.enforce_false(less_than_6_bicond_equals_3(5), "5 < 6 ^ 5 == 3");
  results.enforce_true(less_than_6_bicond_equals_3(3), "3 < 6 ^ 3 == 3");
  results.enforce_true(less_than_6_bicond_equals_3(10),
                       "10 < 6 ^ 10 == 3");

  const auto greater_than_8_bicond_equals_3 {
    supl::pred_bicond(supl::greater_than(8), supl::equal_to(3))};

  results.enforce_false(greater_than_8_bicond_equals_3(10),
                        "10 > 8 ^ 10 == 3");
  results.enforce_false(greater_than_8_bicond_equals_3(3),
                        "3 > 8 ^ 3 == 3");
  results.enforce_true(greater_than_8_bicond_equals_3(5),
                       "5 > 8 ^ 5 == 3");

  return results;
}

static auto test_pred_op_not() -> supl::test_results
{
  supl::test_results results;

  const auto greater_eq_5 {not supl::less_than(5)};

  results.enforce_false(greater_eq_5(0), "0 >= 5");
  results.enforce_false(greater_eq_5(1), "1 >= 5");
  results.enforce_false(greater_eq_5(2), "2 >= 5");
  results.enforce_false(greater_eq_5(3), "3 >= 5");
  results.enforce_false(greater_eq_5(4), "4 >= 5");
  results.enforce_true(greater_eq_5(5), "5 >= 5");
  results.enforce_true(greater_eq_5(6), "6 >= 5");
  results.enforce_true(greater_eq_5(7), "7 >= 5");
  results.enforce_true(greater_eq_5(8), "8 >= 5");
  results.enforce_true(greater_eq_5(9), "9 >= 5");

  return results;
}

static auto test_pred_op_and() -> supl::test_results
{
  supl::test_results results;

  const auto between_3_and_8 {supl::greater_than(3) && supl::less_than(8)};

  results.enforce_false(between_3_and_8(0), "3 < 0 < 8");
  results.enforce_false(between_3_and_8(1), "3 < 1 < 8");
  results.enforce_false(between_3_and_8(2), "3 < 2 < 8");
  results.enforce_false(between_3_and_8(3), "3 < 3 < 8");
  results.enforce_true(between_3_and_8(4), "3 < 4 < 8");
  results.enforce_true(between_3_and_8(5), "3 < 5 < 8");
  results.enforce_true(between_3_and_8(6), "3 < 6 < 8");
  results.enforce_true(between_3_and_8(7), "3 < 7 < 8");
  results.enforce_false(between_3_and_8(8), "3 < 8 < 8");
  results.enforce_false(between_3_and_8(9), "3 < 9 < 8");
  results.enforce_false(between_3_and_8(10), "3 < 10 < 8");

  return results;
}

static auto test_pred_op_or() -> supl::test_results
{
  supl::test_results results;

  const auto not_between_3_and_8 {supl::less_than(3)
                                  || supl::greater_than(8)};

  results.enforce_true(not_between_3_and_8(0), "3 > 0 || 0 > 8");
  results.enforce_true(not_between_3_and_8(1), "3 > 1 || 1 > 8");
  results.enforce_true(not_between_3_and_8(2), "3 > 2 || 2 > 8");
  results.enforce_false(not_between_3_and_8(3), "3 > 3 || 3 > 8");
  results.enforce_false(not_between_3_and_8(4), "3 > 4 || 4 > 8");
  results.enforce_false(not_between_3_and_8(5), "3 > 5 || 5 > 8");
  results.enforce_false(not_between_3_and_8(6), "3 > 6 || 6 > 8");
  results.enforce_false(not_between_3_and_8(7), "3 > 7 || 7 > 8");
  results.enforce_false(not_between_3_and_8(8), "3 > 8 || 8 > 8");
  results.enforce_true(not_between_3_and_8(9), "3 > 9 || 9 > 8");
  results.enforce_true(not_between_3_and_8(10), "3 > 10 || 10 > 8");

  return results;
}

static auto test_pred_op_xor() -> supl::test_results
{
  supl::test_results results;

  const auto less_than_6_xor_equals_3 {supl::less_than(6)
                                       ^ supl::equal_to(3)};

  results.enforce_true(less_than_6_xor_equals_3(5), "5 < 6 ^ 5 == 3");
  results.enforce_false(less_than_6_xor_equals_3(3), "3 < 6 ^ 3 == 3");
  results.enforce_false(less_than_6_xor_equals_3(10), "10 < 6 ^ 10 == 3");

  const auto greater_than_8_xor_equals_3 {
    supl::pred_xor(supl::greater_than(8), supl::equal_to(3))};

  results.enforce_true(greater_than_8_xor_equals_3(10),
                       "10 > 8 ^ 10 == 3");
  results.enforce_true(greater_than_8_xor_equals_3(3), "3 > 8 ^ 3 == 3");
  results.enforce_false(greater_than_8_xor_equals_3(5), "5 > 8 ^ 5 == 3");

  return results;
}

static auto test_pred_op_compound() -> supl::test_results
{
  supl::test_results results;

  const auto test_pred_1 {(supl::greater_eq(3) && supl::less_eq(7))
                          || (supl::greater_eq(12) && supl::less_eq(17))};

  results.enforce_false(test_pred_1(0), "0 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(1), "1 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(2), "2 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(3), "3 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(4), "4 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(5), "5 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(6), "6 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(7), "7 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(8), "8 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(9), "9 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(10), "10 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(11), "11 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(12), "12 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(13), "13 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(14), "14 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(15), "15 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(16), "16 not in ([3, 7] U [12, 17])");
  results.enforce_true(test_pred_1(17), "17 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(18), "18 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(19), "19 not in ([3, 7] U [12, 17])");
  results.enforce_false(test_pred_1(20), "20 not in ([3, 7] U [12, 17])");

  results.enforce_true((supl::greater_than(3) && supl::less_than(8))(5));

  return results;
}

static auto test_apply_operator() -> supl::test_results
{
  supl::test_results results;

  results.enforce_true(5 | supl::less_than(10));
  results.enforce_true(5 | (supl::less_than(10) && supl::greater_than(0)));
  results.enforce_true(5.32
                       | (supl::less_than(10) && supl::greater_than(0)));
  results.enforce_false(20
                        | (supl::less_than(10) && supl::greater_than(0)));

  return results;
}

static auto test_constexpr() -> supl::test_results
{
  supl::test_results results;

  constexpr static auto simple_pred {supl::greater_than(3)
                                     && supl::less_than(8)};

  constexpr static bool t {simple_pred(5)};
  constexpr static bool f {simple_pred(10)};

  static_assert(t);
  static_assert(not f);
  results.enforce_true(t);
  results.enforce_false(f);

  return results;
}

auto test_predicates() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::pred_implies", &test_pred_implies);
  section.add_test("supl::pred_bicond", &test_pred_bicond);
  section.add_test("predicate operator !", &test_pred_op_not);
  section.add_test("predicate operator &&", &test_pred_op_and);
  section.add_test("predicate operator ||", &test_pred_op_or);
  section.add_test("predicate operator ^", &test_pred_op_xor);
  section.add_test("predicate multiple operators", &test_pred_op_compound);
  section.add_test("predicate operator |", &test_apply_operator);
  section.add_test("constexpr predicates", &test_constexpr);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_predicates());

  return runner.run();
}
