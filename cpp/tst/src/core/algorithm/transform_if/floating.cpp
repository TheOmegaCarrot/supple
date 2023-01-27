#include <supl/algorithm.hpp>
#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::vector<double> test_double_input {1.0, 2.0, 3.0, 5.0, 8.0};
  std::vector<double> test_double_output;
  const std::vector<double> double_reference_output {15.0, 24.0};

  const auto times_three {[](auto value) -> decltype(value) {
    return value * 3;
  }};

  supl::transform_if(test_double_input.begin(),
                     test_double_input.end(),
                     std::back_inserter(test_double_output),
                     supl::greater_than(4.0),
                     times_three);

  supl::for_each_both(test_double_output.begin(),
                      test_double_output.end(),
                      double_reference_output.begin(),
                      double_reference_output.end(),
                      [&results](double test, double ref) {
                        results.enforce_floating_point_approx(test, ref);
                      });

  return results.print_and_return();
}
