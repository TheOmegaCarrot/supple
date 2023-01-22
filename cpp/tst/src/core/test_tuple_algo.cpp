#include <type_traits>

#include <supl/metaprogramming.hpp>
#include <supl/tuple_algo.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>

template <typename... Ls>
struct overload : Ls... {
  using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

static auto test_for_each() -> supl::test_results
{
  supl::test_results results;

  const std::tuple<int, char, bool> test {42, 'c', false};

  supl::tuple::for_each(test,
                        [&results, index {0}](const auto& i) mutable {
                          using I = std::decay_t<decltype(i)>;

                          if constexpr ( std::is_same_v<I, int> ) {
                            results.enforce_equal(i, 42);
                            results.enforce_exactly_equal(index, 0);

                          } else if constexpr ( std::is_same_v<I, char> ) {
                            results.enforce_exactly_equal(i, 'c');
                            results.enforce_equal(index, 1);

                          } else if constexpr ( std::is_same_v<I, bool> ) {
                            results.enforce_equal(i, false);
                            results.enforce_exactly_equal(index, 2);
                          }

                          ++index;
                        });

  return results;
}

static auto test_for_each_in_subtuple() -> supl::test_results
{
  supl::test_results results;

  const std::tuple<int, char, bool> test {42, 'c', false};

  supl::tuple::for_each_in_subtuple<0, 1>(
    test, [&results, index {0}](const auto& i) mutable {
      using I = std::decay_t<decltype(i)>;

      if constexpr ( std::is_same_v<I, int> ) {
        results.enforce_equal(i, 42);
        results.enforce_exactly_equal(index, 0);

      } else if constexpr ( std::is_same_v<I, char> ) {
        results.enforce_equal(i, 'c');
        results.enforce_exactly_equal(index, 1);
      }
      ++index;
    });

  return results;
}

static auto test_transform() -> supl::test_results
{
  using supl::literals::size_t_literal::operator""_z;

  supl::test_results results;

  const std::tuple<int, char, bool> test1 {42, 'c', false};

  const auto out1 {supl::tuple::transform(
    test1,
    overload {[](int a) {
                return 2 * a;
              },
              [](char b) {
                return static_cast<char>(std::toupper(b));
              },
              [](bool c) {
                return ! c;
              }})};

  results.enforce_exactly_equal(std::get<0>(out1), 84, "out1 : 0");
  results.enforce_exactly_equal(std::get<1>(out1), 'C', "out1 : 1");
  results.enforce_exactly_equal(std::get<2>(out1), true, "out1 : 2");

  const std::tuple<std::string, std::vector<int>> test2 {
    "Hello", {3, 9, 2}
  };

  const auto out2 {
    supl::tuple::transform(test2,
                           overload {[](const std::string& str) {
                                       return str.length();
                                     },
                                     [](const std::vector<int>& vec) {
                                       return vec.size();
                                     }})};

  results.enforce_exactly_equal(std::get<0>(out2), 5_z, "out2 : 0");
  results.enforce_exactly_equal(std::get<1>(out2), 3_z, "out2 : 1");

  const std::pair<std::string, std::vector<int>> test3 {
    "Hello", {3, 9, 3}
  };

  const auto out3 {
    supl::tuple::transform(test3,
                           overload {[](const std::string& str) {
                                       return str.length();
                                     },
                                     [](const std::vector<int>& vec) {
                                       return vec.size();
                                     }})};

  results.enforce_exactly_equal(std::get<0>(out3), 5_z, "out3 : 0");
  results.enforce_exactly_equal(std::get<1>(out3), 3_z, "out3 : 1");

  return results;
}

static auto test_any_of() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {42, 'c', false};

  results.enforce_exactly_equal(
    supl::tuple::any_of(test1,
                        overload {[](const int i) {
                                    return i < 10;
                                  },
                                  [](const char c) {
                                    return c == 'c';
                                  },
                                  [](const auto&) {
                                    return false;
                                  }

                        }),
    true);

  results.enforce_exactly_equal(
    supl::tuple::any_of(test1,
                        overload {[](const auto& i) {
                                    return i < 0;
                                  },
                                  [](const bool b) {
                                    return b;
                                  }}),
    false);

  return results;
}

