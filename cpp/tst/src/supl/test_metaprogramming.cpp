#include <array>
#include <forward_list>
#include <list>
#include <queue>
#include <type_traits>
#include <vector>

#include "test_utils.hpp"
#include "supl/test_metaprogramming.h"

static auto test_sum_type() -> supl::test
{
  supl::test results;

  results.add_case(std::is_same_v<supl::sum_type_t<int, int>, int>, true,
                   "int + int == int");
  results.add_case(
      std::is_same_v<supl::sum_type_t<int16_t, int16_t>, int>, true,
      "int16_t + int16_t == int");
  results.add_case(
      std::is_same_v<supl::sum_type_t<double, double>, double>, true,
      "double + double == double");
  results.add_case(std::is_same_v<supl::sum_type_t<double, int>, double>,
                   true, "double + int == double");
  results.add_case(
      std::is_same_v<supl::sum_type_t<double, float>, double>, true,
      "double + float == double");
  results.add_case(
      std::is_same_v<supl::sum_type_t<int, int16_t>, int16_t>, false,
      "int + int16_t == int");
  results.add_case(std::is_same_v<supl::sum_type_t<double, int>, int>,
                   false, "double + int == double");
  results.add_case(std::is_same_v<supl::sum_type_t<char, int>, char>,
                   false, "char + int == int");
  results.add_case(
      std::is_same_v<
          supl::sum_type_t<int16_t, int16_t, int16_t, int16_t, int>, int>,
      true, "int16_t + int16_t + int16_t + int16_t + int == int");

  return results;
}

static_assert(std::is_same_v<supl::sum_type_t<int, int>, int>,
              "int + int == int");
static_assert(std::is_same_v<supl::sum_type_t<int16_t, int16_t>, int>,
              "int16_t + int16_t == int");
static_assert(std::is_same_v<supl::sum_type_t<double, double>, double>,
              "double + double == double");
static_assert(std::is_same_v<supl::sum_type_t<double, int>, double>,
              "double + int == double");
static_assert(std::is_same_v<supl::sum_type_t<double, float>, double>,
              "double + float == double");
static_assert(std::is_same_v<supl::sum_type_t<int, int16_t>, int>,
              "int + int16_t == int");
static_assert(std::is_same_v<supl::sum_type_t<double, int>, double>,
              "double + int == double");
static_assert(std::is_same_v<supl::sum_type_t<char, int>, int>,
              "char + int == int");
static_assert(
    std::is_same_v<
        supl::sum_type_t<int16_t, int16_t, int16_t, int16_t, int>, int>,
    "int16_t + int16_t + int16_t + int16_t + int == int");

