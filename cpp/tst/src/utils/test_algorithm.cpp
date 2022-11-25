#include <array>
#include <cstdint>
#include <numeric>
#include <vector>

#include "utils/algorithm.hpp"
#include "utils/etc.hpp"
#include "utils/test_algorithm.h"

static auto test_min_size() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::array<int, 8> test2 {};
  std::array<int, 3> test3 {};
  std::array<int, 6> test4 {};

  using ehanc::literals::operator""_z;
  results.add_case(ehanc::min_size(test1, test2, test3, test4), 3_z);

  return results;
}

static auto test_max_size() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::array<int, 8> test2 {};
  std::array<int, 3> test3 {};
  std::array<int, 6> test4 {};

  using ehanc::literals::operator""_z;
  results.add_case(ehanc::max_size(test1, test2, test3, test4), 8_z);

  return results;
}

static auto test_contains() -> ehanc::test
{
  ehanc::test results;

  std::vector<int> test1 {1, 2, 3, 4, 5, 6};

  results.add_case(ehanc::contains(test1.begin(), test1.end(), 2), true,
                   "Contains 2");
  results.add_case(ehanc::contains(test1.begin(), test1.end(), 42), false,
                   "Does not contain 42");

  std::vector<std::int64_t> test2 {1, 2, 3, 4, 5, 6};

  results.add_case(ehanc::contains(test2.begin(), test2.end(), 2), true,
                   "Contains 2");
  results.add_case(ehanc::contains(test2.begin(), test2.end(), 42), false,
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

  ehanc::transform_if(test_input.cbegin(), test_input.cend(),
                      std::back_inserter(test_output), is_even,
                      times_three);

  ehanc::for_each_both(
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

  ehanc::for_each_adjacent(test_input.cbegin(), test_input.cend(),
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

  ehanc::for_each_adjacent_n(test_input.cbegin(), test_input.cend(), 3,
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

  ehanc::for_each_all(
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

  ehanc::for_each_all_c(
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

  ehanc::for_each_all_n(
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

  ehanc::for_each_both(test_input_1.cbegin(), test_input_1.cend(),
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

  ehanc::for_each_both_n(test_input_1.cbegin(), test_input_1.cend(),
                         test_input_2.cbegin(), test_input_2.cend(), 2,
                         [&test_output](const auto& i, const auto& j) {
                           test_output.push_back(i + j);
                         });

  results.add_case(std::equal(test_output.cbegin(), test_output.cend(),
                              reference_output.cbegin(),
                              reference_output.cend()),
                   true);

  return results;
}

static auto test_for_each_in_tuple() -> ehanc::test
{
  ehanc::test results;

  std::tuple<int, char, bool> test {42, 'c', false};

  ehanc::for_each_in_tuple(
      test, [&results, index {0}](const auto& i) mutable {
        using I = std::decay_t<decltype(i)>;

        if constexpr ( std::is_same_v<I, int> ) {
          results.add_case(static_cast<int>(i), 42);
          results.add_case(index, 0);

        } else if constexpr ( std::is_same_v<I, char> ) {
          results.add_case(static_cast<char>(i), 'c');
          results.add_case(index, 1);

        } else if constexpr ( std::is_same_v<I, bool> ) {
          results.add_case(static_cast<bool>(i), false);
          results.add_case(index, 2);
        }

        ++index;
      });

  return results;
}

template <typename... Ls>
struct overload : Ls... {
  using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

static auto test_tuple_transform() -> ehanc::test
{
  using ehanc::literals::size_t_literal::operator""_z;

  ehanc::test results;

  std::tuple<int, char, bool> test1 {42, 'c', false};

  auto out1 {ehanc::tuple_transform(
      test1,
      overload {[](int a) { return 2 * a; },
                [](char b) { return static_cast<char>(std::toupper(b)); },
                [](bool c) { return !c; }})};

  results.add_case(std::get<0>(out1), 84, "out1 : 0");
  results.add_case(std::get<1>(out1), 'C', "out1 : 1");
  results.add_case(std::get<2>(out1), true, "out1 : 2");

  std::tuple<std::string, std::vector<int>> test2 {
      "Hello", {3, 9, 2}
  };

  auto out2 {ehanc::tuple_transform(
      test2,
      overload {[](const std::string& str) { return str.length(); },
                [](const std::vector<int>& vec) { return vec.size(); }})};

  results.add_case(std::get<0>(out2), 5_z, "out2 : 0");
  results.add_case(std::get<1>(out2), 3_z, "out2 : 1");

  std::pair<std::string, std::vector<int>> test3 {
      "Hello", {3, 9, 3}
  };

  auto out3 {ehanc::tuple_transform(
      test3,
      overload {[](const std::string& str) { return str.length(); },
                [](const std::vector<int>& vec) { return vec.size(); }})};

  results.add_case(std::get<0>(out3), 5_z, "out3 : 0");
  results.add_case(std::get<1>(out3), 3_z, "out3 : 1");

  return results;
}

static auto test_tuple_push_back() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {3, 3.14, 'd', true};

  auto result1 {ehanc::tuple_push_back(test1, true)};

  results.add_case(result1, expected1);

  return results;
}

static auto test_tuple_pop_back() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {3, 3.14};

  auto result1 {ehanc::tuple_pop_back(test1)};

  results.add_case(result1, expected1);

  return results;
}

static auto test_tuple_push_front() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {true, 3, 3.14, 'd'};

  auto result1 {ehanc::tuple_push_front(test1, true)};

  results.add_case(result1, expected1);

  return results;
}

static auto test_tuple_pop_front() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {3.14, 'd'};

  auto result1 {ehanc::tuple_pop_front(test1)};

  results.add_case(result1, expected1);

  return results;
}

static auto test_tuple_count_if() -> ehanc::test
{
  using ehanc::literals::size_t_literal::operator""_z;

  ehanc::test results;

  std::tuple test {7, 2, 42.53, 3.14F, 9344285UL, -83LL};
  results.add_case(
      ehanc::tuple_count_if(
          test, [](const auto& i) -> std::size_t { return i > 5; }),
      3_z);

  return results;
}

static auto test_bkprt_generate() -> ehanc::test
{
  ehanc::test results;

  std::vector<int> test(10);
  std::vector<int> ref(10);

  ehanc::bkprt::generate(test.begin(), test.end(),
                         [i {0}]() mutable { return ++i; });
  std::generate(ref.begin(), ref.end(), [i {0}]() mutable { return ++i; });

  ehanc::for_each_both(
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
  ehanc::run_test("ehanc::min_size", &test_min_size);
  ehanc::run_test("ehanc::max_size", &test_max_size);
  ehanc::run_test("ehanc::contains", &test_contains);
  ehanc::run_test("ehanc::transform_if", &test_transform_if);
  ehanc::run_test("ehanc::for_each_adjacent", &test_for_each_adjacent);
  ehanc::run_test("ehanc::for_each_adjacent_n", &test_for_each_adjacent_n);
  ehanc::run_test("ehanc::for_each_all_n", &test_for_each_all_n);
  ehanc::run_test("ehanc::for_each_all", &test_for_each_all);
  ehanc::run_test("ehanc::for_each_all_c", &test_for_each_all_c);
  ehanc::run_test("ehanc::for_each_both", &test_for_each_both);
  ehanc::run_test("ehanc::for_each_both_n", &test_for_each_both_n);
  ehanc::run_test("ehanc::for_each_in_tuple", &test_for_each_in_tuple);
  ehanc::run_test("ehanc::tuple_transform", &test_tuple_transform);
  ehanc::run_test("ehanc::tuple_push_back", &test_tuple_push_back);
  ehanc::run_test("ehanc::tuple_pop_front", &test_tuple_pop_front);
  ehanc::run_test("ehanc::tuple_push_front", &test_tuple_push_front);
  ehanc::run_test("ehanc::tuple_pop_back", &test_tuple_pop_back);
  ehanc::run_test("ehanc::tuple_count_if", &test_tuple_count_if);
  ehanc::run_test("ehanc::bkprt::generate", &test_bkprt_generate);
}
