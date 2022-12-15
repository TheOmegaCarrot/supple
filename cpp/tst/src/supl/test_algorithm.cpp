#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

#include "supl/algorithm.hpp"
#include "supl/test_algorithm.h"
#include "supl/utility.hpp"

static auto test_min_size() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::array<int, 8> test2 {};
  std::array<int, 3> test3 {};
  std::array<int, 6> test4 {};

  using supl::literals::operator""_z;
  results.add_case(supl::min_size(test1, test2, test3, test4), 3_z);

  return results;
}

static auto test_max_size() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::array<int, 8> test2 {};
  std::array<int, 3> test3 {};
  std::array<int, 6> test4 {};

  using supl::literals::operator""_z;
  results.add_case(supl::max_size(test1, test2, test3, test4), 8_z);

  return results;
}

static auto test_contains() -> ehanc::test
{
  ehanc::test results;

  std::vector<int> test1 {1, 2, 3, 4, 5, 6};

  results.add_case(supl::contains(test1.begin(), test1.end(), 2), true,
                   "Contains 2");
  results.add_case(supl::contains(test1.begin(), test1.end(), 42), false,
                   "Does not contain 42");

  std::vector<std::int64_t> test2 {1, 2, 3, 4, 5, 6};

  results.add_case(supl::contains(test2.begin(), test2.end(), 2), true,
                   "Contains 2");
  results.add_case(supl::contains(test2.begin(), test2.end(), 42), false,
                   "Does not contain 42");

  return results;
}

static auto test_transform_if() -> ehanc::test
{
  ehanc::test results;

  const std::vector<int> test_input {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> test_output;
  const std::vector<int> reference_output {6, 12, 18, 24};
  auto is_even {[](int value) { return value % 2 == 0; }};
  auto times_three {[](int value) -> int { return value * 3; }};

  supl::transform_if(test_input.cbegin(), test_input.cend(),
                     std::back_inserter(test_output), is_even,
                     times_three);

  supl::for_each_both(
      test_output.cbegin(), test_output.cend(), reference_output.cbegin(),
      reference_output.cend(),
      [&results](int test, int ref) { results.add_case(test, ref); });

  return results;
}

static auto test_for_each_adjacent() -> ehanc::test
{
  ehanc::test results;
  std::vector<int> test_input(6);
  std::iota(test_input.begin(), test_input.end(), 1);
  std::vector<int> test_output;
  std::vector<int> reference_output {3, 5, 7, 9, 11};

  supl::for_each_adjacent(test_input.cbegin(), test_input.cend(),
                          [&test_output](const int i, const int j) {
                            test_output.push_back(i + j);
                          });

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_adjacent_n() -> ehanc::test
{
  ehanc::test results;
  std::vector<int> test_input {1, 2, 3, 4, 5, 6};
  std::vector<int> test_output;
  std::vector<int> reference_output {3, 5, 7};

  supl::for_each_adjacent_n(test_input.cbegin(), 3,
                            [&test_output](const int i, const int j) {
                              test_output.push_back(i + j);
                            });
  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_all() -> ehanc::test
{
  ehanc::test results;
  std::array test1 {4, 9, 16, 25};
  std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  std::vector reference_output {2, 3, 4, 5};

  supl::for_each_all(
      [&test_output](const int a, const int b) {
        test_output.push_back(a / b);
      },
      test1, test2);

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_all_c() -> ehanc::test
{
  ehanc::test results;
  const std::array test1 {4, 9, 16, 25};
  const std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  std::vector reference_output {2, 3, 4, 5};

  supl::for_each_all_c(
      [&test_output](const int a, const int b) {
        test_output.push_back(a / b);
      },
      test1, test2);

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_all_n() -> ehanc::test
{
  ehanc::test results;
  std::array test1 {4, 9, 16, 25};
  std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  std::vector reference_output {2, 3, 4};

  supl::for_each_all_n(
      [&test_output](const int a, const int b) {
        test_output.push_back(a / b);
      },
      3, test1.cbegin(), test2.cbegin());

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_both() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test_input_1 {};
  std::iota(test_input_1.begin(), test_input_1.end(), 1);
  std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  std::array<int, 4> reference_output {11, 22, 33, 44};

  supl::for_each_both(test_input_1.cbegin(), test_input_1.cend(),
                      test_input_2.cbegin(), test_input_2.cend(),
                      [&test_output](const auto& i, const auto& j) {
                        test_output.push_back(i + j);
                      });

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_both_n() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test_input_1 {};
  std::iota(test_input_1.begin(), test_input_1.end(), 1);
  std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  std::array<int, 2> reference_output {11, 22};

  supl::for_each_both_n(test_input_1.cbegin(), test_input_2.cbegin(), 2,
                        [&test_output](const auto& i, const auto& j) {
                          test_output.push_back(i + j);
                        });

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_generate() -> ehanc::test
{
  ehanc::test results;

  std::vector<int> test(10);
  std::vector<int> ref(10);

  supl::generate(test.begin(), test.end(),
                 [i {0}]() mutable { return ++i; });
  std::generate(ref.begin(), ref.end(), [i {0}]() mutable { return ++i; });

  supl::for_each_both(
      test.begin(), test.end(), ref.begin(), ref.end(),
      [&results, index {0}](const auto t, const auto r) mutable {
        const std::string case_string([&index]() {
          std::string tmp("Index ");
          tmp += std::to_string(index);
          ++index;
          return tmp;
        }()); // IILE

        results.add_case(t, r, case_string);
      });

  return results;
}

void test_algorithm()
{
  ehanc::run_test("supl::min_size", &test_min_size);
  ehanc::run_test("supl::max_size", &test_max_size);
  ehanc::run_test("supl::contains", &test_contains);
  ehanc::run_test("supl::transform_if", &test_transform_if);
  ehanc::run_test("supl::for_each_adjacent", &test_for_each_adjacent);
  ehanc::run_test("supl::for_each_adjacent_n", &test_for_each_adjacent_n);
  ehanc::run_test("supl::for_each_all_n", &test_for_each_all_n);
  ehanc::run_test("supl::for_each_all", &test_for_each_all);
  ehanc::run_test("supl::for_each_all_c", &test_for_each_all_c);
  ehanc::run_test("supl::for_each_both", &test_for_each_both);
  ehanc::run_test("supl::for_each_both_n", &test_for_each_both_n);
  ehanc::run_test("supl::::generate", &test_generate);
}
