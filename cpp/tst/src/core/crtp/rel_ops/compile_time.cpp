#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

class ct_rel : public supl::rel_ops<ct_rel>
{
private:

  int m_value;

public:

  constexpr explicit ct_rel(int value)
      : m_value {value}
  { }

  constexpr auto operator==(const ct_rel& rhs) const noexcept -> bool
  {
    return m_value == rhs.m_value;
  }

  constexpr auto operator<(const ct_rel& rhs) const noexcept -> bool
  {
    return m_value < rhs.m_value;
  }
};

auto main() -> int
{
  supl::test_results results;

  constexpr static ct_rel ct_rel_A {5};
  constexpr static ct_rel ct_rel_B {5};
  constexpr static ct_rel ct_rel_C {8};

  constexpr static bool A_eq_B {ct_rel_A == ct_rel_B};
  constexpr static bool A_neq_B {ct_rel_A != ct_rel_B};
  constexpr static bool A_lt_B {ct_rel_A < ct_rel_B};
  constexpr static bool A_leq_B {ct_rel_A <= ct_rel_B};
  constexpr static bool A_gt_B {ct_rel_A > ct_rel_B};
  constexpr static bool A_geq_B {ct_rel_A >= ct_rel_B};
  constexpr static bool C_eq_B {ct_rel_C == ct_rel_B};
  constexpr static bool C_neq_B {ct_rel_C != ct_rel_B};
  constexpr static bool C_lt_B {ct_rel_C < ct_rel_B};
  constexpr static bool C_leq_B {ct_rel_C <= ct_rel_B};
  constexpr static bool C_gt_B {ct_rel_C > ct_rel_B};
  constexpr static bool C_geq_B {ct_rel_C >= ct_rel_B};

  results.enforce_true(A_eq_B);
  results.enforce_false(A_neq_B);
  results.enforce_false(A_lt_B);
  results.enforce_true(A_leq_B);
  results.enforce_false(A_gt_B);
  results.enforce_true(A_geq_B);
  results.enforce_false(C_eq_B);
  results.enforce_true(C_neq_B);
  results.enforce_false(C_lt_B);
  results.enforce_false(C_leq_B);
  results.enforce_true(C_gt_B);
  results.enforce_true(C_geq_B);

  return results.print_and_return();
}

