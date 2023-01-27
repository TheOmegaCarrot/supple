#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

class mathematical : public supl::arithmetic<mathematical>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  constexpr mathematical(int value)
      : m_value {value}
  { }

  constexpr auto operator+=(const mathematical& rhs) noexcept
    -> mathematical&
  {
    m_value += rhs.m_value;
    return *this;
  }

  constexpr auto operator-=(const mathematical& rhs) noexcept
    -> mathematical&
  {
    m_value -= rhs.m_value;
    return *this;
  }

  constexpr auto operator*=(const mathematical& rhs) noexcept
    -> mathematical&
  {
    m_value *= rhs.m_value;
    return *this;
  }

  constexpr auto operator/=(const mathematical& rhs) noexcept
    -> mathematical&
  {
    m_value /= rhs.m_value;
    return *this;
  }

  [[nodiscard]] constexpr auto value() const noexcept -> int
  {
    return m_value;
  }
};

auto main() -> int
{
  supl::test_results results;

  const mathematical A {4};

  const mathematical add_l {A + 2};
  const mathematical add_r {2 + A};
  results.enforce_equal(add_l.value(), 6);
  results.enforce_equal(add_r.value(), 6);

  const mathematical sub_l {A - 2};
  const mathematical sub_r {2 - A};
  results.enforce_equal(sub_l.value(), 2);
  results.enforce_equal(sub_r.value(), -2);

  const mathematical mult_l {A * 2};
  const mathematical mult_r {2 * A};
  results.enforce_equal(mult_l.value(), 8);
  results.enforce_equal(mult_r.value(), 8);

  const mathematical div_l {A / 2};
  const mathematical div_r {16 / A};
  results.enforce_equal(div_l.value(), 2);
  results.enforce_equal(div_r.value(), 4);

  return results.print_and_return();
}