static auto test_type_identity() -> supl::test
{
  supl::test results;

  results.add_case(std::is_same_v<supl::type_identity_t<char>, int>,
                   false, "char : int");
  results.add_case(std::is_same_v<supl::type_identity_t<int>, char>,
                   false, "int : char");
  results.add_case(std::is_same_v<supl::type_identity_t<int>, int>, true,
                   "int : int");

  results.add_case(
      std::is_same_v<supl::type_identity_t<const int>, const int>, true,
      "const int : const int");
  results.add_case(std::is_same_v<supl::type_identity_t<const int>, int>,
                   false, "const int: int");
  results.add_case(std::is_same_v<supl::type_identity_t<int>, const int>,
                   false, "int : const int");

  results.add_case(
      std::is_same_v<supl::type_identity_t<volatile int>, volatile int>,
      true, "volatile int : volatile int");
  results.add_case(
      std::is_same_v<supl::type_identity_t<volatile int>, int>, false,
      "volatile int: int");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int>, volatile int>, false,
      "int : volatile int");

  results.add_case(
      std::is_same_v<supl::type_identity_t<const volatile int>,
                     const volatile int>,
      true, "const volatile int : const volatile int");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const volatile int>, int>,
      false, "const volatile int: int");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int>, const volatile int>,
      false, "int : const volatile int");

  results.add_case(std::is_same_v<supl::type_identity_t<int&>, int>,
                   false, "int& : int");
  results.add_case(std::is_same_v<supl::type_identity_t<int>, int&>,
                   false, "int : int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const int&>, const int&>, true,
      "const int& : const int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const int&>, int&>, false,
      "const int&: int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int&>, const int&>, false,
      "int& : const int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<volatile int&>, volatile int&>,
      true, "volatile int& : volatile int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<volatile int&>, int&>, false,
      "volatile int&: int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int&>, volatile int&>, false,
      "int& : volatile int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const volatile int&>,
                     const volatile int&>,
      true, "const volatile int& : const volatile int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const volatile int&>, int&>,
      false, "const volatile int&: int&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int&>, const volatile int&>,
      false, "int& : const volatile int&");

  results.add_case(std::is_same_v<supl::type_identity_t<int&&>, int>,
                   false, "int&& : int");
  results.add_case(std::is_same_v<supl::type_identity_t<int>, int&&>,
                   false, "int : int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const int&&>, const int&&>,
      true, "const int&& : const int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const int&&>, int&&>, false,
      "const int&&: int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int&&>, const int&&>, false,
      "int&& : const int&&");
  results.add_case(std::is_same_v<supl::type_identity_t<volatile int&&>,
                                  volatile int&&>,
                   true, "volatile int&& : volatile int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<volatile int&&>, int&&>, false,
      "volatile int&&: int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int&&>, volatile int&&>, false,
      "int&& : volatile int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const volatile int&&>,
                     const volatile int&&>,
      true, "const volatile int&& : const volatile int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<const volatile int&&>, int&&>,
      false, "const volatile int&&: int&&");
  results.add_case(
      std::is_same_v<supl::type_identity_t<int&&>, const volatile int&&>,
      false, "int&& : const volatile int&&");

  return results;
}

static auto test_is_type_in_pack() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_type_in_pack_v<int, char, bool, int>, true);
  results.add_case(supl::is_type_in_pack_v<int, int, bool, int>, true);
  results.add_case(supl::is_type_in_pack_v<int, char, bool, char, char,
                                            char, float, double>,
                   false);
  results.add_case(supl::is_type_in_pack_v<int, int, int, int>, true);
  results.add_case(supl::is_type_in_pack_v<int, int>, true);
  results.add_case(supl::is_type_in_pack_v<int, char>, false);

  return results;
}

static_assert(supl::is_type_in_pack_v<int, char, bool, int>);
static_assert(supl::is_type_in_pack_v<int, int, bool, int>);
static_assert(not supl::is_type_in_pack_v<int, char, bool, char, char,
                                           char, float, double>);
static_assert(supl::is_type_in_pack_v<int, int, int, int>);
static_assert(supl::is_type_in_pack_v<int, int>);
static_assert(not supl::is_type_in_pack_v<int, char>);

static auto test_peel_first() -> supl::test
{
  supl::test results;

  results.add_case(
      std::is_same_v<supl::peel_first_t<int, char, char, char>, int>,
      true);
  results.add_case(
      std::is_same_v<supl::peel_first_t<int, int, int, int>, int>, true);
  results.add_case(
      std::is_same_v<supl::peel_first_t<int, char, int, int>, int>, true);
  results.add_case(
      std::is_same_v<supl::peel_first_t<char, int, int, int>, char>,
      true);

  return results;
}

static_assert(
    std::is_same_v<supl::peel_first_t<int, char, char, char>, int>);
static_assert(
    std::is_same_v<supl::peel_first_t<int, int, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_first_t<int, char, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_first_t<char, int, int, int>, char>);

static auto test_peel_last() -> supl::test
{
  supl::test results;

  results.add_case(
      std::is_same_v<supl::peel_last_t<char, char, char, int>, int>,
      true);
  results.add_case(
      std::is_same_v<supl::peel_last_t<int, int, int, int>, int>, true);
  results.add_case(
      std::is_same_v<supl::peel_last_t<int, char, int, int>, int>, true);
  results.add_case(
      std::is_same_v<supl::peel_last_t<int, int, int, char>, char>, true);

  return results;
}

static_assert(
    std::is_same_v<supl::peel_last_t<char, char, char, int>, int>);
static_assert(std::is_same_v<supl::peel_last_t<int, int, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_last_t<int, char, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_last_t<int, int, int, char>, char>);

