#include <array>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    constexpr static std::array cexpr_arr {[]()
                                           {
                                               std::array<int, 10> retval {
                                               };
                                               supl::generate(
                                                 retval.begin(),
                                                 retval.end(),
                                                 [i = 0]() mutable
                                                 {
                                                     return ++i;
                                                 }
                                               );
                                               return retval;
                                           }()};  // IILE

    constexpr static std::array expected_cexpr_arr {1, 2, 3, 4, 5,
                                                    6, 7, 8, 9, 10};

    results.enforce_exactly_equal(cexpr_arr, expected_cexpr_arr);

    return results.print_and_return();
}
