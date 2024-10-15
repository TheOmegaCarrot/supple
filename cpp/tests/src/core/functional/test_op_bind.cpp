#include "supl/functional.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

auto test_op_bind_left() -> supl::test_results
{
  supl::test_results results;

  auto five_plus {supl::op_bind_left<5, std::plus> {}};

  results.enforce_equal(five_plus(3), 8);

  auto ten_minus {supl::op_bind_left<10, std::minus> {}};

  results.enforce_equal(ten_minus(3), 7);

  return results;
}

auto test_op_bind_right() -> supl::test_results
{
  supl::test_results results;

  auto plus_5 {supl::op_bind_right<std::plus, 5> {}};

  results.enforce_equal(plus_5(3), 8);

  auto minus_10 {supl::op_bind_right<std::minus, 10> {}};

  results.enforce_equal(minus_10(17), 7);

  return results;
}

auto test_op_bind() -> supl::test_section
{
  supl::test_section section;

  section.add_test("test_op_bind_left", &test_op_bind_left);
  section.add_test("test_op_bind_right", &test_op_bind_right);

  return section;
}

int main()
{
  supl::test_runner runner;

  runner.add_section(test_op_bind());

  return runner.run();
}
