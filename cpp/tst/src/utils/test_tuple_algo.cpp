#include "utils/test_tuple_algo.h"
#include "test_utils.hpp"
#include "utils/metaprogramming.hpp"
#include "utils/tuple_algo.hpp"

template <typename... Ls>
struct overload : Ls... {
  using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

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

static auto test_tuple_any_of() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {42, 'c', false};

  results.add_case(
      ehanc::tuple_any_of(test1,
                          overload {[](const int i) { return i < 10; },
                                    [](const char c) { return c == 'c'; },
                                    [](const auto&) { return false; }

                          }),
      true);

  results.add_case(
      ehanc::tuple_any_of(test1,
                          overload {[](const auto& i) { return i < 0; },
                                    [](const bool b) { return b; }}),
      false);

  return results;
}

static auto test_tuple_all_of() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {42, 'c', false};

  results.add_case(
      ehanc::tuple_all_of(test1,
                          overload {[](const bool b) { return not b; },
                                    [](const auto& i) { return i > 2; }}),
      true);

  results.add_case(
      ehanc::tuple_all_of(test1,
                          overload {[](const bool b) { return b; },
                                    [](const auto& i) { return i > 2; }}),
      false);

  return results;
}

static auto test_tuple_none_of() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {42, 'c', false};

  results.add_case(
      ehanc::tuple_none_of(test1,
                           overload {[](const int i) { return i < 10; },
                                     [](const char c) { return c == 'c'; },
                                     [](const auto&) { return false; }

                           }),
      false);

  results.add_case(
      ehanc::tuple_none_of(test1,
                           overload {[](const auto& i) { return i < 0; },
                                     [](const bool b) { return b; }}),
      true);

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

static auto test_tuple_insert() -> ehanc::test
{
  ehanc::test results;

  std::tuple test_input {3, 3.14};
  std::tuple expected1 {3, true, 3.14};

  auto result1 {
      ehanc::tuple_insert(test_input, ehanc::index_constant<1> {}, true)};

  results.add_case(result1, expected1);

  std::tuple expected2 {3, 3.14, true};
  auto result2 {ehanc::tuple_insert(
      test_input,
      ehanc::index_constant<std::tuple_size_v<decltype(test_input)>> {},
      true)};

  results.add_case(result2, expected2, "max index");

  std::tuple expected3 {true, 3, 3.14};
  auto result3 {
      ehanc::tuple_insert(test_input, ehanc::index_constant<0> {}, true)};

  results.add_case(result3, expected3);

  std::string neat {"neat"};
  std::vector vec {1, 2, 3, 4};
  std::tuple expected4 {3, 42069, neat, vec, 3.14};
  auto result4 {ehanc::tuple_insert(
      test_input, ehanc::index_constant<1> {}, 42069, neat, vec)};

  results.add_case(result4, expected4);

  return results;
}

static auto test_tuple_reorder() -> ehanc::test
{
  ehanc::test results;

  std::string reorder {"reorder"};
  std::vector vec {1, 2, 3, 4};
  std::tuple test_input {3, 3.14, true, reorder, vec};
  std::tuple expected1 {3.14, reorder, 3, 3.14, vec, true};

  auto result1 {ehanc::tuple_reorder(
      test_input, std::index_sequence<1, 3, 0, 1, 4, 2> {})};

  results.add_case(result1, expected1);

  return results;
}

static auto test_subtuple() -> ehanc::test
{
  ehanc::test results;

  std::string str {"subtuple"};
  std::vector vec {1, 2, 3, 4};
  std::tuple test_input {3, true, str, 3.14, vec};

  std::tuple expected1 {str, 3.14, vec};
  auto result1 {ehanc::subtuple(test_input, ehanc::index_constant<2> {},
                                ehanc::index_constant<5> {})};

  results.add_case(result1, expected1);

  std::tuple expected2 {test_input};
  auto result2 {ehanc::subtuple(
      test_input, ehanc::index_constant<0> {},
      ehanc::index_constant<std::tuple_size_v<decltype(test_input)>> {})};

  results.add_case(result2, expected2);

  std::tuple expected3 {3, true, str};
  auto result3 {ehanc::subtuple(test_input, ehanc::index_constant<0> {},
                                ehanc::index_constant<3> {})};

  results.add_case(result3, expected3);

  std::tuple expected4 {3, true, str};
  auto result4 {ehanc::subtuple(test_input, ehanc::index_pair<0, 3> {})};

  results.add_case(result4, expected4);

  std::tuple expected5 {str, 3.14, vec};
  auto result5 {ehanc::subtuple(test_input, ehanc::index_pair<2, 5> {})};

  results.add_case(result5, expected5);

  std::tuple expected6 {str, 3.14, vec};
  auto result6 {ehanc::subtuple<decltype(test_input), 2, 5>(test_input)};

  results.add_case(result6, expected6);

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

void test_tuple_algo()
{
  ehanc::run_test("ehanc::for_each_in_tuple", &test_for_each_in_tuple);
  ehanc::run_test("ehanc::tuple_transform", &test_tuple_transform);
  ehanc::run_test("ehanc::tuple_any_of", &test_tuple_any_of);
  ehanc::run_test("ehanc::tuple_all_of", &test_tuple_all_of);
  ehanc::run_test("ehanc::tuple_none_of", &test_tuple_none_of);
  ehanc::run_test("ehanc::tuple_push_back", &test_tuple_push_back);
  ehanc::run_test("ehanc::tuple_pop_front", &test_tuple_pop_front);
  ehanc::run_test("ehanc::tuple_push_front", &test_tuple_push_front);
  ehanc::run_test("ehanc::tuple_pop_back", &test_tuple_pop_back);
  ehanc::run_test("ehanc::tuple_insert", &test_tuple_insert);
  ehanc::run_test("ehanc::tuple_reorder", &test_tuple_reorder);
  ehanc::run_test("ehanc::subtuple", &test_subtuple);
  ehanc::run_test("ehanc::tuple_count_if", &test_tuple_count_if);
}
