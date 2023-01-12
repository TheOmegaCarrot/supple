#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

#include <supl/algorithm.hpp>
#include <supl/functional.hpp>
#include <supl/utility.hpp>

#include <supl/test_runner.hpp>

static auto test_min_size() -> supl::test_results
{
  supl::test_results results;
  const std::array<int, 5> test1 {};
  const std::array<int, 8> test2 {};
  const std::array<int, 3> test3 {};
  const std::array<int, 6> test4 {};

  using supl::literals::operator""_z;
  results.enforce_exactly_equal(supl::min_size(test1, test2, test3, test4),
                                3_z);

  return results;
}

static auto test_max_size() -> supl::test_results
{
  supl::test_results results;
  const std::array<int, 5> test1 {};
  const std::array<int, 8> test2 {};
  const std::array<int, 3> test3 {};
  const std::array<int, 6> test4 {};

  using supl::literals::operator""_z;
  results.enforce_exactly_equal(supl::max_size(test1, test2, test3, test4),
                                8_z);

  return results;
}

static auto test_contains() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test1 {1, 2, 3, 4, 5, 6};

  results.enforce_exactly_equal(
    supl::contains(test1.begin(), test1.end(), 2), true, "Contains 2");
  results.enforce_exactly_equal(
    supl::contains(test1.begin(), test1.end(), 42),
    false,
    "Does not contain 42");

  const std::vector<std::int64_t> test2 {1, 2, 3, 4, 5, 6};

  results.enforce_exactly_equal(
    supl::contains(test2.begin(), test2.end(), 2), true, "Contains 2");
  results.enforce_exactly_equal(
    supl::contains(test2.begin(), test2.end(), 42),
    false,
    "Does not contain 42");

  return results;
}

static auto test_transform_if() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> test_output;
  const std::vector<int> reference_output {6, 12, 18, 24};
  const auto is_even {[](int value) {
    return value % 2 == 0;
  }};
  const auto times_three {[](auto value) -> decltype(value) {
    return value * 3;
  }};

  supl::transform_if(test_input.cbegin(),
                     test_input.cend(),
                     std::back_inserter(test_output),
                     is_even,
                     times_three);

  results.enforce_exactly_equal(test_output, reference_output);

  supl::for_each_both(test_output.cbegin(),
                      test_output.cend(),
                      reference_output.cbegin(),
                      reference_output.cend(),
                      [&results](int test, int ref) {
                        results.enforce_exactly_equal(test, ref);
                      });

  // this section mostly to test enforce_floating_point_approx

  const std::vector<double> test_double_input {1.0, 2.0, 3.0, 5.0, 8.0};
  std::vector<double> test_double_output;
  const std::vector<double> double_reference_output {15.0, 24.0};

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

  return results;
}

static auto test_for_each_adjacent() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6};
  std::vector<int> test_output;
  const std::vector<int> reference_output {3, 5, 7, 9, 11};

  supl::for_each_adjacent(test_input.begin(),
                          test_input.end(),
                          [&test_output](const int i, const int j) {
                            test_output.push_back(i + j);
                          });

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_for_each_adjacent_n() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6};
  std::vector<int> test_output;
  const std::vector<int> reference_output {3, 5, 7};

  supl::for_each_adjacent_n(
    test_input.begin(), 3, [&test_output](const int i, const int j) {
      test_output.push_back(i + j);
    });

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_for_each_all_n() -> supl::test_results
{
  supl::test_results results;

  const std::array test1 {4, 9, 16, 25};
  const std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  const std::vector reference_output {2, 3, 4};

  supl::for_each_all_n(
    [&test_output](const int a, const int b) {
      test_output.push_back(a / b);
    },
    3,
    test1.cbegin(),
    test2.cbegin());

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_tuple_elementwise_compare_any() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1_1 {1, 2, 3};
  const std::tuple test1_2 {4, 5, 6};
  results.enforce_false(
    supl::impl::tuple_elementwise_compare_any(test1_1, test1_2),
    supl::to_string(test1_1) + " && " + supl::to_string(test1_2));

  const std::tuple test2_1 {1, 2, 3};
  const std::tuple test2_2 {1, 4, 5};
  results.enforce_true(
    supl::impl::tuple_elementwise_compare_any(test2_1, test2_2),
    supl::to_string(test2_1) + " && " + supl::to_string(test2_2));

  const std::tuple test3_1 {1, 2, 3};
  const std::tuple test3_2 {4, 2, 5};
  results.enforce_true(
    supl::impl::tuple_elementwise_compare_any(test3_1, test3_2),
    supl::to_string(test3_1) + " && " + supl::to_string(test3_2));

  const std::tuple test4_1 {1, 2, 3};
  const std::tuple test4_2 {4, 5, 3};
  results.enforce_true(
    supl::impl::tuple_elementwise_compare_any(test4_1, test4_2),
    supl::to_string(test4_1) + " && " + supl::to_string(test4_2));

  return results;
}

static auto test_for_each_all() -> supl::test_results
{
  supl::test_results results;

  const std::array test1_1 {4, 9, 16, 25};
  const std::array test1_2 {2, 3, 4, 5};
  const std::array test1_3 {5, 6, 7, 8};
  std::vector<int> test_output1;
  const std::vector reference_output1 {11, 18, 27, 38};

  supl::for_each_all(
    [&test_output1](const int a, const int b, const int c) {
      test_output1.push_back(a + b + c);
    },
    test1_1.begin(),
    test1_1.end(),
    test1_2.begin(),
    test1_2.end(),
    test1_3.begin(),
    test1_3.end());

  results.enforce_equal(test_output1, reference_output1);

  const std::array test2_1 {4, 9, 16, 25};
  const std::array test2_2 {2, 3, 4, 5};
  const std::array test2_3 {5, 6, 7, 8, 0x00F};
  std::vector<int> test_output2;
  const std::vector reference_output2 {11, 18, 27, 38};

  supl::for_each_all(
    [&test_output2](const int a, const int b, const int c) {
      test_output2.push_back(a + b + c);
    },
    test2_1.begin(),
    test2_1.end(),
    test2_2.begin(),
    test2_2.end(),
    test2_3.begin(),
    test2_3.end());

  results.enforce_equal(test_output2, reference_output2);

  return results;
}

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

  constexpr static std::array from_arr {1, 2, 3, 4, 5, 6};

  constexpr static std::array expected1 {1, 2, 3, 4};
  constexpr static std::array result1 {[&]() {
    std::array<int, 4> retval {};
    supl::copy(from_arr.begin(), from_arr.begin() + 4, retval.begin());
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result1, expected1);

  const std::array expected2 {1, 2, 3, 4};
  const std::array result2 {[&]() {
    std::array<int, 4> retval {};
    auto* oitr {
      supl::copy(from_arr.begin(), from_arr.begin() + 4, retval.begin())};
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

  section.add_test("supl::min_size", &test_min_size);
  section.add_test("supl::max_size", &test_max_size);
  section.add_test("supl::contains", &test_contains);
  section.add_test("supl::transform_if", &test_transform_if);
  section.add_test("supl::for_each_adjacent", &test_for_each_adjacent);
  section.add_test("supl::for_each_adjacent_n", &test_for_each_adjacent_n);
  section.add_test("supl::for_each_all_n", &test_for_each_all_n);
  section.add_test("supl::impl::test_tuple_elementwise_compare_any",
                   &test_tuple_elementwise_compare_any);
  section.add_test("supl::for_each_all", &test_for_each_all);
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
