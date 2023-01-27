#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple referenced_data {42, 3.14, true};

  const std::tuple<const int&, const double&, const bool&> test_input {
    std::get<0>(referenced_data),
    std::get<1>(referenced_data),
    std::get<2>(referenced_data)};

  const auto& expected {referenced_data};
  const auto result {supl::tuple::type_transform<std::decay>(test_input)};
  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}
