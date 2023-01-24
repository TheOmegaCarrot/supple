#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, 3.14, true};

  const std::tuple<const int&, const double&, const bool&> expected {
    std::get<0>(test_input),
    std::get<1>(test_input),
    std::get<2>(test_input)};

  const auto result {
    supl::tuple::type_transform<supl::make_const_ref>(test_input)};

  results.enforce_exactly_equal(result, expected);

  results.enforce_equal(&std::get<0>(test_input), &std::get<0>(result));

  results.enforce_equal(&std::get<1>(test_input), &std::get<1>(result));

  results.enforce_equal(&std::get<2>(test_input), &std::get<2>(result));

  return results.print_and_return();
}

