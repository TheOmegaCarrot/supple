#include <array>
#include <forward_list>
#include <list>
#include <queue>
#include <type_traits>
#include <vector>

#include "supl/metaprogramming.hpp"
#include "supl/test_metaprogramming.h"
#include "test_utils.hpp"

static auto pass() -> ehanc::test
{
  return ehanc::test {};
}

// all tests happen at compile time anyway
// if this TU compiles, all tests pass
void test_metaprogramming()
{
  ehanc::run_test("supl::sum_type", &pass);
  ehanc::run_test("supl::is_type_in_pack", &pass);
  ehanc::run_test("supl::peel_first", &pass);
  ehanc::run_test("supl::peel_last", &pass);
  ehanc::run_test("supl::is_pack_uniform", &pass);
  ehanc::run_test("supl::is_pack_only", &pass);
  ehanc::run_test("supl::is_iterable", &pass);
  ehanc::run_test("supl::is_iterator", &pass);
  ehanc::run_test("supl::is_bidirectional", &pass);
  ehanc::run_test("supl::is_random_access", &pass);
  ehanc::run_test("supl::is_tuple", &pass);
  ehanc::run_test("supl::is_pair", &pass);
  ehanc::run_test("supl::is_printable", &pass);
  ehanc::run_test("supl::are_equality_comparable", &pass);
  ehanc::run_test("supl::are_inequality_comparable", &pass);
  ehanc::run_test("supl::are_less_comparable", &pass);
  ehanc::run_test("supl::are_less_eq_comparable", &pass);
  ehanc::run_test("supl::are_greater_comparable", &pass);
  ehanc::run_test("supl::are_greater_eq_comparable", &pass);
}

///////////////////////////////////////////// sum_type

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

///////////////////////////////////////////// is_type_in_pack

static_assert(supl::is_type_in_pack_v<int, char, bool, int>);
static_assert(supl::is_type_in_pack_v<int, int, bool, int>);
static_assert(not supl::is_type_in_pack_v<int, char, bool, char, char,
                                          char, float, double>);
static_assert(supl::is_type_in_pack_v<int, int, int, int>);
static_assert(supl::is_type_in_pack_v<int, int>);
static_assert(not supl::is_type_in_pack_v<int, char>);

///////////////////////////////////////////// peel_first

static_assert(
    std::is_same_v<supl::peel_first_t<int, char, char, char>, int>);
static_assert(std::is_same_v<supl::peel_first_t<int, int, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_first_t<int, char, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_first_t<char, int, int, int>, char>);

///////////////////////////////////////////// peel_last

static_assert(
    std::is_same_v<supl::peel_last_t<char, char, char, int>, int>);
static_assert(std::is_same_v<supl::peel_last_t<int, int, int, int>, int>);
static_assert(std::is_same_v<supl::peel_last_t<int, char, int, int>, int>);
static_assert(
    std::is_same_v<supl::peel_last_t<int, int, int, char>, char>);

///////////////////////////////////////////// is_pack_uniform

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

///////////////////////////////////////////// is_pack_only

static_assert(supl::is_pack_only_v<double, double, double, double>);
static_assert(not supl::is_pack_only_v<double, double, double, int>);
static_assert(supl::is_pack_only_v<int, int, int, int>);
static_assert(not supl::is_pack_only_v<int, double, double, double>);
static_assert(
    not supl::is_pack_only_v<std::string, std::string_view,
                             std::string_view, std::string_view>);
static_assert(not supl::is_pack_only_v<double>);
static_assert(not supl::is_pack_only_v<void>);

///////////////////////////////////////////// is_iterable

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

///////////////////////////////////////////// is_iterator

static_assert(not supl::is_iterator_v<int>);
static_assert(supl::is_iterator_v<std::vector<int>::iterator>);
static_assert(supl::is_iterator_v<std::array<int, 5>::iterator>);
static_assert(supl::is_iterator_v<int*>);
static_assert(supl::is_iterator_v<const int*>);

