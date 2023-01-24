#include <supl/test_results.hpp>

struct just_a_bool {
  // NOLINTNEXTLINE(google-explicit-constructor)
  just_a_bool(bool arg)
      : value {arg}
  { }

  bool value;

  // NOLINTNEXTLINE(*unused*)
  void to_stream(std::ostream& out) const noexcept
  {
    out << value;
  }

  // NOLINTNEXTLINE(*unused*)
  [[nodiscard]] constexpr auto operator==(const just_a_bool& rhs) const
    -> bool
  {
    return this->value == rhs.value;
  }
};

auto main() -> int
{
  supl::test_results results;

  constexpr static std::tuple test_input {42, 3.14, true};

  const std::tuple<char, double, just_a_bool> expected {
    '*', 3.14, just_a_bool {true}};

  const auto result {
    supl::tuple::convert<char, double, just_a_bool>(test_input)};

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}

