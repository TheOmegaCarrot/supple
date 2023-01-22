#include <tuple>
#include <type_traits>

#include <supl/test_results.hpp>
#include <supl/tuple_algo.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::tuple<int, char, bool> test {42, 'c', false};

  supl::tuple::for_each(test,
                        [&results, index {0}](const auto& i) mutable {
                          using I = std::decay_t<decltype(i)>;

                          if constexpr ( std::is_same_v<I, int> ) {
                            results.enforce_equal(i, 42);
                            results.enforce_exactly_equal(index, 0);

                          } else if constexpr ( std::is_same_v<I, char> ) {
                            results.enforce_exactly_equal(i, 'c');
                            results.enforce_equal(index, 1);

                          } else if constexpr ( std::is_same_v<I, bool> ) {
                            results.enforce_equal(i, false);
                            results.enforce_exactly_equal(index, 2);
                          }

                          ++index;
                        });

  return results.print_and_return();
}

