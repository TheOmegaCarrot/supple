#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    const std::tuple<double, char, const int> const_tuple_const_element {
      3.14, 'g', 42
    };

    static_assert(std::is_same_v<
                  const int&,
                  decltype(supl::tuple::back(const_tuple_const_element))>);
    results.enforce_equal(
      supl::tuple::back(const_tuple_const_element), 42
    );

    return results.print_and_return();
}