static auto test_is_pack_uniform() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_pack_uniform_v<int, int, int, int, int>,
                   true);
  results.add_case(supl::is_pack_uniform_v<int, char, int, int, int>,
                   false);
  results.add_case(supl::is_pack_uniform_v<int, int, int, int, char>,
                   false);
  results.add_case(supl::is_pack_uniform_v<int, int, int, char, int, int>,
                   false);
  results.add_case(supl::is_pack_uniform_v<int, bool, bool, bool, bool>,
                   false);
  results.add_case(supl::is_pack_uniform_v<bool, bool, int, bool, bool>,
                   false);
  results.add_case(supl::is_pack_uniform_v<bool, bool, bool, bool, int>,
                   false);
  results.add_case(supl::is_pack_uniform_v<int, int>, true);
  results.add_case(supl::is_pack_uniform_v<int, char>, false);
  results.add_case(supl::is_pack_uniform_v<int>, true);

  return results;
}

static_assert(supl::is_pack_uniform_v<int, int, int, int, int>);
static_assert(not supl::is_pack_uniform_v<int, char, int, int, int>);
static_assert(not supl::is_pack_uniform_v<int, int, int, int, char>);
static_assert(not supl::is_pack_uniform_v<int, int, int, char, int, int>);
static_assert(not supl::is_pack_uniform_v<int, bool, bool, bool, bool>);
static_assert(not supl::is_pack_uniform_v<bool, bool, int, bool, bool>);
static_assert(not supl::is_pack_uniform_v<bool, bool, bool, bool, int>);
static_assert(supl::is_pack_uniform_v<int, int>);
static_assert(not supl::is_pack_uniform_v<int, char>);
static_assert(supl::is_pack_uniform_v<int>);

static auto test_is_pack_only() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_pack_only_v<double, double, double, double>,
                   true);
  results.add_case(supl::is_pack_only_v<double, double, double, int>,
                   false);
  results.add_case(supl::is_pack_only_v<int, int, int, int>, true);
  results.add_case(supl::is_pack_only_v<int, double, double, double>,
                   false);
  results.add_case(
      supl::is_pack_only_v<std::string, std::string_view,
                            std::string_view, std::string_view>,
      false);
  results.add_case(supl::is_pack_only_v<double>, false);
  results.add_case(supl::is_pack_only_v<void>, false);

  return results;
}

static_assert(supl::is_pack_only_v<double, double, double, double>);
static_assert(not supl::is_pack_only_v<double, double, double, int>);
static_assert(supl::is_pack_only_v<int, int, int, int>);
static_assert(not supl::is_pack_only_v<int, double, double, double>);
static_assert(
    not supl::is_pack_only_v<std::string, std::string_view,
                              std::string_view, std::string_view>);
static_assert(not supl::is_pack_only_v<double>);
static_assert(not supl::is_pack_only_v<void>);

static auto test_is_iterable() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_iterable_v<std::vector<int>>, true);
  results.add_case(supl::is_iterable_v<std::list<int>>, true);
  results.add_case(supl::is_iterable_v<std::array<int, 5>>, true);
  // NOLINTNEXTLINE(*-avoid-c-arrays)
  results.add_case(supl::is_iterable_v<int[5]>, true);
  results.add_case(supl::is_iterable_v<int>, false);
  results.add_case(supl::is_iterable_v<char>, false);
  results.add_case(supl::is_iterable_v<bool>, false);
  results.add_case(supl::is_iterable_v<std::tuple<int, char, bool>>,
                   false);
  results.add_case(supl::is_iterable_v<std::pair<int, char>>, false);

  return results;
}

static_assert(supl::is_iterable_v<std::vector<int>>);
static_assert(supl::is_iterable_v<std::list<int>>);
static_assert(supl::is_iterable_v<std::array<int, 5>>);
// NOLINTNEXTLINE(*-avoid-c-arrays)
static_assert(supl::is_iterable_v<int[5]>);
static_assert(not supl::is_iterable_v<int>);
static_assert(not supl::is_iterable_v<char>);
static_assert(not supl::is_iterable_v<bool>);
static_assert(not supl::is_iterable_v<std::tuple<int, char, bool>>);
static_assert(not supl::is_iterable_v<std::pair<int, char>>);

