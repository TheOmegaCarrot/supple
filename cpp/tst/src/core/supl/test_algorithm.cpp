#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

#include <supl/algorithm.hpp>
#include <supl/utility.hpp>

#include "supl/test_algorithm.h"

static auto test_min_size() -> supl::test_results
{
  supl::test_results results;
  const std::array<int, 5> test1 {};
  const std::array<int, 8> test2 {};
  const std::array<int, 3> test3 {};
  const std::array<int, 6> test4 {};

  using supl::literals::operator""_z;
  results.enforce_exactly_equal(
    supl::min_size(test1, test2, test3, test4), 3_z
  );

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
  results.enforce_exactly_equal(
    supl::max_size(test1, test2, test3, test4), 8_z
  );

  return results;
}

static auto test_contains() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test1 {1, 2, 3, 4, 5, 6};

  results.enforce_exactly_equal(
    supl::contains(test1.begin(), test1.end(), 2), true, "Contains 2"
  );
  results.enforce_exactly_equal(
    supl::contains(test1.begin(), test1.end(), 42),
    false,
    "Does not contain 42"
  );

  const std::vector<std::int64_t> test2 {1, 2, 3, 4, 5, 6};

  results.enforce_exactly_equal(
    supl::contains(test2.begin(), test2.end(), 2), true, "Contains 2"
  );
  results.enforce_exactly_equal(
    supl::contains(test2.begin(), test2.end(), 42),
    false,
    "Does not contain 42"
  );

  return results;
}

static auto test_transform_if() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> test_output;
  const std::vector<int> reference_output {6, 12, 18, 24};
  auto is_even {[](int value) {
    return value % 2 == 0;
  }};
  auto times_three {[](int value) -> int {
    return value * 3;
  }};

  supl::transform_if(
    test_input.cbegin(),
    test_input.cend(),
    std::back_inserter(test_output),
    is_even,
    times_three
  );

  results.enforce_exactly_equal(test_output, reference_output);

  supl::for_each_both(
    test_output.cbegin(),
    test_output.cend(),
    reference_output.cbegin(),
    reference_output.cend(),
    [&results](int test, int ref) {
      results.enforce_exactly_equal(test, ref);
    }
  );

  return results;
}

static auto test_for_each_adjacent() -> supl::test_results
{
  supl::test_results results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6};
  std::vector<int> test_output;
  const std::vector<int> reference_output {3, 5, 7, 9, 11};

  supl::for_each_adjacent(
    test_input.begin(),
    test_input.end(),
    [&test_output](const int i, const int j) {
      test_output.push_back(i + j);
    }
  );

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
    test_input.begin(),
    3,
    [&test_output](const int i, const int j) {
      test_output.push_back(i + j);
    }
  );

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_for_each_all() -> supl::test_results
{
  supl::test_results results;

  const std::array test1 {4, 9, 16, 25};
  const std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  const std::vector reference_output {2, 3, 4, 5};

  supl::for_each_all(
    [&test_output](const int a, const int b) {
      test_output.push_back(a / b);
    },
    test1,
    test2
  );

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_for_each_all_c() -> supl::test_results
{
  supl::test_results results;

  const std::array test1 {4, 9, 16, 25};
  const std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  const std::vector reference_output {2, 3, 4, 5};

  supl::for_each_all_c(
    [&test_output](const int a, const int b) {
      test_output.push_back(a / b);
    },
    test1,
    test2
  );

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
    test2.cbegin()
  );

  results.enforce_exactly_equal(test_output, reference_output);

  return results;
}

static auto test_for_each_both() -> supl::test_results
{
  supl::test_results results;

  const std::array<int, 5> test_input_1 {1, 2, 3, 4, 5};
  const std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  const std::vector reference_output {11, 22, 33, 44};

  supl::for_each_both(
    test_input_1.cbegin(),
    test_input_1.cend(),
    test_input_2.cbegin(),
    test_input_2.cend(),
    [&test_output](const auto& i, const auto& j) {
      test_output.push_back(i + j);
    }
  );

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

  supl::for_each_both_n(
    test_input_1.cbegin(),
    test_input_2.cbegin(),
    2,
    [&test_output](const auto& i, const auto& j) {
      test_output.push_back(i + j);
    }
  );

  results.enforce_exactly_equal(test_output, reference_output);

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
  std::generate(ref.begin(), ref.end(), [i {0}]() mutable { return ++i; });

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
    }
  );

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
      oitr, retval.end(), "Returned iterator not correct"
    );
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
  section.add_test("supl::for_each_all", &test_for_each_all);
  section.add_test("supl::for_each_all_c", &test_for_each_all_c);
  section.add_test("supl::for_each_both", &test_for_each_both);
  section.add_test("supl::for_each_both_n", &test_for_each_both_n);
  section.add_test("supl::generate", &test_generate);
  section.add_test("supl::copy", &test_copy);

  return section;
}