///////////////////////////////////////////// is_bidirectional

static_assert(
    not supl::is_bidirectional_v<std::forward_list<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::vector<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::list<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::deque<int>::iterator>);

///////////////////////////////////////////// is_random_access

static_assert(
    not supl::is_random_access_v<std::forward_list<int>::iterator>);
static_assert(supl::is_random_access_v<std::vector<int>::iterator>);
static_assert(not supl::is_random_access_v<std::list<int>::iterator>);
static_assert(supl::is_random_access_v<std::deque<int>::iterator>);

///////////////////////////////////////////// is_tuple

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
static_assert(not supl::is_tuple_v<const volatile std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<int>);

///////////////////////////////////////////// is_pair

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
static_assert(not supl::is_pair_v<const volatile std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<int>);

///////////////////////////////////////////// is_printable

static_assert(supl::is_printable_v<int>);
static_assert(supl::is_printable_v<char>);
static_assert(supl::is_printable_v<bool>);
static_assert(supl::is_printable_v<std::string>);
static_assert(not supl::is_printable_v<std::vector<int>>);
static_assert(not supl::is_printable_v<std::list<std::string>>);

///////////////////////////////////////////// are_equality_comparable

static_assert(supl::are_equality_comparable_v<int, int>);
static_assert(supl::are_equality_comparable_v<int, char>);
static_assert(supl::are_equality_comparable_v<int, float>);
static_assert(supl::are_equality_comparable_v<int, double>);
static_assert(supl::are_equality_comparable_v<int, bool>);
static_assert(supl::are_equality_comparable_v<int, char>);
static_assert(not supl::are_equality_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_inequality_comparable

static_assert(supl::are_inequality_comparable_v<int, int>);
static_assert(supl::are_inequality_comparable_v<int, char>);
static_assert(supl::are_inequality_comparable_v<int, float>);
static_assert(supl::are_inequality_comparable_v<int, double>);
static_assert(supl::are_inequality_comparable_v<int, bool>);
static_assert(supl::are_inequality_comparable_v<int, char>);
static_assert(
    not supl::are_inequality_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_less_comparable

static_assert(supl::are_less_comparable_v<int, int>);
static_assert(supl::are_less_comparable_v<int, char>);
static_assert(supl::are_less_comparable_v<int, float>);
static_assert(supl::are_less_comparable_v<int, double>);
static_assert(supl::are_less_comparable_v<int, bool>);
static_assert(supl::are_less_comparable_v<int, char>);
static_assert(not supl::are_less_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_less_eq_comparable

static_assert(supl::are_less_eq_comparable_v<int, int>);
static_assert(supl::are_less_eq_comparable_v<int, char>);
static_assert(supl::are_less_eq_comparable_v<int, float>);
static_assert(supl::are_less_eq_comparable_v<int, double>);
static_assert(supl::are_less_eq_comparable_v<int, bool>);
static_assert(supl::are_less_eq_comparable_v<int, char>);
static_assert(not supl::are_less_eq_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_greater_comparable

static_assert(supl::are_greater_comparable_v<int, int>);
static_assert(supl::are_greater_comparable_v<int, char>);
static_assert(supl::are_greater_comparable_v<int, float>);
static_assert(supl::are_greater_comparable_v<int, double>);
static_assert(supl::are_greater_comparable_v<int, bool>);
static_assert(supl::are_greater_comparable_v<int, char>);
static_assert(not supl::are_greater_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_greater_eq_comparable

static_assert(supl::are_greater_eq_comparable_v<int, int>);
static_assert(supl::are_greater_eq_comparable_v<int, char>);
static_assert(supl::are_greater_eq_comparable_v<int, float>);
static_assert(supl::are_greater_eq_comparable_v<int, double>);
static_assert(supl::are_greater_eq_comparable_v<int, bool>);
static_assert(supl::are_greater_eq_comparable_v<int, char>);
static_assert(
    not supl::are_greater_eq_comparable_v<int, std::vector<int>>);
