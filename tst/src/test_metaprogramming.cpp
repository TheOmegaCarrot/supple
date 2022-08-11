#include <array>
#include <list>
#include <type_traits>
#include <vector>

#include "test_metaprogramming.h"
#include "test_utils.hpp"

#include "utils/lazy.hpp"

auto test_sum_type() -> ehanc::test
{
  ehanc::test results;

  results.add_case(std::is_same_v<ehanc::sum_type_t<int, int>, int>, true,
                   "int + int == int");
  results.add_case(std::is_same_v<ehanc::sum_type_t<short, short>, int>,
                   true, "short + short == int");
  results.add_case(
      std::is_same_v<ehanc::sum_type_t<double, double>, double>, true,
      "double + double == double");
  results.add_case(std::is_same_v<ehanc::sum_type_t<double, int>, double>,
                   true, "double + int == double");
  results.add_case(
      std::is_same_v<ehanc::sum_type_t<double, float>, double>, true,
      "double + float == double");
  results.add_case(
      std::is_same_v<ehanc::sum_type_t<ehanc::lazy<int>, int>, int>, true,
      "ehanc::lazy<int> + int == int");
  results.add_case(std::is_same_v<ehanc::sum_type_t<int, short>, short>,
                   false, "int + short == int");
  results.add_case(std::is_same_v<ehanc::sum_type_t<double, int>, int>,
                   false, "double + int == double");
  results.add_case(std::is_same_v<ehanc::sum_type_t<char, int>, char>,
                   false, "char + int == int");
  results.add_case(
      std::is_same_v<ehanc::sum_type_t<short, short, short, short, int>,
                     int>,
      true, "short + short + short + short + int == int");

  return results;
}

static_assert(std::is_same_v<ehanc::sum_type_t<int, int>, int>,
              "int + int == int");
static_assert(std::is_same_v<ehanc::sum_type_t<short, short>, int>,
              "short + short == int");
static_assert(std::is_same_v<ehanc::sum_type_t<double, double>, double>,
              "double + double == double");
static_assert(std::is_same_v<ehanc::sum_type_t<double, int>, double>,
              "double + int == double");
static_assert(std::is_same_v<ehanc::sum_type_t<double, float>, double>,
              "double + float == double");
static_assert(
    std::is_same_v<ehanc::sum_type_t<ehanc::lazy<int>, int>, int>,
    "ehanc::lazy<int> + int == int");
static_assert(std::is_same_v<ehanc::sum_type_t<int, short>, int>,
              "int + short == int");
static_assert(std::is_same_v<ehanc::sum_type_t<double, int>, double>,
              "double + int == double");
static_assert(std::is_same_v<ehanc::sum_type_t<char, int>, int>,
              "char + int == int");
static_assert(std::is_same_v<
                  ehanc::sum_type_t<short, short, short, short, int>, int>,
              "short + short + short + short + int == int");

