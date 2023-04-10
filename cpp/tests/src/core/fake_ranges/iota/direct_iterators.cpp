#include "supl/fake_ranges.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const std::array<std::size_t, 10> expected {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto result {[]() {
    std::array<std::size_t, 10> retval {};
    std::copy(supl::fr::iota<std::size_t>::iterator {1},
              supl::fr::iota<std::size_t>::iterator {11},
              retval.begin());
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}
