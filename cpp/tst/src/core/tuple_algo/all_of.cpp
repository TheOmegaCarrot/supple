#include <supl/test_results.hpp>
#include <supl/tuple_algo.hpp>

template <typename... Ls>
struct overload : Ls... {
  using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, 'c', false};

  results.enforce_true(supl::tuple::all_of(test_input,
                                           overload {[](const bool b) {
                                                       return not b;
                                                     },
                                                     [](const auto& i) {
                                                       return i > 2;
                                                     }}));

  results.enforce_false(supl::tuple::all_of(test_input,
                                            overload {[](const bool b) {
                                                        return b;
                                                      },
                                                      [](const auto& i) {
                                                        return i > 2;
                                                      }}));

  return results.print_and_return();
}

