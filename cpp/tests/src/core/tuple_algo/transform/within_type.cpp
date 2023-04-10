#include <cstddef>

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

  using supl::literals::size_t_literal::operator""_z;

  const std::tuple<int, char, bool> test_input {42, 'c', false};

  const auto test_output {supl::tuple::transform(
    test_input,
    overload {[](int a) {
                return 2 * a;
              },
              [](char b) {
                return static_cast<char>(std::toupper(b));
              },
              [](bool c) {
                return ! c;
              }})};

  results.enforce_exactly_equal(std::get<0>(test_output), 84, "out1 : 0");
  results.enforce_exactly_equal(std::get<1>(test_output), 'C', "out1 : 1");
  results.enforce_exactly_equal(
    std::get<2>(test_output), true, "out1 : 2");

  return results.print_and_return();
}
