#include "supl/test_crtp.h"
#include "test_utils.hpp"

static auto test_rel_ops() -> ehanc::test
{
  ehanc::test results;

  class rel : public supl::rel_ops<rel>
  {
  private:

    int m_value;

  public:

    explicit rel(int value)
        : m_value {value}
    {}

    auto operator==(const rel& rhs) const noexcept -> bool
    {
      return m_value == rhs.m_value;
    }

    auto operator<(const rel& rhs) const noexcept -> bool
    {
      return m_value < rhs.m_value;
    }

    using supl::rel_ops<rel>::operator!=;
    using supl::rel_ops<rel>::operator<=;
    using supl::rel_ops<rel>::operator>;
    using supl::rel_ops<rel>::operator>=;
  };

  rel rel_A {5};
  rel rel_B {5};
  rel rel_C {8};

  results.add_case(rel_A == rel_B, true);
  results.add_case(rel_A != rel_B, false);
  results.add_case(rel_A < rel_B, false);
  results.add_case(rel_A <= rel_B, true);
  results.add_case(rel_A > rel_B, false);
  results.add_case(rel_A >= rel_B, true);

  results.add_case(rel_C == rel_B, false);
  results.add_case(rel_C != rel_B, true);
  results.add_case(rel_C < rel_B, false);
  results.add_case(rel_C <= rel_B, false);
  results.add_case(rel_C > rel_B, true);
  results.add_case(rel_C >= rel_B, true);

  // and at compile-time!
  class ct_rel : public supl::rel_ops<ct_rel>
  {
  private:

    int m_value;

  public:

    constexpr explicit ct_rel(int value)
        : m_value {value}
    {}

    constexpr auto operator==(const ct_rel& rhs) const noexcept -> bool
    {
      return m_value == rhs.m_value;
    }

    constexpr auto operator<(const ct_rel& rhs) const noexcept -> bool
    {
      return m_value < rhs.m_value;
    }

    using supl::rel_ops<ct_rel>::operator!=;
    using supl::rel_ops<ct_rel>::operator<=;
    using supl::rel_ops<ct_rel>::operator>;
    using supl::rel_ops<ct_rel>::operator>=;
  };

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

  results.add_case(A_eq_B, true);
  results.add_case(A_neq_B, false);
  results.add_case(A_lt_B, false);
  results.add_case(A_leq_B, true);
  results.add_case(A_gt_B, false);
  results.add_case(A_geq_B, true);
  results.add_case(C_eq_B, false);
  results.add_case(C_neq_B, true);
  results.add_case(C_lt_B, false);
  results.add_case(C_leq_B, false);
  results.add_case(C_gt_B, true);
  results.add_case(C_geq_B, true);

  return results;
}

void test_crtp()
{
  ehanc::run_test("supl::rel_ops", &test_rel_ops);
}