auto test_type_identity() -> ehanc::test
{
  ehanc::test results;

  results.add_case(std::is_same_v<ehanc::type_identity_t<char>, int>,
                   false, "char : int");
  results.add_case(std::is_same_v<ehanc::type_identity_t<int>, char>,
                   false, "int : char");
  results.add_case(std::is_same_v<ehanc::type_identity_t<int>, int>, true,
                   "int : int");

  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const int>, const int>, true,
      "const int : const int");
  results.add_case(std::is_same_v<ehanc::type_identity_t<const int>, int>,
                   false, "const int: int");
  results.add_case(std::is_same_v<ehanc::type_identity_t<int>, const int>,
                   false, "int : const int");

  results.add_case(
      std::is_same_v<ehanc::type_identity_t<volatile int>, volatile int>,
      true, "volatile int : volatile int");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<volatile int>, int>, false,
      "volatile int: int");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int>, volatile int>, false,
      "int : volatile int");

  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const volatile int>,
                     const volatile int>,
      true, "const volatile int : const volatile int");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const volatile int>, int>,
      false, "const volatile int: int");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int>, const volatile int>,
      false, "int : const volatile int");

  results.add_case(std::is_same_v<ehanc::type_identity_t<int&>, int>,
                   false, "int& : int");
  results.add_case(std::is_same_v<ehanc::type_identity_t<int>, int&>,
                   false, "int : int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const int&>, const int&>, true,
      "const int& : const int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const int&>, int&>, false,
      "const int&: int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int&>, const int&>, false,
      "int& : const int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<volatile int&>, volatile int&>,
      true, "volatile int& : volatile int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<volatile int&>, int&>, false,
      "volatile int&: int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int&>, volatile int&>, false,
      "int& : volatile int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const volatile int&>,
                     const volatile int&>,
      true, "const volatile int& : const volatile int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const volatile int&>, int&>,
      false, "const volatile int&: int&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int&>, const volatile int&>,
      false, "int& : const volatile int&");

  results.add_case(std::is_same_v<ehanc::type_identity_t<int&&>, int>,
                   false, "int&& : int");
  results.add_case(std::is_same_v<ehanc::type_identity_t<int>, int&&>,
                   false, "int : int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const int&&>, const int&&>,
      true, "const int&& : const int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const int&&>, int&&>, false,
      "const int&&: int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int&&>, const int&&>, false,
      "int&& : const int&&");
  results.add_case(std::is_same_v<ehanc::type_identity_t<volatile int&&>,
                                  volatile int&&>,
                   true, "volatile int&& : volatile int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<volatile int&&>, int&&>, false,
      "volatile int&&: int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int&&>, volatile int&&>, false,
      "int&& : volatile int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const volatile int&&>,
                     const volatile int&&>,
      true, "const volatile int&& : const volatile int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<const volatile int&&>, int&&>,
      false, "const volatile int&&: int&&");
  results.add_case(
      std::is_same_v<ehanc::type_identity_t<int&&>, const volatile int&&>,
      false, "int&& : const volatile int&&");

  return results;
}

auto test_is_type_in_pack() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_type_in_pack_v<int, char, bool, int>, true);
  results.add_case(ehanc::is_type_in_pack_v<int, int, bool, int>, true);
  results.add_case(
      ehanc::is_type_in_pack_v<int, char, bool, ehanc::lazy<int>>, false);
  results.add_case(ehanc::is_type_in_pack_v<int, char, bool, char, char,
                                            char, float, double>,
                   false);
  results.add_case(ehanc::is_type_in_pack_v<int, int, int, int>, true);
  results.add_case(ehanc::is_type_in_pack_v<int, int>, true);
  results.add_case(ehanc::is_type_in_pack_v<int, char>, false);

  return results;
}

static_assert(ehanc::is_type_in_pack_v<int, char, bool, int>);
static_assert(ehanc::is_type_in_pack_v<int, int, bool, int>);
static_assert(
    not ehanc::is_type_in_pack_v<int, char, bool, ehanc::lazy<int>>);
static_assert(not ehanc::is_type_in_pack_v<int, char, bool, char, char,
                                           char, float, double>);
static_assert(ehanc::is_type_in_pack_v<int, int, int, int>);
static_assert(ehanc::is_type_in_pack_v<int, int>);
static_assert(not ehanc::is_type_in_pack_v<int, char>);

auto test_peel_first() -> ehanc::test
{
  ehanc::test results;

  results.add_case(
      std::is_same_v<ehanc::peel_first_t<int, char, char, char>, int>,
      true);
  results.add_case(
      std::is_same_v<ehanc::peel_first_t<int, int, int, int>, int>, true);
  results.add_case(
      std::is_same_v<ehanc::peel_first_t<int, char, int, int>, int>, true);
  results.add_case(
      std::is_same_v<ehanc::peel_first_t<char, int, int, int>, char>,
      true);

  return results;
}

static_assert(
    std::is_same_v<ehanc::peel_first_t<int, char, char, char>, int>);
static_assert(
    std::is_same_v<ehanc::peel_first_t<int, int, int, int>, int>);
static_assert(
    std::is_same_v<ehanc::peel_first_t<int, char, int, int>, int>);
