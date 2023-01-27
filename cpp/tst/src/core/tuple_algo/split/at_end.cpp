#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::string a_string {"split"};
  const std::vector a_vector {1, 2, 3, 4};
  const std::tuple test_input {3, 3.14, a_vector, true, 'f', a_string};
  const auto& expected_first {test_input};
  const std::tuple<> expected_second {};
  const auto result {
    supl::tuple::split<std::tuple_size_v<decltype(test_input)>>(
      test_input)};

  results.enforce_exactly_equal(result.first, expected_first);
  results.enforce_exactly_equal(result.second, expected_second);

  return results.print_and_return();
}
