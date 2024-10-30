#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::tuple const_tuple {3.14, 'g', 42};

    static_assert(std::is_same_v<
                  const int&, decltype(supl::tuple::back(const_tuple))>);
    results.enforce_equal(supl::tuple::back(const_tuple), 42);

    return results.print_and_return();
}
