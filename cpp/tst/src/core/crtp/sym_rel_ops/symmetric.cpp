#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

class ct_sym_rel : public supl::sym_rel_ops<ct_sym_rel>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  constexpr ct_sym_rel(int value)
      : m_value {value}
  { }

  friend constexpr auto operator==(const ct_sym_rel& lhs,
                                   const ct_sym_rel& rhs) noexcept -> bool
  {
    return lhs.m_value == rhs.m_value;
  }

  friend constexpr auto operator<(const ct_sym_rel& lhs,
                                  const ct_sym_rel& rhs) noexcept -> bool
  {
    return lhs.m_value < rhs.m_value;
  }
};

auto main() -> int
{
  supl::test_results results;

  constexpr static ct_sym_rel ct_rel_sym {5};

  constexpr static bool sym_5_eq_B {5 == ct_rel_sym};
  constexpr static bool sym_5_neq_B {5 != ct_rel_sym};
  constexpr static bool sym_5_lt_B {5 < ct_rel_sym};
  constexpr static bool sym_5_leq_B {5 <= ct_rel_sym};
  constexpr static bool sym_5_gt_B {5 > ct_rel_sym};
  constexpr static bool sym_5_geq_B {5 >= ct_rel_sym};
  constexpr static bool sym_8_eq_B {8 == ct_rel_sym};
  constexpr static bool sym_8_neq_B {8 != ct_rel_sym};
  constexpr static bool sym_8_lt_B {8 < ct_rel_sym};
  constexpr static bool sym_8_leq_B {8 <= ct_rel_sym};
  constexpr static bool sym_8_gt_B {8 > ct_rel_sym};
  constexpr static bool sym_8_geq_B {8 >= ct_rel_sym};

  results.enforce_true(sym_5_eq_B);
  results.enforce_false(sym_5_neq_B);
  results.enforce_false(sym_5_lt_B);
  results.enforce_true(sym_5_leq_B);
  results.enforce_false(sym_5_gt_B);
  results.enforce_true(sym_5_geq_B);
  results.enforce_false(sym_8_eq_B);
  results.enforce_true(sym_8_neq_B);
  results.enforce_false(sym_8_lt_B);
  results.enforce_false(sym_8_leq_B);
  results.enforce_true(sym_8_gt_B);
  results.enforce_true(sym_8_geq_B);

  return results.print_and_return();
}
