#include <array>
#include <vector>

#include <supl/algorithm.hpp>
#include <supl/functional.hpp>
#include <supl/utility.hpp>

#include <supl/test_runner.hpp>

static auto test_generate() -> supl::test_results
{
  supl::test_results results;

  // compare_to_std
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

  // use_in_constexpr
  constexpr static std::array cexpr_arr {[]() {
    std::array<int, 10> retval {};
    supl::generate(retval.begin(), retval.end(), [i = 0]() mutable {
      return ++i;
    });
    return retval;
  }()};  // IILE

  constexpr static std::array expected_cexpr_arr {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  results.enforce_exactly_equal(cexpr_arr, expected_cexpr_arr);

  return results;
}

static auto test_copy() -> supl::test_results
{
  supl::test_results results;

  // use_in_constexpr
  constexpr static std::array from_arr {1, 2, 3, 4, 5, 6};

  constexpr static std::array expected1 {1, 2, 3, 4};
  constexpr static std::array result1 {[&]() {
    std::array<int, 4> retval {};
    supl::copy(
      from_arr.begin(), std::next(from_arr.begin(), 4), retval.begin());
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result1, expected1);

  // returned_iterator
  const std::array expected2 {1, 2, 3, 4};
  const std::array result2 {[&]() {
    std::array<int, 4> retval {};
    auto* oitr {supl::copy(
      from_arr.begin(), std::next(from_arr.begin(), 4), retval.begin())};
    results.enforce_exactly_equal(
      oitr, retval.end(), "Returned iterator not correct");
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

auto test_algorithm() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::generate", &test_generate);
  section.add_test("supl::copy", &test_copy);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_algorithm());

  return runner.run();
}
