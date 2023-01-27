#include <supl/algorithm.hpp>
#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  std::vector<int> test(10);
  std::vector<int> ref(10);

  supl::generate(test.begin(), test.end(), [i {0}]() mutable {
    return ++i;
  });
  std::generate(ref.begin(), ref.end(), [i {0}]() mutable {
    return ++i;
  });

  supl::for_each_both(
    test.begin(),
    test.end(),
    ref.begin(),
    ref.end(),
    [&results, index {0}](const auto t, const auto r) mutable {
      const std::string case_string([&index]() {
        std::string tmp("Index ");
        tmp += std::to_string(index);
        ++index;
        return tmp;
      }());  // IILE

      results.enforce_exactly_equal(t, r, case_string);
    });

  return results.print_and_return();
}
