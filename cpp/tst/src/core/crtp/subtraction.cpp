#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

class subtractable : public supl::subtraction<subtractable>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  constexpr subtractable(int value)
      : m_value {value}
  { }

  constexpr auto operator-=(const subtractable& rhs) noexcept
    -> subtractable&
  {
    m_value -= rhs.m_value;
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

  const subtractable A {2};
  const subtractable B {A - 5};
  const subtractable C {5 - A};
  results.enforce_equal(B.value(), -3);
  results.enforce_equal(C.value(), 3);

  return results.print_and_return();
}
