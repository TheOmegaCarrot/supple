#include "supl/test_tuple_algo.h"
#include "supl/metaprogramming.hpp"
#include "supl/tuple_algo.hpp"
#include "test_utils.hpp"

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

  supl::for_each_in_tuple(
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
  using supl::literals::size_t_literal::operator""_z;

  ehanc::test results;

  /*constexpr static*/ std::tuple<int, char, bool> test1 {42, 'c', false};

  /*constexpr static*/ auto out1 {supl::tuple_transform(
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

  auto out2 {supl::tuple_transform(
      test2,
      overload {[](const std::string& str) { return str.length(); },
                [](const std::vector<int>& vec) { return vec.size(); }})};

  results.add_case(std::get<0>(out2), 5_z, "out2 : 0");
  results.add_case(std::get<1>(out2), 3_z, "out2 : 1");

  std::pair<std::string, std::vector<int>> test3 {
      "Hello", {3, 9, 3}
  };

  auto out3 {supl::tuple_transform(
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
      supl::tuple_any_of(test1,
                         overload {[](const int i) { return i < 10; },
                                   [](const char c) { return c == 'c'; },
                                   [](const auto&) { return false; }

                         }),
      true);

  results.add_case(
      supl::tuple_any_of(test1,
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
      supl::tuple_all_of(test1,
                         overload {[](const bool b) { return not b; },
                                   [](const auto& i) { return i > 2; }}),
      true);

  results.add_case(
      supl::tuple_all_of(test1,
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
      supl::tuple_none_of(test1,
                          overload {[](const int i) { return i < 10; },
                                    [](const char c) { return c == 'c'; },
                                    [](const auto&) { return false; }

                          }),
      false);

  results.add_case(
      supl::tuple_none_of(test1,
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

  auto result1 {supl::tuple_push_back(test1, true)};

  results.add_case(result1, expected1);

  int ref_test {5};
  std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  std::tuple<int&, int, char, bool, double> expected2 {ref_test, 3, 'g',
                                                       true, 3.14};

  auto result2 {supl::tuple_push_back(test2, 3.14)};

  results.add_case(result2, expected2);

  std::tuple expected3 {3, 3.14, 'd', true, 42, nullptr};
  auto result3 {supl::tuple_push_back(test1, true, 42, nullptr)};

  results.add_case(result3, expected3);

  return results;
}

static auto test_tuple_pop_back() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {3, 3.14};

  auto result1 {supl::tuple_pop_back(test1)};

  results.add_case(result1, expected1);

  int ref_test {5};
  std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  std::tuple<int&, int, char> expected2 {ref_test, 3, 'g'};

  auto result2 {supl::tuple_pop_back(test2)};

  results.add_case(result2, expected2);

  return results;
}

static auto test_tuple_push_front() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {true, 3, 3.14, 'd'};

  auto result1 {supl::tuple_push_front(test1, true)};

  results.add_case(result1, expected1);

  int ref_test {5};
  std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  std::tuple<double, int&, int, char, bool> expected2 {3.14, ref_test, 3,
                                                       'g', true};

  auto result2 {supl::tuple_push_front(test2, 3.14)};

  results.add_case(result2, expected2);

  std::tuple expected3 {true, 42, nullptr, 3, 3.14, 'd'};
  auto result3 {supl::tuple_push_front(test1, true, 42, nullptr)};

  results.add_case(result3, expected3);

  return results;
}

static auto test_tuple_pop_front() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1 {3, 3.14, 'd'};
  std::tuple expected1 {3.14, 'd'};

  auto result1 {supl::tuple_pop_front(test1)};

  results.add_case(result1, expected1);

  int ref_test {5};
  std::tuple<bool, int&, int, char> test2 {true, ref_test, 3, 'g'};
  std::tuple<int&, int, char> expected2 {ref_test, 3, 'g'};

  auto result2 {supl::tuple_pop_front(test2)};

  results.add_case(result2, expected2);

  return results;
}

static auto test_tuple_rotate_left() -> ehanc::test
{
  ehanc::test results;

  std::tuple test_input {42, false, 3.14, 'w'};

  std::tuple expected1 {false, 3.14, 'w', 42};

  auto result1 {supl::tuple_rotate_left(test_input)};

  results.add_case(result1, expected1);

  return results;
}

static auto test_tuple_rotate_right() -> ehanc::test
{
  ehanc::test results;

  std::tuple test_input {42, false, 3.14, 'w'};

  std::tuple expected1 {'w', 42, false, 3.14};

  auto result1 {supl::tuple_rotate_right(test_input)};

  results.add_case(result1, expected1);

  return results;
}

static auto test_tuple_insert() -> ehanc::test
{
  ehanc::test results;

  std::tuple test_input {3, 3.14};
  std::tuple expected1 {3, true, 3.14};

  auto result1 {supl::tuple_insert<1>(test_input, true)};

  results.add_case(result1, expected1);

  std::tuple expected2 {3, 3.14, true};
  auto result2 {
      supl::tuple_insert<std::tuple_size_v<decltype(test_input)>>(
          test_input, true)};

  results.add_case(result2, expected2, "max index");

  std::tuple expected3 {true, 3, 3.14};
  auto result3 {supl::tuple_insert<0>(test_input, true)};

  results.add_case(result3, expected3);

  std::string neat {"neat"};
  std::vector vec {1, 2, 3, 4};
  std::tuple expected4 {3, 42069, neat, vec, 3.14};
  auto result4 {supl::tuple_insert<1>(test_input, 42069, neat, vec)};

  results.add_case(result4, expected4);

  int ref_test {42};
  std::tuple<const char, int&, double> test_ref_input {'g', ref_test,
                                                       3.14};
  std::tuple<const char, int&, bool, char, double> expected5 {
      'g', ref_test, false, '*', 3.14};
  auto result5 {supl::tuple_insert<2>(test_ref_input, false, '*')};

  results.add_case(result5, expected5);

  std::tuple expected6 {3, 42069, neat, vec, 3.14};
  auto result6 {supl::tuple_insert<1>(test_input, 42069, neat, vec)};

  results.add_case(result6, expected6);

  return results;
}

/* static auto test_tuple_insert_as() -> ehanc::test */
/* { */
/*   ehanc::test results; */

/*   return results; */
/* } */

static auto test_tuple_erase() -> ehanc::test
{
  ehanc::test results;

  constexpr static std::tuple test_input {3, true, 3.14};
  constexpr static std::tuple expected1 {3, 3.14};

  constexpr static auto result1 {supl::tuple_erase<1>(test_input)};

  results.add_case(result1, expected1);

  std::tuple expected2 {3, true};

  auto result2 {supl::tuple_erase<2>(test_input)};

  results.add_case(result2, expected2);

  std::tuple expected3 {true, 3.14};

  auto result3 {supl::tuple_erase<0>(test_input)};

  results.add_case(result3, expected3);

  constexpr static std::tuple<int, decltype(test_input)&, bool>
      test_ref_input {42, test_input, false};

  std::tuple<decltype(test_input)&, bool> expected4 {test_input, false};

  auto result4 {supl::tuple_erase<0>(test_ref_input)};

  results.add_case(result4, expected4);

  std::tuple<int, bool> expected5 {42, false};

  auto result5 {supl::tuple_erase<1>(test_ref_input)};

  results.add_case(result5, expected5);

  constexpr static std::tuple<int, decltype(test_input)&> expected6 {
      42, test_input};

  constexpr static auto result6 {supl::tuple_erase<2>(test_ref_input)};

  results.add_case(result6, expected6);

  constexpr static std::tuple expected7 {3, 3.14};

  constexpr static auto result7 {supl::tuple_erase<1>(test_input)};

  results.add_case(result7, expected7);

  return results;
}

static auto test_tuple_split() -> ehanc::test
{
  ehanc::test results;

  std::string split {"split"};
  std::vector vec {1, 2, 3, 4};
  std::tuple test_input {3, 3.14, vec, true, 'f', split};

  std::tuple expected1_first {3, 3.14, vec};
  std::tuple expected1_second {true, 'f', split};
  auto result1 {supl::tuple_split<3>(test_input)};

  results.add_case(result1.first, expected1_first);
  results.add_case(result1.second, expected1_second);

  std::tuple expected2_first {3, 3.14, vec, true, 'f'};
  std::tuple expected2_second {split};
  auto result2 {supl::tuple_split<5>(test_input)};

  results.add_case(result2.first, expected2_first);
  results.add_case(result2.second, expected2_second);

  int ref_test {42};
  std::tuple<int&, char, bool, double, int&> test_ref_input {
      ref_test, 'y', false, 3.14, ref_test};
  std::tuple<int&, char, bool> expected3_first {ref_test, 'y', false};
  std::tuple<double, int&> expected3_second {3.14, ref_test};

  auto results3 {supl::tuple_split<3>(test_ref_input)};

  results.add_case(results3.first, expected3_first);
  results.add_case(results3.second, expected3_second);

  std::tuple expected4_first {3, 3.14, vec};
  std::tuple expected4_second {true, 'f', split};
  auto result4 {supl::tuple_split<3>(test_input)};

  results.add_case(result4.first, expected4_first);
  results.add_case(result4.second, expected4_second);

  return results;
}

static auto test_tuple_reorder() -> ehanc::test
{
  ehanc::test results;

  std::string reorder {"reorder"};
  std::vector vec {1, 2, 3, 4};
  std::tuple test_input {3, 3.14, true, reorder, vec};
  std::tuple expected1 {3.14, reorder, 3, 3.14, vec, true};

  auto result1 {supl::tuple_reorder<1, 3, 0, 1, 4, 2>(test_input)};

  results.add_case(result1, expected1);

  std::tuple<int, std::vector<int>&, std::string&, char, bool>
      test_ref_input {42, vec, reorder, 'j', true};

  std::tuple<std::vector<int>&, bool, char, std::string&,
             std::vector<int>&, int>
      expected2 {vec, true, 'j', reorder, vec, 42};

  auto result2 {supl::tuple_reorder<1, 4, 3, 2, 1, 0>(test_ref_input)};

  results.add_case(result2, expected2);

  std::tuple expected3 {3.14, reorder, 3, 3.14, vec, true};

  auto result3 {supl::tuple_reorder<1, 3, 0, 1, 4, 2>(test_input)};

  results.add_case(result3, expected3);

  return results;
}

static auto test_subtuple() -> ehanc::test
{
  ehanc::test results;

  std::string str {"subtuple"};
  std::vector vec {1, 2, 3, 4};
  std::tuple test_input {3, true, str, 3.14, vec};

  std::tuple expected1 {str, 3.14, vec};
  auto result1 {supl::subtuple<2, 5>(test_input)};

  results.add_case(result1, expected1);

  std::tuple expected2 {test_input};
  auto result2 {supl::subtuple<0, std::tuple_size_v<decltype(test_input)>>(
      test_input)};

  results.add_case(result2, expected2);

  std::tuple expected3 {3, true, str};
  auto result3 {supl::subtuple<0, 3>(test_input)};

  results.add_case(result3, expected3);

  std::tuple expected4 {3, true, str};
  auto result4 {supl::subtuple<0, 3>(test_input)};

  results.add_case(result4, expected4);

  std::tuple expected5 {str, 3.14, vec};
  auto result5 {supl::subtuple<2, 5>(test_input)};

  results.add_case(result5, expected5);

  std::tuple expected6 {str, 3.14, vec};
  auto result6 {supl::subtuple<2, 5>(test_input)};

  results.add_case(result6, expected6);

  int ref_test {42};
  std::tuple<char, bool, int&, double, char> test_ref_input {
      'j', true, ref_test, 3.14, 'l'};
  std::tuple<bool, int&, double> expected7 {true, ref_test, 3.14};
  auto result7 {supl::subtuple<1, 4>(test_ref_input)};

  results.add_case(result7, expected7);

  std::tuple expected8 {3, true, str};
  auto result8 {supl::subtuple<0, 3>(test_input)};

  results.add_case(result8, expected8);

  return results;
}

static auto test_tuple_count_if() -> ehanc::test
{
  using supl::literals::size_t_literal::operator""_z;

  ehanc::test results;

  std::tuple test {7, 2, 42.53, 3.14F, 9344285UL, -83LL};
  results.add_case(
      supl::tuple_count_if(
          test, [](const auto& i) -> std::size_t { return i > 5; }),
      3_z);

  return results;
}

static auto test_tuple_interleave() -> ehanc::test
{
  ehanc::test results;

  std::tuple test1_1 {42, 'g', 3.14};
  std::tuple test1_2 {2.6F, true, 8L};

  std::tuple expected1 {42, 2.6F, 'g', true, 3.14, 8L};
  auto result1 {supl::tuple_interleave(test1_1, test1_2)};

  results.add_case(result1, expected1);

  using std::string_literals::operator""s;

  std::tuple test2_1 {42, 'g', 3.14, 81, 5.31F, 4UL, false, '7'};
  std::tuple test2_2 {
      2.6F, true, 8L, 8.3F, 2U, 'u', "yes"s, std::vector {3, 5, 7}
  };

  std::tuple expected2 {
      42,    2.6F, 'g', true, 3.14,  8L,     81,  8.3F,
      5.31F, 2U,   4UL, 'u',  false, "yes"s, '7', std::vector {3, 5, 7}
  };
  auto result2 {supl::tuple_interleave(test2_1, test2_2)};

  results.add_case(result2, expected2);

  return results;
}

/* static auto test_tuple_elem_swap() -> ehanc::test */
/* { */
/*   ehanc::test results; */

/*   std::tuple test_input {42, 'g', 3.14, true, 18UL, 2.7F}; */

/*   std::tuple expected1 {42, 18UL, 3.14, true, 'g', 2.7F}; */
/*   auto result1 { */
/*       supl::tuple_elem_swap<1, 4>(test_input)}; */

/*   results.add_case(result1, expected1); */

/*   return results; */
/* } */

void test_tuple_algo()
{
  ehanc::run_test("supl::for_each_in_tuple", &test_for_each_in_tuple);
  ehanc::run_test("supl::tuple_transform", &test_tuple_transform);
  ehanc::run_test("supl::tuple_any_of", &test_tuple_any_of);
  ehanc::run_test("supl::tuple_all_of", &test_tuple_all_of);
  ehanc::run_test("supl::tuple_none_of", &test_tuple_none_of);
  ehanc::run_test("supl::tuple_push_back", &test_tuple_push_back);
  ehanc::run_test("supl::tuple_pop_front", &test_tuple_pop_front);
  ehanc::run_test("supl::tuple_push_front", &test_tuple_push_front);
  ehanc::run_test("supl::tuple_pop_back", &test_tuple_pop_back);
  ehanc::run_test("supl::tuple_rotate_left", &test_tuple_rotate_left);
  ehanc::run_test("supl::tuple_rotate_right", &test_tuple_rotate_right);
  ehanc::run_test("supl::tuple_insert", &test_tuple_insert);
  /* ehanc::run_test("supl::tuple_insert_as", &test_tuple_insert_as); */
  ehanc::run_test("supl::tuple_erase", &test_tuple_erase);
  ehanc::run_test("supl::tuple_reorder", &test_tuple_reorder);
  ehanc::run_test("supl::tuple_split", &test_tuple_split);
  ehanc::run_test("supl::subtuple", &test_subtuple);
  ehanc::run_test("supl::tuple_count_if", &test_tuple_count_if);
  ehanc::run_test("supl::tuple_interleave", &test_tuple_interleave);
  /* ehanc::run_test("supl::tuple_elem_swap", &test_tuple_elem_swap); */
}
