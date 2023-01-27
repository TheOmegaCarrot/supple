#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

class multiplicable : public supl::multiplication<multiplicable>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  constexpr multiplicable(int value)
      : m_value {value}
  { }

  constexpr auto operator*=(const multiplicable& rhs) noexcept
    -> multiplicable&
  {
    m_value *= rhs.m_value;
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

  const multiplicable A {2};
  const multiplicable B {A * 5};
  const multiplicable C {5 * A};
  results.enforce_equal(B.value(), 10);
  results.enforce_equal(C.value(), 10);

  return results.print_and_return();
}

