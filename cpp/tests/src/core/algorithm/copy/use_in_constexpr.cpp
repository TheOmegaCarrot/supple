#include <array>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    constexpr static std::array from_arr {1, 2, 3, 4, 5, 6};

    constexpr static std::array expected1 {1, 2, 3, 4};
    constexpr static std::array result1 {
      [&]()
      {
          std::array<int, 4> retval {};
          supl::copy(
            from_arr.begin(), std::next(from_arr.begin(), 4),
            retval.begin()
          );
          return retval;
      }()
    };  // IILE

    results.enforce_exactly_equal(result1, expected1);

    return results.print_and_return();
}
