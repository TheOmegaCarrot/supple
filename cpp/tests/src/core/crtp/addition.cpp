#include "supl/crtp.hpp"
#include "supl/test_results.hpp"

class addable : public supl::addition<addable>
{
private:

    int m_value;

public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr addable(int value)
            : m_value {value}
    {
    }

    constexpr auto operator+=(const addable& rhs) noexcept -> addable&
    {
        m_value += rhs.m_value;
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

    const addable A {2};
    const addable B {A + 5};
    const addable C {5 + A};
    results.enforce_equal(B.value(), 7);
    results.enforce_equal(C.value(), 7);

    return results.print_and_return();
}
