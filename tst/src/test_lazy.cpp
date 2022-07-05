#include "test_lazy.h"

#include <iostream>

ehanc::test test_lazy_class()
{
  ehanc::test results;

  std::vector<int> check;

  ehanc::lazy<int> test([&check, i{5}]() mutable -> int {
    check.push_back(2);
    return i++;
  });

  check.push_back(1);

  results.add_case(test.get(), 5, "Incorrect evaluation");
  results.add_case(check[0], 1, "Likely early evaluation");
  results.add_case(check[1], 2, "Likely early evaluation");
  results.add_case(test.func()(), 6,
                   "Stateful functions not able to be so");
  results.add_case(test.get(), 5, "Value changed");

  return results;
}

ehanc::test test_lazy_has_value()
{
  ehanc::test results;

  auto test{ehanc::make_lazy([]() { return 5; })};

  results.add_case(test.has_value(), false,
                   "Should not have value before use");

  [[maybe_unused]] auto discarded{test.get()};

  results.add_case(test.has_value(), true, "Should have value after use");

  return results;
}

ehanc::test test_make_lazy()
{
  ehanc::test results;

  auto test{ehanc::make_lazy([]() { return 5; })};

  results.add_case(test.get(), 5, "Incorrect evaluation");
  results.add_case(test.get(), 5, "Incorrect evaluation");

  return results;
}

ehanc::test test_is_lazy()
{
  ehanc::test results;

  results.add_case(ehanc::is_lazy_v<ehanc::lazy<int>>, true);
  results.add_case(ehanc::is_lazy_v<const ehanc::lazy<int>>, true);
  results.add_case(ehanc::is_lazy_v<volatile ehanc::lazy<int>>, true);
  results.add_case(ehanc::is_lazy_v<const volatile ehanc::lazy<int>>,
                   true);
  results.add_case(ehanc::is_lazy_v<ehanc::lazy<int>&>, true);
  results.add_case(ehanc::is_lazy_v<const ehanc::lazy<int>&>, true);
  results.add_case(ehanc::is_lazy_v<volatile ehanc::lazy<int>&>, true);
  results.add_case(ehanc::is_lazy_v<const volatile ehanc::lazy<int>&>,
                   true);
  results.add_case(ehanc::is_lazy_v<ehanc::lazy<int>&&>, true);
  results.add_case(ehanc::is_lazy_v<const ehanc::lazy<int>&&>, true);
  results.add_case(ehanc::is_lazy_v<volatile ehanc::lazy<int>&&>, true);
  results.add_case(ehanc::is_lazy_v<const volatile ehanc::lazy<int>&&>,
                   true);
  results.add_case(ehanc::is_lazy_v<int>, false);

  return results;
}

static_assert(ehanc::is_lazy_v<ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_v<const ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_v<volatile ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_v<const volatile ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_v<ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_v<const ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_v<volatile ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_v<const volatile ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_v<ehanc::lazy<int>&&>);
static_assert(ehanc::is_lazy_v<const ehanc::lazy<int>&&>);
static_assert(ehanc::is_lazy_v<volatile ehanc::lazy<int>&&>);
static_assert(ehanc::is_lazy_v<const volatile ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_v<int>);

ehanc::test test_is_lazy_of()
{
  ehanc::test results;

  results.add_case(ehanc::is_lazy_of_v<int, ehanc::lazy<int>>, true);
  results.add_case(ehanc::is_lazy_of_v<int, const ehanc::lazy<int>>, true);
  results.add_case(ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>>,
                   true);
  results.add_case(
      ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>>, true);
  results.add_case(ehanc::is_lazy_of_v<int, ehanc::lazy<int>&>, true);
  results.add_case(ehanc::is_lazy_of_v<int, const ehanc::lazy<int>&>,
                   true);
  results.add_case(ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>&>,
                   true);
  results.add_case(
      ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>&>, true);
  results.add_case(ehanc::is_lazy_of_v<int, ehanc::lazy<int>&&>, true);
  results.add_case(ehanc::is_lazy_of_v<int, const ehanc::lazy<int>&&>,
                   true);
  results.add_case(ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>&&>,
                   true);
  results.add_case(
      ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>&&>, true);
  results.add_case(ehanc::is_lazy_of_v<char, ehanc::lazy<int>>, false);
  results.add_case(ehanc::is_lazy_of_v<char, const ehanc::lazy<int>>,
                   false);
  results.add_case(ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>>,
                   false);
  results.add_case(
      ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>>, false);
  results.add_case(ehanc::is_lazy_of_v<char, ehanc::lazy<int>&&>, false);
  results.add_case(ehanc::is_lazy_of_v<char, const ehanc::lazy<int>&&>,
                   false);
  results.add_case(ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>&&>,
                   false);
  results.add_case(
      ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>&&>, false);
  results.add_case(ehanc::is_lazy_of_v<char, ehanc::lazy<int>&&>, false);
  results.add_case(ehanc::is_lazy_of_v<char, const ehanc::lazy<int>&&>,
                   false);
  results.add_case(ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>&&>,
                   false);
  results.add_case(
      ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>&&>, false);
  results.add_case(ehanc::is_lazy_of_v<int, int>, false);
  results.add_case(ehanc::is_lazy_of_v<int, char>, false);

  return results;
}