static_assert(
    std::is_same_v<ehanc::peel_first_t<char, int, int, int>, char>);

auto test_peel_last() -> ehanc::test
{
  ehanc::test results;

  results.add_case(
      std::is_same_v<ehanc::peel_last_t<char, char, char, int>, int>,
      true);
  results.add_case(
      std::is_same_v<ehanc::peel_last_t<int, int, int, int>, int>, true);
  results.add_case(
      std::is_same_v<ehanc::peel_last_t<int, char, int, int>, int>, true);
  results.add_case(
      std::is_same_v<ehanc::peel_last_t<int, int, int, char>, char>, true);

  return results;
}

static_assert(
    std::is_same_v<ehanc::peel_last_t<char, char, char, int>, int>);
static_assert(std::is_same_v<ehanc::peel_last_t<int, int, int, int>, int>);
static_assert(
    std::is_same_v<ehanc::peel_last_t<int, char, int, int>, int>);
static_assert(
    std::is_same_v<ehanc::peel_last_t<int, int, int, char>, char>);

auto test_is_pack_uniform() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_pack_uniform_v<int, int, int, int, int>,
                   true);
  results.add_case(ehanc::is_pack_uniform_v<int, char, int, int, int>,
                   false);
  results.add_case(ehanc::is_pack_uniform_v<int, int, int, int, char>,
                   false);
  results.add_case(ehanc::is_pack_uniform_v<int, int, int, char, int, int>,
                   false);
  results.add_case(ehanc::is_pack_uniform_v<int, bool, bool, bool, bool>,
                   false);
  results.add_case(ehanc::is_pack_uniform_v<bool, bool, int, bool, bool>,
                   false);
  results.add_case(ehanc::is_pack_uniform_v<bool, bool, bool, bool, int>,
                   false);
  results.add_case(ehanc::is_pack_uniform_v<int, int>, true);
  results.add_case(ehanc::is_pack_uniform_v<int, char>, false);
  results.add_case(ehanc::is_pack_uniform_v<int>, true);

  return results;
}

static_assert(ehanc::is_pack_uniform_v<int, int, int, int, int>);
static_assert(not ehanc::is_pack_uniform_v<int, char, int, int, int>);
static_assert(not ehanc::is_pack_uniform_v<int, int, int, int, char>);
static_assert(not ehanc::is_pack_uniform_v<int, int, int, char, int, int>);
static_assert(not ehanc::is_pack_uniform_v<int, bool, bool, bool, bool>);
static_assert(not ehanc::is_pack_uniform_v<bool, bool, int, bool, bool>);
static_assert(not ehanc::is_pack_uniform_v<bool, bool, bool, bool, int>);
static_assert(ehanc::is_pack_uniform_v<int, int>);
static_assert(not ehanc::is_pack_uniform_v<int, char>);
static_assert(ehanc::is_pack_uniform_v<int>);

auto test_is_iterable() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_iterable_v<std::vector<int>>, true);
  results.add_case(ehanc::is_iterable_v<std::list<int>>, true);
  results.add_case(ehanc::is_iterable_v<std::array<int, 5>>, true);
  // NOLINTNEXTLINE(*-avoid-c-arrays)
  results.add_case(ehanc::is_iterable_v<int[5]>, true);
  results.add_case(ehanc::is_iterable_v<int>, false);
  results.add_case(ehanc::is_iterable_v<char>, false);
  results.add_case(ehanc::is_iterable_v<bool>, false);
  results.add_case(ehanc::is_iterable_v<std::tuple<int, char, bool>>,
                   false);
  results.add_case(ehanc::is_iterable_v<std::pair<int, char>>, false);

  return results;
}

