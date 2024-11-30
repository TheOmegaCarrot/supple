#include "supl/crtp.hpp"
#include "supl/test_results.hpp"

class divisible : public supl::division<divisible>
{
private:

    int m_value;

public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr divisible(int value)
            : m_value {value}
    {
    }

    constexpr auto operator/=(const divisible& rhs) noexcept -> divisible&
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

    const divisible A {4};
    const divisible B {A / 2};
    const divisible C {12 / A};
    results.enforce_equal(B.value(), 2);
    results.enforce_equal(C.value(), 3);

    return results.print_and_return();
}
