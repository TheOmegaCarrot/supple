#include <array>

#include "supl/algorithm.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
    supl::test_results results;

    constexpr static std::array from_arr {1, 2, 3, 4, 5, 6};

    const std::array expected2 {1, 2, 3, 4};
    const std::array result2 {
      [&]()
      {
          std::array<int, 4> retval {};
          std::array<int, 4>::const_iterator oitr {supl::copy(
            from_arr.begin(), std::next(from_arr.begin(), 4),
            retval.begin()
          )};
          results.enforce_true(
            oitr == retval.end(), "Returned iterator not correct"
          );
          return retval;
      }()
    };

    results.enforce_true(result2 == expected2);

    return results.print_and_return();
}
