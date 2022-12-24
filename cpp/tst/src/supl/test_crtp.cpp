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

  return results;
}

void test_crtp()
{
  ehanc::run_test("supl::rel_ops", &test_rel_ops);
}