static_assert(ehanc::is_iterable_v<std::vector<int>>);
static_assert(ehanc::is_iterable_v<std::list<int>>);
static_assert(ehanc::is_iterable_v<std::array<int, 5>>);
// NOLINTNEXTLINE(*-avoid-c-arrays)
static_assert(ehanc::is_iterable_v<int[5]>);
static_assert(not ehanc::is_iterable_v<int>);
static_assert(not ehanc::is_iterable_v<char>);
static_assert(not ehanc::is_iterable_v<bool>);
static_assert(not ehanc::is_iterable_v<std::tuple<int, char, bool>>);
static_assert(not ehanc::is_iterable_v<std::pair<int, char>>);

auto test_is_iterator() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_iterator_v<int>, false);
  results.add_case(ehanc::is_iterator_v<std::vector<int>::iterator>, true);
  results.add_case(ehanc::is_iterator_v<std::array<int, 5>::iterator>,
                   true);
  results.add_case(ehanc::is_iterator_v<int*>, true);
  results.add_case(ehanc::is_iterator_v<const int*>, true);

  return results;
}

static_assert(not ehanc::is_iterator_v<int>);
static_assert(ehanc::is_iterator_v<std::vector<int>::iterator>);
static_assert(ehanc::is_iterator_v<std::array<int, 5>::iterator>);
static_assert(ehanc::is_iterator_v<int*>);
static_assert(ehanc::is_iterator_v<const int*>);

auto test_is_bidirectional() -> ehanc::test
{
  ehanc::test results;

  results.add_case(
      ehanc::is_bidirectional_v<std::forward_list<int>::iterator>, false);
  results.add_case(ehanc::is_bidirectional_v<std::vector<int>::iterator>,
                   true);
  results.add_case(ehanc::is_bidirectional_v<std::list<int>::iterator>,
                   true);
  results.add_case(ehanc::is_bidirectional_v<std::deque<int>::iterator>,
                   true);

  return results;
}

static_assert(
    not ehanc::is_bidirectional_v<std::forward_list<int>::iterator>);
static_assert(ehanc::is_bidirectional_v<std::vector<int>::iterator>);
static_assert(ehanc::is_bidirectional_v<std::list<int>::iterator>);
static_assert(ehanc::is_bidirectional_v<std::deque<int>::iterator>);

auto test_is_random_access() -> ehanc::test
{
  ehanc::test results;

  results.add_case(
      ehanc::is_random_access_v<std::forward_list<int>::iterator>, false);
  results.add_case(ehanc::is_random_access_v<std::vector<int>::iterator>,
                   true);
  results.add_case(ehanc::is_random_access_v<std::list<int>::iterator>,
                   false);
  results.add_case(ehanc::is_random_access_v<std::deque<int>::iterator>,
                   true);

  return results;
}

static_assert(
    not ehanc::is_random_access_v<std::forward_list<int>::iterator>);
static_assert(ehanc::is_random_access_v<std::vector<int>::iterator>);
static_assert(not ehanc::is_random_access_v<std::list<int>::iterator>);
static_assert(ehanc::is_random_access_v<std::deque<int>::iterator>);

