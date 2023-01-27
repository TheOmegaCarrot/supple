#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

class sym_rel : public supl::sym_rel_ops<sym_rel>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  sym_rel(int value)
      : m_value {value}
  { }

  friend auto operator==(const sym_rel& lhs, const sym_rel& rhs) noexcept
    -> bool
  {
    return lhs.m_value == rhs.m_value;
  }

  friend auto operator<(const sym_rel& lhs, const sym_rel& rhs) noexcept
    -> bool
  {
    return lhs.m_value < rhs.m_value;
  }
};

auto main() -> int
{
  supl::test_results results;

  const sym_rel rel_A {5};
  const sym_rel rel_B {5};
  const sym_rel rel_C {8};

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