static auto test_is_iterator() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_iterator_v<int>, false);
  results.add_case(supl::is_iterator_v<std::vector<int>::iterator>, true);
  results.add_case(supl::is_iterator_v<std::array<int, 5>::iterator>,
                   true);
  results.add_case(supl::is_iterator_v<int*>, true);
  results.add_case(supl::is_iterator_v<const int*>, true);

  return results;
}

static_assert(not supl::is_iterator_v<int>);
static_assert(supl::is_iterator_v<std::vector<int>::iterator>);
static_assert(supl::is_iterator_v<std::array<int, 5>::iterator>);
static_assert(supl::is_iterator_v<int*>);
static_assert(supl::is_iterator_v<const int*>);

static auto test_is_bidirectional() -> supl::test
{
  supl::test results;

  results.add_case(
      supl::is_bidirectional_v<std::forward_list<int>::iterator>, false);
  results.add_case(supl::is_bidirectional_v<std::vector<int>::iterator>,
                   true);
  results.add_case(supl::is_bidirectional_v<std::list<int>::iterator>,
                   true);
  results.add_case(supl::is_bidirectional_v<std::deque<int>::iterator>,
                   true);

  return results;
}

static_assert(
    not supl::is_bidirectional_v<std::forward_list<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::vector<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::list<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::deque<int>::iterator>);

static auto test_is_random_access() -> supl::test
{
  supl::test results;

  results.add_case(
      supl::is_random_access_v<std::forward_list<int>::iterator>, false);
  results.add_case(supl::is_random_access_v<std::vector<int>::iterator>,
                   true);
  results.add_case(supl::is_random_access_v<std::list<int>::iterator>,
                   false);
  results.add_case(supl::is_random_access_v<std::deque<int>::iterator>,
                   true);

  return results;
}

static_assert(
    not supl::is_random_access_v<std::forward_list<int>::iterator>);
static_assert(supl::is_random_access_v<std::vector<int>::iterator>);
static_assert(not supl::is_random_access_v<std::list<int>::iterator>);
static_assert(supl::is_random_access_v<std::deque<int>::iterator>);

static auto test_is_tuple() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_tuple_v<std::tuple<int, char, double>>, true);
  results.add_case(supl::is_tuple_v<const std::tuple<int, char, double>>,
                   true);
  results.add_case(
      supl::is_tuple_v<volatile std::tuple<int, char, double>>, true);
  results.add_case(
      supl::is_tuple_v<const volatile std::tuple<int, char, double>>,
      true);
  results.add_case(supl::is_tuple_v<std::tuple<int, char, double>&>,
                   true);
  results.add_case(supl::is_tuple_v<const std::tuple<int, char, double>&>,
                   true);
  results.add_case(
      supl::is_tuple_v<volatile std::tuple<int, char, double>&>, true);
  results.add_case(
      supl::is_tuple_v<const volatile std::tuple<int, char, double>&>,
      true);
  results.add_case(supl::is_tuple_v<std::tuple<int, char, double>&&>,
                   true);
  results.add_case(
      supl::is_tuple_v<const std::tuple<int, char, double>&&>, true);
  results.add_case(
      supl::is_tuple_v<volatile std::tuple<int, char, double>&&>, true);
  results.add_case(
      supl::is_tuple_v<const volatile std::tuple<int, char, double>&&>,
      true);

  results.add_case(supl::is_tuple_v<std::tuple<int, char>>, true);
  results.add_case(supl::is_tuple_v<const std::tuple<int, char>>, true);
  results.add_case(supl::is_tuple_v<volatile std::tuple<int, char>>,
                   true);
  results.add_case(supl::is_tuple_v<const volatile std::tuple<int, char>>,
                   true);
  results.add_case(supl::is_tuple_v<std::tuple<int, char>&>, true);
  results.add_case(supl::is_tuple_v<const std::tuple<int, char>&>, true);
  results.add_case(supl::is_tuple_v<volatile std::tuple<int, char>&>,
                   true);
  results.add_case(
      supl::is_tuple_v<const volatile std::tuple<int, char>&>, true);
  results.add_case(supl::is_tuple_v<std::tuple<int, char>&&>, true);
  results.add_case(supl::is_tuple_v<const std::tuple<int, char>&&>, true);
  results.add_case(supl::is_tuple_v<volatile std::tuple<int, char>&&>,
                   true);
  results.add_case(
      supl::is_tuple_v<const volatile std::tuple<int, char>&&>, true);

  results.add_case(supl::is_tuple_v<std::pair<int, char>>, false);
  results.add_case(supl::is_tuple_v<const std::pair<int, char>>, false);
  results.add_case(supl::is_tuple_v<volatile std::pair<int, char>>,
                   false);
  results.add_case(supl::is_tuple_v<const volatile std::pair<int, char>>,
                   false);
  results.add_case(supl::is_tuple_v<std::pair<int, char>&>, false);
  results.add_case(supl::is_tuple_v<const std::pair<int, char>&>, false);
  results.add_case(supl::is_tuple_v<volatile std::pair<int, char>&>,
                   false);
  results.add_case(supl::is_tuple_v<const volatile std::pair<int, char>&>,
                   false);
  results.add_case(supl::is_tuple_v<std::pair<int, char>&&>, false);
  results.add_case(supl::is_tuple_v<const std::pair<int, char>&&>, false);
  results.add_case(supl::is_tuple_v<volatile std::pair<int, char>&&>,
                   false);
  results.add_case(
      supl::is_tuple_v<const volatile std::pair<int, char>&&>, false);

  results.add_case(supl::is_tuple_v<int>, false);

  return results;
}