auto test_is_tuple() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_tuple_v<std::tuple<int, char, double>>, true);
  results.add_case(ehanc::is_tuple_v<const std::tuple<int, char, double>>,
                   true);
  results.add_case(
      ehanc::is_tuple_v<volatile std::tuple<int, char, double>>, true);
  results.add_case(
      ehanc::is_tuple_v<const volatile std::tuple<int, char, double>>,
      true);
  results.add_case(ehanc::is_tuple_v<std::tuple<int, char, double>&>,
                   true);
  results.add_case(ehanc::is_tuple_v<const std::tuple<int, char, double>&>,
                   true);
  results.add_case(
      ehanc::is_tuple_v<volatile std::tuple<int, char, double>&>, true);
  results.add_case(
      ehanc::is_tuple_v<const volatile std::tuple<int, char, double>&>,
      true);
  results.add_case(ehanc::is_tuple_v<std::tuple<int, char, double>&&>,
                   true);
  results.add_case(
      ehanc::is_tuple_v<const std::tuple<int, char, double>&&>, true);
  results.add_case(
      ehanc::is_tuple_v<volatile std::tuple<int, char, double>&&>, true);
  results.add_case(
      ehanc::is_tuple_v<const volatile std::tuple<int, char, double>&&>,
      true);

  results.add_case(ehanc::is_tuple_v<std::tuple<int, char>>, true);
  results.add_case(ehanc::is_tuple_v<const std::tuple<int, char>>, true);
  results.add_case(ehanc::is_tuple_v<volatile std::tuple<int, char>>,
                   true);
  results.add_case(ehanc::is_tuple_v<const volatile std::tuple<int, char>>,
                   true);
  results.add_case(ehanc::is_tuple_v<std::tuple<int, char>&>, true);
  results.add_case(ehanc::is_tuple_v<const std::tuple<int, char>&>, true);
  results.add_case(ehanc::is_tuple_v<volatile std::tuple<int, char>&>,
                   true);
  results.add_case(
      ehanc::is_tuple_v<const volatile std::tuple<int, char>&>, true);
  results.add_case(ehanc::is_tuple_v<std::tuple<int, char>&&>, true);
  results.add_case(ehanc::is_tuple_v<const std::tuple<int, char>&&>, true);
  results.add_case(ehanc::is_tuple_v<volatile std::tuple<int, char>&&>,
                   true);
  results.add_case(
      ehanc::is_tuple_v<const volatile std::tuple<int, char>&&>, true);

  results.add_case(ehanc::is_tuple_v<std::pair<int, char>>, false);
  results.add_case(ehanc::is_tuple_v<const std::pair<int, char>>, false);
  results.add_case(ehanc::is_tuple_v<volatile std::pair<int, char>>,
                   false);
  results.add_case(ehanc::is_tuple_v<const volatile std::pair<int, char>>,
                   false);
  results.add_case(ehanc::is_tuple_v<std::pair<int, char>&>, false);
  results.add_case(ehanc::is_tuple_v<const std::pair<int, char>&>, false);
  results.add_case(ehanc::is_tuple_v<volatile std::pair<int, char>&>,
                   false);
  results.add_case(ehanc::is_tuple_v<const volatile std::pair<int, char>&>,
                   false);
  results.add_case(ehanc::is_tuple_v<std::pair<int, char>&&>, false);
  results.add_case(ehanc::is_tuple_v<const std::pair<int, char>&&>, false);
  results.add_case(ehanc::is_tuple_v<volatile std::pair<int, char>&&>,
                   false);
  results.add_case(
      ehanc::is_tuple_v<const volatile std::pair<int, char>&&>, false);

  results.add_case(ehanc::is_tuple_v<int>, false);

  return results;
}

static_assert(ehanc::is_tuple_v<std::tuple<int, char, double>>);
static_assert(ehanc::is_tuple_v<const std::tuple<int, char, double>>);
static_assert(ehanc::is_tuple_v<volatile std::tuple<int, char, double>>);
static_assert(
    ehanc::is_tuple_v<const volatile std::tuple<int, char, double>>);
static_assert(ehanc::is_tuple_v<std::tuple<int, char, double>&>);
static_assert(ehanc::is_tuple_v<const std::tuple<int, char, double>&>);
static_assert(ehanc::is_tuple_v<volatile std::tuple<int, char, double>&>);
static_assert(
    ehanc::is_tuple_v<const volatile std::tuple<int, char, double>&>);
static_assert(ehanc::is_tuple_v<std::tuple<int, char, double>&&>);
static_assert(ehanc::is_tuple_v<const std::tuple<int, char, double>&&>);
static_assert(ehanc::is_tuple_v<volatile std::tuple<int, char, double>&&>);
static_assert(
    ehanc::is_tuple_v<const volatile std::tuple<int, char, double>&&>);