static_assert(ehanc::is_lazy_of_v<int, ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_of_v<int, const ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>>);
static_assert(ehanc::is_lazy_of_v<int, ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_of_v<int, const ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>&>);
static_assert(ehanc::is_lazy_of_v<int, ehanc::lazy<int>&&>);
static_assert(ehanc::is_lazy_of_v<int, const ehanc::lazy<int>&&>);
static_assert(ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>&&>);
static_assert(ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<char, ehanc::lazy<int>>);
static_assert(not ehanc::is_lazy_of_v<char, const ehanc::lazy<int>>);
static_assert(not ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>>);
static_assert(
    not ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>>);
static_assert(not ehanc::is_lazy_of_v<char, ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<char, const ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>&&>);
static_assert(
    not ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<char, ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<char, const ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>&&>);
static_assert(
    not ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>&&>);
static_assert(not ehanc::is_lazy_of_v<int, int>);
static_assert(not ehanc::is_lazy_of_v<int, char>);

ehanc::test test_lazy_inner_type()
{
  ehanc::test results;

  results.add_case(
      std::is_same_v<int, ehanc::lazy_inner_type_t<ehanc::lazy<int>>>,
      true);
  results.add_case(
      std::is_same_v<int,
                     ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>,
      true);
  results.add_case(
      std::is_same_v<int,
                     ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>,
      true);
  results.add_case(
      std::is_same_v<
          int, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>,
      true);
  results.add_case(
      std::is_same_v<int, ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>,
      true);
  results.add_case(
      std::is_same_v<int,
                     ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>,
      true);
  results.add_case(
      std::is_same_v<int,
                     ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>,
      true);
  results.add_case(
      std::is_same_v<
          int, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>,
      true);
  results.add_case(
      std::is_same_v<int, ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>,
      true);
  results.add_case(
      std::is_same_v<int,
                     ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>,
      true);
  results.add_case(
      std::is_same_v<
          int, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>,
      true);
  results.add_case(
      std::is_same_v<int, ehanc::lazy_inner_type_t<
                              const volatile ehanc::lazy<int>&&>>,
      true);
  results.add_case(
      std::is_same_v<char, ehanc::lazy_inner_type_t<ehanc::lazy<int>>>,
      false);
  results.add_case(
      std::is_same_v<char,
                     ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>,
      false);
  results.add_case(
      std::is_same_v<char,
                     ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>,
      false);
  results.add_case(
      std::is_same_v<
          char, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>,
      false);
  results.add_case(
      std::is_same_v<char, ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>,
      false);
  results.add_case(
      std::is_same_v<char,
                     ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>,
      false);
  results.add_case(
      std::is_same_v<char,
                     ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>,
      false);
  results.add_case(
      std::is_same_v<char, ehanc::lazy_inner_type_t<
                               const volatile ehanc::lazy<int>&>>,
      false);
  results.add_case(
      std::is_same_v<char, ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>,
      false);
  results.add_case(
      std::is_same_v<char,
                     ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>,
      false);
  results.add_case(
      std::is_same_v<
          char, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>,
      false);
  results.add_case(
      std::is_same_v<char, ehanc::lazy_inner_type_t<
                               const volatile ehanc::lazy<int>&&>>,
      false);
  results.add_case(std::is_same_v<int, ehanc::lazy_inner_type_t<int>>,
                   true);

  return results;
}

static_assert(
    std::is_same_v<int, ehanc::lazy_inner_type_t<ehanc::lazy<int>>>);
static_assert(
    std::is_same_v<int, ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>);
static_assert(std::is_same_v<
              int, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>);
static_assert(
    std::is_same_v<
        int, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>);
static_assert(
    std::is_same_v<int, ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>);
static_assert(std::is_same_v<
              int, ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>);
static_assert(std::is_same_v<
              int, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>);
static_assert(
    std::is_same_v<
        int, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>);
static_assert(
    std::is_same_v<int, ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>);
static_assert(std::is_same_v<
              int, ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>);
static_assert(std::is_same_v<
              int, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>);
static_assert(
    std::is_same_v<
        int, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&&>>);
static_assert(
    not std::is_same_v<char, ehanc::lazy_inner_type_t<ehanc::lazy<int>>>);
static_assert(not std::is_same_v<
              char, ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>);
static_assert(not std::is_same_v<
              char, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>);
static_assert(
    not std::is_same_v<
        char, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>);
static_assert(
    not std::is_same_v<char, ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>);
static_assert(not std::is_same_v<
              char, ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>);
static_assert(not std::is_same_v<
              char, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>);
static_assert(
    not std::is_same_v<
        char, ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>);
static_assert(not std::is_same_v<
              char, ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>);
static_assert(not std::is_same_v<
              char, ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>);
static_assert(
    not std::is_same_v<
        char, ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>);
static_assert(
    not std::is_same_v<char, ehanc::lazy_inner_type_t<
                                 const volatile ehanc::lazy<int>&&>>);
static_assert(std::is_same_v<int, ehanc::lazy_inner_type_t<int>>);

void test_lazy()
{
  ehanc::run_test("ehanc::lazy", &test_lazy_class);
  ehanc::run_test("ehanc::lazy::has_value", &test_lazy_has_value);
  ehanc::run_test("ehanc::make_lazy", &test_make_lazy);
  ehanc::run_test("ehanc::is_lazy", &test_is_lazy);
  ehanc::run_test("ehanc::is_lazy_of", &test_is_lazy_of);
  ehanc::run_test("ehanc::lazy_inner_type", &test_lazy_inner_type);
}
