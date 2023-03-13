#include <supl/crtp.hpp>
#include <supl/test_results.hpp>

struct consumer_both : supl::add_ostream<consumer_both> {
  std::vector<int> value {1, 2, 42, 18};

  [[nodiscard]] auto begin() const noexcept
  {
    return value.begin();
  }

  [[nodiscard]] auto end() const noexcept
  {
    return value.end();
  }

  void to_stream(std::ostream& out) const noexcept
  {
    out << value.front();
  }
};

auto main() -> int
{
  supl::test_results results;

  using std::literals::operator""s;

  const consumer_both test3 {};

  std::stringstream discard;
  test3.to_stream(discard);
  [[maybe_unused]] auto discard_begin {test3.begin()};
  [[maybe_unused]] auto discard_end {test3.end()};

  std::stringstream str3;
  str3 << test3;
  results.enforce_exactly_equal(str3.str(), "1"s);

  return results.print_and_return();
}
