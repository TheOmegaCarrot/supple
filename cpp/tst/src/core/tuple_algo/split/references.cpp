#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  int referenced_data {42};

  const std::tuple<int&, char, bool, double, int&> test_input {
    referenced_data, 'y', false, 3.14, referenced_data};

  const std::tuple<int&, char, bool> expected_first {
    referenced_data, 'y', false};

  const std::tuple<double, int&> expected_second {3.14, referenced_data};

  const auto results3 {supl::tuple::split<3>(test_input)};

  results.enforce_exactly_equal(results3.first, expected_first, "First");
  results.enforce_exactly_equal(
    results3.second, expected_second, "Second");

  return results.print_and_return();
}