static_assert(ehanc::is_tuple_v<std::tuple<int, char>>);
static_assert(ehanc::is_tuple_v<const std::tuple<int, char>>);
static_assert(ehanc::is_tuple_v<volatile std::tuple<int, char>>);
static_assert(ehanc::is_tuple_v<const volatile std::tuple<int, char>>);
static_assert(ehanc::is_tuple_v<std::tuple<int, char>&>);
static_assert(ehanc::is_tuple_v<const std::tuple<int, char>&>);
static_assert(ehanc::is_tuple_v<volatile std::tuple<int, char>&>);
static_assert(ehanc::is_tuple_v<const volatile std::tuple<int, char>&>);
static_assert(ehanc::is_tuple_v<std::tuple<int, char>&&>);
static_assert(ehanc::is_tuple_v<const std::tuple<int, char>&&>);
static_assert(ehanc::is_tuple_v<volatile std::tuple<int, char>&&>);
static_assert(ehanc::is_tuple_v<const volatile std::tuple<int, char>&&>);
static_assert(not ehanc::is_tuple_v<std::pair<int, char>>);
static_assert(not ehanc::is_tuple_v<const std::pair<int, char>>);
static_assert(not ehanc::is_tuple_v<volatile std::pair<int, char>>);
static_assert(not ehanc::is_tuple_v<const volatile std::pair<int, char>>);
static_assert(not ehanc::is_tuple_v<std::pair<int, char>&>);
static_assert(not ehanc::is_tuple_v<const std::pair<int, char>&>);
static_assert(not ehanc::is_tuple_v<volatile std::pair<int, char>&>);
static_assert(not ehanc::is_tuple_v<const volatile std::pair<int, char>&>);
static_assert(not ehanc::is_tuple_v<std::pair<int, char>&&>);
static_assert(not ehanc::is_tuple_v<const std::pair<int, char>&&>);
static_assert(not ehanc::is_tuple_v<volatile std::pair<int, char>&&>);
static_assert(
    not ehanc::is_tuple_v<const volatile std::pair<int, char>&&>);
static_assert(not ehanc::is_tuple_v<int>);

auto test_is_pair() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_pair_v<std::pair<int, char>>, true);
  results.add_case(ehanc::is_pair_v<const std::pair<int, char>>, true);
  results.add_case(ehanc::is_pair_v<volatile std::pair<int, char>>, true);
  results.add_case(ehanc::is_pair_v<const volatile std::pair<int, char>>,
                   true);
  results.add_case(ehanc::is_pair_v<std::pair<int, char>&>, true);
  results.add_case(ehanc::is_pair_v<const std::pair<int, char>&>, true);
  results.add_case(ehanc::is_pair_v<volatile std::pair<int, char>&>, true);
  results.add_case(ehanc::is_pair_v<const volatile std::pair<int, char>&>,
                   true);
  results.add_case(ehanc::is_pair_v<std::pair<int, char>&&>, true);
  results.add_case(ehanc::is_pair_v<const std::pair<int, char>&&>, true);
  results.add_case(ehanc::is_pair_v<volatile std::pair<int, char>&&>,
                   true);
  results.add_case(ehanc::is_pair_v<const volatile std::pair<int, char>&&>,
                   true);

  results.add_case(ehanc::is_pair_v<std::tuple<int, char>>, false);
  results.add_case(ehanc::is_pair_v<const std::tuple<int, char>>, false);
  results.add_case(ehanc::is_pair_v<volatile std::tuple<int, char>>,
                   false);
  results.add_case(ehanc::is_pair_v<const volatile std::tuple<int, char>>,
                   false);
  results.add_case(ehanc::is_pair_v<std::tuple<int, char>&>, false);
  results.add_case(ehanc::is_pair_v<const std::tuple<int, char>&>, false);
  results.add_case(ehanc::is_pair_v<volatile std::tuple<int, char>&>,
                   false);
  results.add_case(ehanc::is_pair_v<const volatile std::tuple<int, char>&>,
                   false);
  results.add_case(ehanc::is_pair_v<std::tuple<int, char>&&>, false);
  results.add_case(ehanc::is_pair_v<const std::tuple<int, char>&&>, false);
  results.add_case(ehanc::is_pair_v<volatile std::tuple<int, char>&&>,
                   false);
  results.add_case(
      ehanc::is_pair_v<const volatile std::tuple<int, char>&&>, false);

  results.add_case(ehanc::is_pair_v<int>, false);

  return results;
}