static auto test_all_of() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {42, 'c', false};

  results.enforce_exactly_equal(
    supl::tuple::all_of(test1,
                        overload {[](const bool b) {
                                    return not b;
                                  },
                                  [](const auto& i) {
                                    return i > 2;
                                  }}),
    true);

  results.enforce_exactly_equal(
    supl::tuple::all_of(test1,
                        overload {[](const bool b) {
                                    return b;
                                  },
                                  [](const auto& i) {
                                    return i > 2;
                                  }}),
    false);

  return results;
}

static auto test_none_of() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {42, 'c', false};

  results.enforce_exactly_equal(
    supl::tuple::none_of(test1,
                         overload {[](const int i) {
                                     return i < 10;
                                   },
                                   [](const char c) {
                                     return c == 'c';
                                   },
                                   [](const auto&) {
                                     return false;
                                   }

                         }),
    false);

  results.enforce_exactly_equal(
    supl::tuple::none_of(test1,
                         overload {[](const auto& i) {
                                     return i < 0;
                                   },
                                   [](const bool b) {
                                     return b;
                                   }}),
    true);

  return results;
}

static auto test_push_back() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const std::tuple expected1 {3, 3.14, 'd', true};

  const auto result1 {supl::tuple::push_back(test1, true)};

  results.enforce_exactly_equal(result1, expected1);

  int ref_test {5};
  const std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  const std::tuple<int&, int, char, bool, double> expected2 {
    ref_test, 3, 'g', true, 3.14};

  const auto result2 {supl::tuple::push_back(test2, 3.14)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {3, 3.14, 'd', true, 42, nullptr};
  const auto result3 {supl::tuple::push_back(test1, true, 42, nullptr)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_push_back_as() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const bool test_val_1 {true};
  const std::tuple<int, double, char, const bool&> expected1 {
    3, 3.14, 'd', test_val_1};

  const auto result1 {
    supl::tuple::push_back_as<const bool&>(test1, test_val_1)};

  results.enforce_exactly_equal(result1, expected1);

  int ref_test {5};
  const std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  double test_val_2 {3.14};
  const std::tuple<int&, int, char, bool, double&> expected2 {
    ref_test, 3, 'g', true, test_val_2};

  const auto result2 {
    supl::tuple::push_back_as<double&>(test2, test_val_2)};

  results.enforce_exactly_equal(result2, expected2);

  const int test_val_3 {81};
  const std::tuple expected3 {3, 3.14, 'd', 81};
  const auto result3 {supl::tuple::push_back_as<int>(test1, test_val_3)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_pop_back() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const std::tuple expected1 {3, 3.14};

  const auto result1 {supl::tuple::pop_back(test1)};

  results.enforce_exactly_equal(result1, expected1);

  int ref_test {5};
  const std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  const std::tuple<int&, int, char> expected2 {ref_test, 3, 'g'};

  const auto result2 {supl::tuple::pop_back(test2)};

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_push_front() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const std::tuple expected1 {true, 3, 3.14, 'd'};

  const auto result1 {supl::tuple::push_front(test1, true)};

  results.enforce_exactly_equal(result1, expected1);

  int ref_test {5};
  const std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  const std::tuple<double, int&, int, char, bool> expected2 {
    3.14, ref_test, 3, 'g', true};

  const auto result2 {supl::tuple::push_front(test2, 3.14)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {true, 42, nullptr, 3, 3.14, 'd'};
  const auto result3 {supl::tuple::push_front(test1, true, 42, nullptr)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_push_front_as() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const bool test_val_1 {true};
  const std::tuple<const bool&, int, double, char> expected1 {
    test_val_1, 3, 3.14, 'd'};

  const auto result1 {
    supl::tuple::push_front_as<const bool&>(test1, test_val_1)};

  results.enforce_exactly_equal(result1, expected1);

  int ref_test {5};
  const std::tuple<int&, int, char, bool> test2 {ref_test, 3, 'g', true};
  double test_val_2 {3.14};
  const std::tuple<double&, int&, int, char, bool> expected2 {
    test_val_2, ref_test, 3, 'g', true};

  const auto result2 {
    supl::tuple::push_front_as<double&>(test2, test_val_2)};

  results.enforce_exactly_equal(result2, expected2);

  const int test_val_3 {81};
  const std::tuple expected3 {81, 3, 3.14, 'd'};
  const auto result3 {supl::tuple::push_front_as<int>(test1, test_val_3)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_pop_front() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1 {3, 3.14, 'd'};
  const std::tuple expected1 {3.14, 'd'};

  const auto result1 {supl::tuple::pop_front(test1)};

  results.enforce_exactly_equal(result1, expected1);

  int ref_test {5};
  const std::tuple<bool, int&, int, char> test2 {true, ref_test, 3, 'g'};
  const std::tuple<int&, int, char> expected2 {ref_test, 3, 'g'};

  const auto result2 {supl::tuple::pop_front(test2)};

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_rotate_left() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, false, 3.14, 'w'};

  const std::tuple expected1 {false, 3.14, 'w', 42};

  const auto result1 {supl::tuple::rotate_left(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_rotate_right() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, false, 3.14, 'w'};

  const std::tuple expected1 {'w', 42, false, 3.14};

  const auto result1 {supl::tuple::rotate_right(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_insert() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {3, 3.14};
  const std::tuple expected1 {3, true, 3.14};

  const auto result1 {supl::tuple::insert<1>(test_input, true)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {3, 3.14, true};
  const auto result2 {
    supl::tuple::insert<std::tuple_size_v<decltype(test_input)>>(
      test_input, true)};

  results.enforce_exactly_equal(result2, expected2, "max index");

  const std::tuple expected3 {true, 3, 3.14};
  const auto result3 {supl::tuple::insert<0>(test_input, true)};

  results.enforce_exactly_equal(result3, expected3);

  const std::string neat {"neat"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple expected4 {3, 42069, neat, vec, 3.14};
  const auto result4 {
    supl::tuple::insert<1>(test_input, 42069, neat, vec)};

  results.enforce_exactly_equal(result4, expected4);

  int ref_test {42};
  const std::tuple<const char, int&, double> test_ref_input {
    'g', ref_test, 3.14};
  const std::tuple<const char, int&, bool, char, double> expected5 {
    'g', ref_test, false, '*', 3.14};
  const auto result5 {supl::tuple::insert<2>(test_ref_input, false, '*')};

  results.enforce_exactly_equal(result5, expected5);

  const std::tuple expected6 {3, 42069, neat, vec, 3.14};
  const auto result6 {
    supl::tuple::insert<1>(test_input, 42069, neat, vec)};

  results.enforce_exactly_equal(result6, expected6);

  return results;
}

static auto test_erase() -> supl::test_results
{
  supl::test_results results;

  constexpr static std::tuple test_input {3, true, 3.14};
  constexpr static std::tuple expected1 {3, 3.14};

  constexpr static auto result1 {supl::tuple::erase<1>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {3, true};

  const auto result2 {supl::tuple::erase<2>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {true, 3.14};

  const auto result3 {supl::tuple::erase<0>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  constexpr static std::tuple<int, decltype(test_input)&, bool>
    test_ref_input {42, test_input, false};

  const std::tuple<decltype(test_input)&, bool> expected4 {test_input,
                                                           false};

  const auto result4 {supl::tuple::erase<0>(test_ref_input)};

  results.enforce_exactly_equal(result4, expected4);

  const std::tuple<int, bool> expected5 {42, false};

  const auto result5 {supl::tuple::erase<1>(test_ref_input)};

  results.enforce_exactly_equal(result5, expected5);

  constexpr static std::tuple<int, decltype(test_input)&> expected6 {
    42, test_input};

  constexpr static auto result6 {supl::tuple::erase<2>(test_ref_input)};

  results.enforce_exactly_equal(result6, expected6);

  constexpr static std::tuple expected7 {3, 3.14};

  constexpr static auto result7 {supl::tuple::erase<1>(test_input)};

  results.enforce_exactly_equal(result7, expected7);

  return results;
}

static auto test_replace() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, true, 3.14};
  const std::tuple expected1 {42, 'B', 3.14};
  const auto result1 {supl::tuple::replace<1>(test_input, 'B')};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {'B', true, 3.14};
  const auto result2 {supl::tuple::replace<0>(test_input, 'B')};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {42, true, 'B'};
  const auto result3 {supl::tuple::replace<2, char>(test_input, 'B')};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_split() -> supl::test_results
{
  supl::test_results results;

  const std::string split {"split"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple test_input {3, 3.14, vec, true, 'f', split};

  const std::tuple expected1_first {3, 3.14, vec};
  const std::tuple expected1_second {true, 'f', split};
  const auto result1 {supl::tuple::split<3>(test_input)};

  results.enforce_exactly_equal(result1.first, expected1_first);
  results.enforce_exactly_equal(result1.second, expected1_second);

  const std::tuple expected2_first {3, 3.14, vec, true, 'f'};
  const std::tuple expected2_second {split};
  const auto result2 {supl::tuple::split<5>(test_input)};

  results.enforce_exactly_equal(result2.first, expected2_first);
  results.enforce_exactly_equal(result2.second, expected2_second);

  int ref_test {42};
  const std::tuple<int&, char, bool, double, int&> test_ref_input {
    ref_test, 'y', false, 3.14, ref_test};
  const std::tuple<int&, char, bool> expected3_first {
    ref_test, 'y', false};
  const std::tuple<double, int&> expected3_second {3.14, ref_test};

  const auto results3 {supl::tuple::split<3>(test_ref_input)};

  results.enforce_exactly_equal(results3.first, expected3_first);
  results.enforce_exactly_equal(results3.second, expected3_second);

  const std::tuple expected4_first {3, 3.14, vec};
  const std::tuple expected4_second {true, 'f', split};
  const auto result4 {supl::tuple::split<3>(test_input)};

  results.enforce_exactly_equal(result4.first, expected4_first);
  results.enforce_exactly_equal(result4.second, expected4_second);

  return results;
}

static auto test_reorder() -> supl::test_results
{
  supl::test_results results;

  const std::string reorder {"reorder"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple test_input {3, 3.14, true, reorder, vec};
  const std::tuple expected1 {3.14, reorder, 3, 3.14, vec, true};

  const auto result1 {supl::tuple::reorder<1, 3, 0, 1, 4, 2>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::
    tuple<int, const std::vector<int>&, const std::string&, char, bool>
      test_ref_input {42, vec, reorder, 'j', true};

  const std::tuple<const std::vector<int>&,
                   bool,
                   char,
                   const std::string&,
                   const std::vector<int>&,
                   int>
    expected2 {vec, true, 'j', reorder, vec, 42};

  const auto result2 {
    supl::tuple::reorder<1, 4, 3, 2, 1, 0>(test_ref_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {3.14, reorder, 3, 3.14, vec, true};

  const auto result3 {supl::tuple::reorder<1, 3, 0, 1, 4, 2>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_reverse() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, 'g', 3.14, true, 'D'};
  const std::tuple expected1 {'D', true, 3.14, 'g', 42};
  const auto result1 {supl::tuple::reverse(test_input)};
  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_subtuple() -> supl::test_results
{
  supl::test_results results;

  const std::string str {"subtuple"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple test_input {3, true, str, 3.14, vec};

  const std::tuple expected1 {str, 3.14, vec};
  const auto result1 {supl::tuple::subtuple<2, 5>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {test_input};
  const auto result2 {
    supl::tuple::subtuple<0, std::tuple_size_v<decltype(test_input)>>(
      test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {3, true, str};
  const auto result3 {supl::tuple::subtuple<0, 3>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  const std::tuple expected4 {3, true, str};
  const auto result4 {supl::tuple::subtuple<0, 3>(test_input)};

  results.enforce_exactly_equal(result4, expected4);

  const std::tuple expected5 {str, 3.14, vec};
  const auto result5 {supl::tuple::subtuple<2, 5>(test_input)};

  results.enforce_exactly_equal(result5, expected5);

  const std::tuple expected6 {str, 3.14, vec};
  const auto result6 {supl::tuple::subtuple<2, 5>(test_input)};

  results.enforce_exactly_equal(result6, expected6);

  int ref_test {42};
  const std::tuple<char, bool, int&, double, char> test_ref_input {
    'j', true, ref_test, 3.14, 'l'};
  const std::tuple<bool, int&, double> expected7 {true, ref_test, 3.14};
  const auto result7 {supl::tuple::subtuple<1, 4>(test_ref_input)};

  results.enforce_exactly_equal(result7, expected7);

  const std::tuple expected8 {3, true, str};
  const auto result8 {supl::tuple::subtuple<0, 3>(test_input)};

  results.enforce_exactly_equal(result8, expected8);

  return results;
}

static auto test_count_if() -> supl::test_results
{
  using supl::literals::size_t_literal::operator""_z;

  supl::test_results results;

  const std::tuple test {7, 2, 42.53, 3.14F, 9344285UL, -83LL};
  results.enforce_exactly_equal(
    supl::tuple::count_if(test,
                          [](const auto& i) -> std::size_t {
                            return i > 5;
                          }),
    3_z);

  return results;
}

static auto test_interleave() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1_1 {42, 'g', 3.14};
  const std::tuple test1_2 {2.6F, true, 8L};

  const std::tuple expected1 {42, 2.6F, 'g', true, 3.14, 8L};
  const auto result1 {supl::tuple::interleave(test1_1, test1_2)};

  results.enforce_exactly_equal(result1, expected1);

  using std::string_literals::operator""s;

  const std::tuple test2_1 {42, 'g', 3.14, 81, 5.31F, 4UL, false, '7'};
  const std::tuple test2_2 {
    2.6F, true, 8L, 8.3F, 2U, 'u', "yes"s, std::vector {3, 5, 7}
  };

  const std::tuple expected2 {
    42,
    2.6F,
    'g',
    true,
    3.14,
    8L,
    81,
    8.3F,
    5.31F,
    2U,
    4UL,
    'u',
    false,
    "yes"s,
    '7',
    std::vector {3, 5, 7}
  };
  const auto result2 {supl::tuple::interleave(test2_1, test2_2)};

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_tuple_alternating_split() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input_1 {1, 'J', true, 3.14, 'c', 42};
  const std::tuple expected1_first {1, true, 'c'};
  const std::tuple expected1_second {'J', 3.14, 42};

  const auto [result1_first, result1_second] {
    supl::tuple::alternating_split(test_input_1)};

  results.enforce_exactly_equal(result1_first, expected1_first);
  results.enforce_exactly_equal(result1_second, expected1_second);

  const std::tuple test_input_2 {1, true, 'c', 3.14, 42};
  const std::tuple expected2_first {1, 'c'};
  const std::tuple expected2_second {true, 3.14};
  const auto [result2_first, result2_second] {
    supl::tuple::alternating_split(test_input_2)};

  results.enforce_exactly_equal(result2_first, expected2_first);
  results.enforce_exactly_equal(result2_second, expected2_second);

  return results;
}

static auto test_front_n() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, '&', 3.14};

  const std::tuple<> expected0 {};
  const auto result0 {supl::tuple::front_n<0>(test_input)};

  results.enforce_exactly_equal(result0, expected0);

  const std::tuple expected1 {42};
  const auto result1 {supl::tuple::front_n<1>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {42, '&'};
  const auto result2 {supl::tuple::front_n<2>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {42, '&', 3.14};
  const auto result3 {supl::tuple::front_n<3>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_front() -> supl::test_results
{
  supl::test_results results;

  // NOLINTNEXTLINE(*const*)
  std::tuple not_const {42, 'g', 3.14};

  static_assert(
    std::is_same_v<int&, decltype(supl::tuple::front(not_const))>);
  results.enforce_equal(supl::tuple::front(not_const), 42);

  // NOLINTNEXTLINE(*const*)
  std::tuple<const int, char, double> const_element {42, 'g', 3.14};

  static_assert(
    std::is_same_v<const int&,
                   decltype(supl::tuple::front(const_element))>);
  results.enforce_equal(supl::tuple::front(const_element), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple const_tuple {42, 'g', 3.14};

  static_assert(
    std::is_same_v<const int&, decltype(supl::tuple::front(const_tuple))>);
  results.enforce_equal(supl::tuple::front(const_tuple), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple<const int, char, double> const_tuple_const_element {
    42, 'g', 3.14};

  static_assert(std::is_same_v<const int&,
                               decltype(supl::tuple::front(
                                 const_tuple_const_element))>);
  results.enforce_equal(supl::tuple::front(const_tuple_const_element), 42);

  return results;
}

static auto test_back_n() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, '&', 3.14};

  const std::tuple<> expected0 {};
  const auto result0 {supl::tuple::back_n<0>(test_input)};

  results.enforce_exactly_equal(result0, expected0);

  const std::tuple expected1 {3.14};
  const auto result1 {supl::tuple::back_n<1>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {'&', 3.14};
  const auto result2 {supl::tuple::back_n<2>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {42, '&', 3.14};
  const auto result3 {supl::tuple::back_n<3>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_back() -> supl::test_results
{
  supl::test_results results;

  // NOLINTNEXTLINE(*const*)
  std::tuple not_const {3.14, 'g', 42};

  static_assert(
    std::is_same_v<int&, decltype(supl::tuple::back(not_const))>);
  results.enforce_equal(supl::tuple::back(not_const), 42);

  // NOLINTNEXTLINE(*const*)
  std::tuple<double, char, const int> const_element {3.14, 'g', 42};

  static_assert(
    std::is_same_v<const int&,
                   decltype(supl::tuple::back(const_element))>);
  results.enforce_equal(supl::tuple::back(const_element), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple const_tuple {3.14, 'g', 42};

  static_assert(
    std::is_same_v<const int&, decltype(supl::tuple::back(const_tuple))>);
  results.enforce_equal(supl::tuple::back(const_tuple), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple<double, char, const int> const_tuple_const_element {
    3.14, 'g', 42};

  static_assert(std::is_same_v<const int&,
                               decltype(supl::tuple::back(
                                 const_tuple_const_element))>);
  results.enforce_equal(supl::tuple::back(const_tuple_const_element), 42);

  return results;
}

static auto test_elem_swap() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, 'g', 3.14, true, 18UL, 2.7F};

  const std::tuple expected1 {42, 18UL, 3.14, true, 'g', 2.7F};
  const auto result1 {supl::tuple::elem_swap<1, 4>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {42, 'g', 3.14, true, 18UL, 2.7F};
  const auto result2 {supl::tuple::elem_swap<3, 3>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {2.7F, 'g', 3.14, true, 18UL, 42};
  const auto result3 {supl::tuple::elem_swap<0, 5>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  const std::tuple expected4 {42, 18UL, 3.14, true, 'g', 2.7F};
  const auto result4 {supl::tuple::elem_swap<4, 1>(test_input)};

  results.enforce_exactly_equal(result4, expected4);

  return results;
}

static auto test_type_transform() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, 3.14, true};

  const std::tuple<const int&, const double&, const bool&> expected1 {
    std::get<0>(test_input),
    std::get<1>(test_input),
    std::get<2>(test_input)};

  const auto result1 {
    supl::tuple::type_transform<supl::make_const_ref>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  results.enforce_exactly_equal<const int*>(&std::get<0>(test_input),
                                            &std::get<0>(result1));

  results.enforce_exactly_equal<const double*>(&std::get<1>(test_input),
                                               &std::get<1>(result1));

  results.enforce_exactly_equal<const bool*>(&std::get<2>(test_input),
                                             &std::get<2>(result1));

  const auto& test_input_2 {expected1};
  const auto& expected2 {test_input};
  const auto result2 {
    supl::tuple::type_transform<std::decay>(test_input_2)};
  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_convert() -> supl::test_results
{
  struct just_a_bool {
    // NOLINTNEXTLINE(google-explicit-constructor)
    just_a_bool(bool arg)
        : value {arg}
    { }

    bool value;

    // NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
      out << value;
    }

    // NOLINTNEXTLINE(*unused*)
    [[nodiscard]] constexpr auto operator==(const just_a_bool& rhs) const
      -> bool
    {
      return this->value == rhs.value;
    }
  };

  using supl::literals::size_t_literal::operator""_z;
  supl::test_results results;

  constexpr static std::tuple test_input {42, 3.14, true};

  constexpr static std::tuple expected1 {42.0, 3_z, 1};
  constexpr static auto result1 {
    supl::tuple::convert<double, std::size_t, int>(test_input)};
  results.enforce_exactly_equal(result1, expected1);
  const std::tuple<char, double, just_a_bool> expected2 {
    '*', 3.14, just_a_bool {true}};
  const auto result2 {
    supl::tuple::convert<char, double, just_a_bool>(test_input)};
  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_resolve_refs() -> supl::test_results
{
  supl::test_results results;

  // NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
  class copy_counter
  {
  private:

    std::size_t m_count {0};

  public:

    copy_counter() = default;

    explicit copy_counter(std::size_t count)
        : m_count {count}
    { }

    copy_counter(const copy_counter& src) noexcept
        : m_count {src.m_count + 1}
    { }

    copy_counter(copy_counter&& src) = default;

    auto operator=(const copy_counter& rhs) noexcept -> copy_counter&
    {
      if ( &rhs != this ) {
        this->m_count = rhs.m_count + 1;
      }
      return *this;
    }

    /* ~copy_counter()                                 = default; */

    //NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
      out << "Copies: " << m_count;
    }

    //NOLINTNEXTLINE(*unused*)
    auto operator==(const copy_counter& rhs) const noexcept -> bool
    {
      return m_count == rhs.m_count;
    }
  };

  const std::tuple test_input {42, 3.14, '&', copy_counter {}};
  const std::tuple intermediate1_1 {
    supl::tuple::type_transform<supl::make_const_ref>(test_input)};
  const int i1 {42};
  const std::tuple intermediate1_2 {
    supl::tuple::push_back_as<const int&>(intermediate1_1, i1)};
  const double d1 {3.14};
  const std::tuple intermediate1_3 {
    supl::tuple::push_back_as<const double&>(intermediate1_2, d1)};
  const char c1 {'|'};
  const std::tuple intermediate1_4 {
    supl::tuple::push_back_as<const char&>(intermediate1_3, c1)};
  const bool b1 {false};
  const std::tuple intermediate1_5 {
    supl::tuple::push_back_as<const bool&>(intermediate1_4, b1)};
  const std::tuple result1 {supl::tuple::resolve_refs(intermediate1_5)};

  const std::
    tuple<int, double, char, copy_counter, int, double, char, bool>
      expected1 {42, 3.14, '&', copy_counter {1}, 42, 3.14, '|', false};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_make_cref_tuple() -> supl::test_results
{
  supl::test_results results;

  const int a {5};
  const char b {'b'};
  const bool c {true};
  const std::tuple<const int&, const char&, const bool&> expected1 {
    a, b, c};
  const auto result1 {supl::tuple::make_cref_tuple(a, b, c)};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

auto test_tuple_algo() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::tuple::for_each", &test_for_each);
  section.add_test("supl::tuple::for_each_in_subtuple",
                   &test_for_each_in_subtuple);
  section.add_test("supl::tuple::transform", &test_transform);
  section.add_test("supl::tuple::any_of", &test_any_of);
  section.add_test("supl::tuple::all_of", &test_all_of);
  section.add_test("supl::tuple::none_of", &test_none_of);
  section.add_test("supl::tuple::push_back", &test_push_back);
  section.add_test("supl::tuple::push_back_as", &test_push_back_as);
  section.add_test("supl::tuple::pop_front", &test_pop_front);
  section.add_test("supl::tuple::push_front", &test_push_front);
  section.add_test("supl::tuple::push_front_as", &test_push_front_as);
  section.add_test("supl::tuple::pop_back", &test_pop_back);
  section.add_test("supl::tuple::rotate_left", &test_rotate_left);
  section.add_test("supl::tuple::rotate_right", &test_rotate_right);
  section.add_test("supl::tuple::insert", &test_insert);
  section.add_test("supl::tuple::erase", &test_erase);
  section.add_test("supl::tuple::replace", &test_replace);
  section.add_test("supl::tuple::reorder", &test_reorder);
  section.add_test("supl::tuple::reverse", &test_reverse);
  section.add_test("supl::tuple::split", &test_split);
  section.add_test("supl::tuple::subtuple", &test_subtuple);
  section.add_test("supl::tuple::count_if", &test_count_if);
  section.add_test("supl::tuple::interleave", &test_interleave);
  section.add_test("supl::tuple::alternating_split",
                   &test_tuple_alternating_split);
  section.add_test("supl::tuple::front_n", &test_front_n);
  section.add_test("supl::tuple::front", &test_front);
  section.add_test("supl::tuple::back_n", &test_back_n);
  section.add_test("supl::tuple::back", &test_back);
  section.add_test("supl::tuple::elem_swap", &test_elem_swap);
  section.add_test("supl::tuple::type_transform", &test_type_transform);
  section.add_test("supl::tuple::convert", &test_convert);
  section.add_test("supl::tuple::resolve_refs", &test_resolve_refs);
  section.add_test("supl::tuple::make_cref_tuple", &test_make_cref_tuple);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_tuple_algo());

  return runner.run();
}
