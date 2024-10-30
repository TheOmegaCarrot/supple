#include "supl/crtp.hpp"
#include "supl/test_results.hpp"

struct consumer_to_stream : supl::add_ostream<consumer_to_stream>
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

    const consumer_to_stream test1 {};

    std::stringstream str1;
    str1 << test1;
    results.enforce_equal(str1.str(), "42");

    return results.print_and_return();
}
