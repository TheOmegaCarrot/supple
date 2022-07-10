#include <array>
#include <numeric>
#include <vector>

#include "test_algorithm.h"

auto
test_min_size() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::array<int, 8> test2 {};
  std::array<int, 3> test3 {};
  std::array<int, 6> test4 {};

  results.add_case(ehanc::min_size(test1, test2, test3, test4),
                   std::size_t {3});

  return results;
}

auto
test_for_each_adjacent() -> ehanc::test
{
  ehanc::test results;
  std::vector<int> test_input(6);
  std::iota(test_input.begin(), test_input.end(), 1);
  std::vector<int> test_output;
  std::vector<int> reference_output {3, 5, 7, 9, 11};

  ehanc::for_each_adjacent(test_input.cbegin(), test_input.cend(),
                           [&test_output](const int i, const int j) {
                             test_output.push_back(i + j);
                           });

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

auto
test_for_each_adjacent_n() -> ehanc::test
{
  ehanc::test results;
  std::vector<int> test_input {1, 2, 3, 4, 5, 6};
  std::vector<int> test_output;
  std::vector<int> reference_output {3, 5, 7};

  ehanc::for_each_adjacent_n(test_input.cbegin(), test_input.cend(), 3,
                             [&test_output](const int i, const int j) {
                               test_output.push_back(i + j);
                             });

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

auto
test_for_each_all() -> ehanc::test
{
  ehanc::test results;
  std::array test1 {4, 9, 16, 25};
  std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  std::vector reference_output {2, 3, 4, 5};

  ehanc::for_each_all(
      [&test_output](const int a, const int b) {
        test_output.push_back(a / b);
      },
      test1, test2);

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

auto
test_for_each_all_c() -> ehanc::test
{
  ehanc::test results;
  const std::array test1 {4, 9, 16, 25};
  const std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  std::vector reference_output {2, 3, 4, 5};

  ehanc::for_each_all_c(
      [&test_output](const int a, const int b) {
        test_output.push_back(a / b);
      },
      test1, test2);

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

auto
test_for_each_all_n() -> ehanc::test
{
  ehanc::test results;
  std::array test1 {4, 9, 16, 25};
  std::array test2 {2, 3, 4, 5};
  std::vector<int> test_output;
  std::vector reference_output {2, 3, 4};

  ehanc::for_each_all_n(
      [&test_output](const int a, const int b) {
        test_output.push_back(a / b);
      },
      3, test1.cbegin(), test2.cbegin());

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

auto
test_for_each_both() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test_input_1 {};
  std::iota(test_input_1.begin(), test_input_1.end(), 1);
  std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  std::array<int, 4> reference_output {11, 22, 33, 44};

  ehanc::for_each_both(test_input_1.cbegin(), test_input_1.cend(),
                       test_input_2.cbegin(), test_input_2.cend(),
                       [&test_output](const auto& i, const auto& j) {
                         test_output.push_back(i + j);
                       });

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

auto
test_for_each_both_n() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test_input_1 {};
  std::iota(test_input_1.begin(), test_input_1.end(), 1);
  std::array<int, 4> test_input_2 {10, 20, 30, 40};
  std::vector<int> test_output;
  std::array<int, 2> reference_output {11, 22};

  ehanc::for_each_both_n(test_input_1.cbegin(), test_input_1.cend(),
                         test_input_2.cbegin(), test_input_2.cend(), 2,
                         [&test_output](const auto& i, const auto& j) {
                           test_output.push_back(i + j);
                         });

  auto [test, ref] =
      std::mismatch(test_output.cbegin(), test_output.cend(),
                    reference_output.cbegin(), reference_output.cend());

  results.add_case(test == test_output.cend(), true);
  results.add_case(ref == reference_output.cend(), true);

  return results;
}

void
test_algorithm()
{
  ehanc::run_test("ehanc::min_size", &test_min_size);
  ehanc::run_test("ehanc::for_each_adjacent", &test_for_each_adjacent);
  ehanc::run_test("ehanc::for_each_adjacent_n", &test_for_each_adjacent_n);
  ehanc::run_test("ehanc::for_each_all_n", &test_for_each_all_n);
  ehanc::run_test("ehanc::for_each_all", &test_for_each_all);
  ehanc::run_test("ehanc::for_each_all_c", &test_for_each_all_c);
  ehanc::run_test("ehanc::for_each_both", &test_for_each_both);
  ehanc::run_test("ehanc::for_each_both_n", &test_for_each_both_n);
}