static_assert(supl::is_tuple_v<std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<const std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char, double>>);
static_assert(
    supl::is_tuple_v<const volatile std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char, double>&>);
static_assert(
    supl::is_tuple_v<const volatile std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char, double>&&>);
static_assert(
    supl::is_tuple_v<const volatile std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<std::tuple<int, char>>);
static_assert(supl::is_tuple_v<const std::tuple<int, char>>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char>>);
static_assert(supl::is_tuple_v<const volatile std::tuple<int, char>>);
static_assert(supl::is_tuple_v<std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<const volatile std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<std::tuple<int, char>&&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char>&&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char>&&>);
static_assert(supl::is_tuple_v<const volatile std::tuple<int, char>&&>);
static_assert(not supl::is_tuple_v<std::pair<int, char>>);
static_assert(not supl::is_tuple_v<const std::pair<int, char>>);
static_assert(not supl::is_tuple_v<volatile std::pair<int, char>>);
static_assert(not supl::is_tuple_v<const volatile std::pair<int, char>>);
static_assert(not supl::is_tuple_v<std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<const std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<volatile std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<const volatile std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<const std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<volatile std::pair<int, char>&&>);
static_assert(
    not supl::is_tuple_v<const volatile std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<int>);

static auto test_is_pair() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_pair_v<std::pair<int, char>>, true);
  results.add_case(supl::is_pair_v<const std::pair<int, char>>, true);
  results.add_case(supl::is_pair_v<volatile std::pair<int, char>>, true);
  results.add_case(supl::is_pair_v<const volatile std::pair<int, char>>,
                   true);
  results.add_case(supl::is_pair_v<std::pair<int, char>&>, true);
  results.add_case(supl::is_pair_v<const std::pair<int, char>&>, true);
  results.add_case(supl::is_pair_v<volatile std::pair<int, char>&>, true);
  results.add_case(supl::is_pair_v<const volatile std::pair<int, char>&>,
                   true);
  results.add_case(supl::is_pair_v<std::pair<int, char>&&>, true);
  results.add_case(supl::is_pair_v<const std::pair<int, char>&&>, true);
  results.add_case(supl::is_pair_v<volatile std::pair<int, char>&&>,
                   true);
  results.add_case(supl::is_pair_v<const volatile std::pair<int, char>&&>,
                   true);

  results.add_case(supl::is_pair_v<std::tuple<int, char>>, false);
  results.add_case(supl::is_pair_v<const std::tuple<int, char>>, false);
  results.add_case(supl::is_pair_v<volatile std::tuple<int, char>>,
                   false);
  results.add_case(supl::is_pair_v<const volatile std::tuple<int, char>>,
                   false);
  results.add_case(supl::is_pair_v<std::tuple<int, char>&>, false);
  results.add_case(supl::is_pair_v<const std::tuple<int, char>&>, false);
  results.add_case(supl::is_pair_v<volatile std::tuple<int, char>&>,
                   false);
  results.add_case(supl::is_pair_v<const volatile std::tuple<int, char>&>,
                   false);
  results.add_case(supl::is_pair_v<std::tuple<int, char>&&>, false);
  results.add_case(supl::is_pair_v<const std::tuple<int, char>&&>, false);
  results.add_case(supl::is_pair_v<volatile std::tuple<int, char>&&>,
                   false);
  results.add_case(
      supl::is_pair_v<const volatile std::tuple<int, char>&&>, false);

  results.add_case(supl::is_pair_v<int>, false);

  return results;
}