static_assert(ehanc::is_pair_v<std::pair<int, char>>);
static_assert(ehanc::is_pair_v<const std::pair<int, char>>);
static_assert(ehanc::is_pair_v<volatile std::pair<int, char>>);
static_assert(ehanc::is_pair_v<const volatile std::pair<int, char>>);
static_assert(ehanc::is_pair_v<std::pair<int, char>&>);
static_assert(ehanc::is_pair_v<const std::pair<int, char>&>);
static_assert(ehanc::is_pair_v<volatile std::pair<int, char>&>);
static_assert(ehanc::is_pair_v<const volatile std::pair<int, char>&>);
static_assert(ehanc::is_pair_v<std::pair<int, char>&&>);
static_assert(ehanc::is_pair_v<const std::pair<int, char>&&>);
static_assert(ehanc::is_pair_v<volatile std::pair<int, char>&&>);
static_assert(ehanc::is_pair_v<const volatile std::pair<int, char>&&>);
static_assert(not ehanc::is_pair_v<std::tuple<int, char>>);
static_assert(not ehanc::is_pair_v<const std::tuple<int, char>>);
static_assert(not ehanc::is_pair_v<volatile std::tuple<int, char>>);
static_assert(not ehanc::is_pair_v<const volatile std::tuple<int, char>>);
static_assert(not ehanc::is_pair_v<std::tuple<int, char>&>);
static_assert(not ehanc::is_pair_v<const std::tuple<int, char>&>);
static_assert(not ehanc::is_pair_v<volatile std::tuple<int, char>&>);
static_assert(not ehanc::is_pair_v<const volatile std::tuple<int, char>&>);
static_assert(not ehanc::is_pair_v<std::tuple<int, char>&&>);
static_assert(not ehanc::is_pair_v<const std::tuple<int, char>&&>);
static_assert(not ehanc::is_pair_v<volatile std::tuple<int, char>&&>);
static_assert(
    not ehanc::is_pair_v<const volatile std::tuple<int, char>&&>);
static_assert(not ehanc::is_pair_v<int>);

auto test_is_printable() -> ehanc::test
{
  ehanc::test results;

  results.add_case(ehanc::is_printable_v<int>, true);
  results.add_case(ehanc::is_printable_v<char>, true);
  results.add_case(ehanc::is_printable_v<bool>, true);
  results.add_case(ehanc::is_printable_v<std::string>, true);
  results.add_case(ehanc::is_printable_v<std::vector<int>>, false);
  results.add_case(ehanc::is_printable_v<std::list<std::string>>, false);

  return results;
}

static_assert(ehanc::is_printable_v<int>);
static_assert(ehanc::is_printable_v<char>);
static_assert(ehanc::is_printable_v<bool>);
static_assert(ehanc::is_printable_v<std::string>);
static_assert(not ehanc::is_printable_v<std::vector<int>>);
static_assert(not ehanc::is_printable_v<std::list<std::string>>);

void test_metaprogramming()
{
  ehanc::run_test("ehanc::sum_type", &test_sum_type);
  ehanc::run_test("ehanc::type_identity", &test_type_identity);
  ehanc::run_test("ehanc::is_type_in_pack", &test_is_type_in_pack);
  ehanc::run_test("ehanc::peel_first", &test_peel_first);
  ehanc::run_test("ehanc::peel_last", &test_peel_last);
  ehanc::run_test("ehanc::is_pack_uniform", &test_is_pack_uniform);
  ehanc::run_test("ehanc::is_iterable", &test_is_iterable);
  ehanc::run_test("ehanc::is_iterator", &test_is_iterator);
  ehanc::run_test("ehanc::is_bidirectional", &test_is_bidirectional);
  ehanc::run_test("ehanc::is_random_access", &test_is_random_access);
  ehanc::run_test("ehanc::is_tuple", &test_is_tuple);
  ehanc::run_test("ehanc::is_pair", &test_is_pair);
  ehanc::run_test("ehanc::is_printable", &test_is_printable);
}
