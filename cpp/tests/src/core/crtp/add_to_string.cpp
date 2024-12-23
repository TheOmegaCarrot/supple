#include "supl/crtp.hpp"
#include "supl/test_results.hpp"

struct consumer : supl::add_to_string<consumer>
{
    int value {42};

    // NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
        out << value;
    }
};

auto main() -> int
{
    supl::test_results results;

    results.enforce_equal(consumer {}.to_string(), "42");

    return results.print_and_return();
}
