#include "supl/test_results.hpp"

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

  results.enforce_true(supl::tuple::any_of(test_input,
                                           overload {[](const int i) {
                                                       return i < 10;
                                                     },
                                                     [](const char c) {
                                                       return c == 'c';
                                                     },
                                                     [](const auto&) {
                                                       return false;
                                                     }

                                           }));

  results.enforce_false(supl::tuple::any_of(test_input,
                                            overload {[](const auto& i) {
                                                        return i < 0;
                                                      },
                                                      [](const bool b) {
                                                        return b;
                                                      }}));

  return results.print_and_return();
}
