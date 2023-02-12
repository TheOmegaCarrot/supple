#include <array>
#include <cstddef>

#include <supl/algorithm.hpp>
#include <supl/fake_ranges.hpp>
#include <supl/predicates.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  constexpr static int input_size {8};
  constexpr supl::fr::iota input_iota {0, input_size};

  constexpr static std::array output {[&input_iota]() {
    std::array<int, input_size> tmp {};
    supl::for_each(input_iota.begin(),
                   input_iota.end(),
                   [&tmp, i = std::size_t {0}](auto&& arg) mutable {
                     tmp.at(i++) = arg * 2;
                   });
    return tmp;
  }()};

  constexpr static std::array expected_output {[&input_iota]() {
    std::array<int, input_size> tmp {};
    // supl could probably use a simple constexpr transform
    supl::transform_if(input_iota.begin(),
                       input_iota.end(),
                       tmp.begin(),
                       supl::true_pred,
                       [](auto&& arg) {
                         return arg * 2;
                       });
    return tmp;
  }()};

  results.enforce_equal(output, expected_output);

  return results.print_and_return();
}

