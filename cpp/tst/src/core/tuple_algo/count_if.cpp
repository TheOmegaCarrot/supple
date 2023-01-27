#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple test {7, 2, 42.53, 3.14F, 9344285UL, -83LL};

  results.enforce_equal(supl::tuple::count_if(test,
                                              [](const auto& i) {
                                                return i > 5;
                                              }),
                        3);

  return results.print_and_return();
}
