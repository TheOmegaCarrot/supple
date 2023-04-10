#include "supl/crtp.hpp"
#include "supl/test_results.hpp"

class rel : public supl::rel_ops<rel>
{
private:

  int m_value;

public:

  explicit rel(int value)
      : m_value {value}
  { }

  auto operator==(const rel& rhs) const noexcept -> bool
  {
    return m_value == rhs.m_value;
  }

  auto operator<(const rel& rhs) const noexcept -> bool
  {
    return m_value < rhs.m_value;
  }
};

auto main() -> int
{
  supl::test_results results;

  const rel rel_A {5};
  const rel rel_B {5};
  const rel rel_C {8};

  results.enforce_true(rel_A == rel_B);
  results.enforce_false(rel_A != rel_B);
  results.enforce_false(rel_A < rel_B);
  results.enforce_true(rel_A <= rel_B);
  results.enforce_false(rel_A > rel_B);
  results.enforce_true(rel_A >= rel_B);

  results.enforce_false(rel_C == rel_B);
  results.enforce_true(rel_C != rel_B);
  results.enforce_false(rel_C < rel_B);
  results.enforce_false(rel_C <= rel_B);
  results.enforce_true(rel_C > rel_B);
  results.enforce_true(rel_C >= rel_B);

  return results.print_and_return();
}
