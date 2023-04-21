#include <array>

#include "supl/fake_ranges.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  constexpr static std::array<std::size_t, 10> expected {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  constexpr static auto result {[]() {
    std::array<std::size_t, 10> retval {};
    for ( const std::size_t i : supl::fr::iota<std::size_t> {1, 11} ) {
      retval.at(i - 1) = i;
    }
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}
