#include "supl/crtp.hpp"
#include "supl/test_results.hpp"

struct consumer_iterators : supl::add_ostream<consumer_iterators> {
  std::vector<int> value {1, 2, 42, 18};

  // NOLINTNEXTLINE(*unused*)
  [[nodiscard]] auto begin() const noexcept
  {
    return value.begin();
  }

  // NOLINTNEXTLINE(*unused*)
  [[nodiscard]] auto end() const noexcept
  {
    return value.end();
  }
};

auto main() -> int
{
  supl::test_results results;

  using std::literals::operator""s;

  const consumer_iterators test2 {};

  std::stringstream str2;
  str2 << test2;
  results.enforce_exactly_equal(str2.str(), "[ 1, 2, 42, 18 ]"s);

  return results.print_and_return();
}