static_assert(supl::is_pair_v<std::pair<int, char>>);
static_assert(supl::is_pair_v<const std::pair<int, char>>);
static_assert(supl::is_pair_v<volatile std::pair<int, char>>);
static_assert(supl::is_pair_v<const volatile std::pair<int, char>>);
static_assert(supl::is_pair_v<std::pair<int, char>&>);
static_assert(supl::is_pair_v<const std::pair<int, char>&>);
static_assert(supl::is_pair_v<volatile std::pair<int, char>&>);
static_assert(supl::is_pair_v<const volatile std::pair<int, char>&>);
static_assert(supl::is_pair_v<std::pair<int, char>&&>);
static_assert(supl::is_pair_v<const std::pair<int, char>&&>);
static_assert(supl::is_pair_v<volatile std::pair<int, char>&&>);
static_assert(supl::is_pair_v<const volatile std::pair<int, char>&&>);
static_assert(not supl::is_pair_v<std::tuple<int, char>>);
static_assert(not supl::is_pair_v<const std::tuple<int, char>>);
static_assert(not supl::is_pair_v<volatile std::tuple<int, char>>);
static_assert(not supl::is_pair_v<const volatile std::tuple<int, char>>);
static_assert(not supl::is_pair_v<std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<const std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<volatile std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<const volatile std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<const std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<volatile std::tuple<int, char>&&>);
static_assert(
    not supl::is_pair_v<const volatile std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<int>);

static auto test_is_printable() -> supl::test
{
  supl::test results;

  results.add_case(supl::is_printable_v<int>, true);
  results.add_case(supl::is_printable_v<char>, true);
  results.add_case(supl::is_printable_v<bool>, true);
  results.add_case(supl::is_printable_v<std::string>, true);
  results.add_case(supl::is_printable_v<std::vector<int>>, false);
  results.add_case(supl::is_printable_v<std::list<std::string>>, false);

  return results;
}

static_assert(supl::is_printable_v<int>);
static_assert(supl::is_printable_v<char>);
static_assert(supl::is_printable_v<bool>);
static_assert(supl::is_printable_v<std::string>);
static_assert(not supl::is_printable_v<std::vector<int>>);
static_assert(not supl::is_printable_v<std::list<std::string>>);

void test_metaprogramming()
{
  supl::run_test("supl::sum_type", &test_sum_type);
  supl::run_test("supl::type_identity", &test_type_identity);
  supl::run_test("supl::is_type_in_pack", &test_is_type_in_pack);
  supl::run_test("supl::peel_first", &test_peel_first);
  supl::run_test("supl::peel_last", &test_peel_last);
  supl::run_test("supl::is_pack_uniform", &test_is_pack_uniform);
  supl::run_test("supl::is_pack_only", &test_is_pack_only);
  supl::run_test("supl::is_iterable", &test_is_iterable);
  supl::run_test("supl::is_iterator", &test_is_iterator);
  supl::run_test("supl::is_bidirectional", &test_is_bidirectional);
  supl::run_test("supl::is_random_access", &test_is_random_access);
  supl::run_test("supl::is_tuple", &test_is_tuple);
  supl::run_test("supl::is_pair", &test_is_pair);
  supl::run_test("supl::is_printable", &test_is_printable);
}
