#include <array>
#include <vector>

#include <supl/algorithm.hpp>
#include <supl/functional.hpp>
#include <supl/utility.hpp>

#include <supl/test_runner.hpp>

static auto test_for_each_both() -> supl::test_results
{
  supl::test_results results;

  const std::array<int, 5> test_input_1 {1, 2, 3, 4, 5};
  const std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  const std::vector reference_output {11, 22, 33, 44};

  supl::for_each_both(test_input_1.cbegin(),
                      test_input_1.cend(),
                      test_input_2.cbegin(),
                      test_input_2.cend(),
                      [&test_output](const auto& i, const auto& j) {
                        test_output.push_back(i + j);
                      });

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_for_each_both_n() -> supl::test_results
{
  supl::test_results results;
  const std::array<int, 5> test_input_1 {1, 2, 3, 4, 5};
  const std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  const std::vector reference_output {11, 22};

  supl::for_each_both_n(test_input_1.cbegin(),
                        test_input_2.cbegin(),
                        2,
                        [&test_output](const auto& i, const auto& j) {
                          test_output.push_back(i + j);
                        });

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_all_of_pack() -> supl::test_results
{
  supl::test_results results;

  results.enforce_true(supl::all_of(supl::greater_than(3), 4, 8, 3.14, 42),
                       "All pass case");

  results.enforce_false(supl::all_of(supl::greater_than(3), 4, 8, 3.14, 1),
                        "One argument fails");

  results.enforce_true(supl::all_of(supl::less_than(8)),
                       "No argument case");

  return results;
}

static auto test_any_of_pack() -> supl::test_results
{
  supl::test_results results;

  results.enforce_true(
    supl::any_of(supl::greater_than(42), 2, 18, 1, 4.82, 85.2, 3),
    "One pass case");

  results.enforce_false(
    supl::any_of(supl::greater_than(42), 8, 3.14, 18, 27),
    "None pass case");

  results.enforce_false(supl::any_of(supl::less_than(8)),
                        "No argument case");

  return results;
}

static auto test_none_of_pack() -> supl::test_results
{
  supl::test_results results;

  results.enforce_false(
    supl::none_of(supl::greater_than(42), 2, 18, 1, 4.82, 85.2, 3),
    "One pass case");

  results.enforce_true(
    supl::none_of(supl::greater_than(42), 8, 3.14, 18, 27),
    "None pass case");

  results.enforce_true(supl::none_of(supl::less_than(8)),
                       "No argument case");

  return results;
}

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

  section.add_test("supl::for_each_both", &test_for_each_both);
  section.add_test("supl::for_each_both_n", &test_for_each_both_n);
  section.add_test("supl::all_of (pack)", &test_all_of_pack);
  section.add_test("supl::any_of (pack)", &test_any_of_pack);
  section.add_test("supl::none_of (pack)", &test_none_of_pack);
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
