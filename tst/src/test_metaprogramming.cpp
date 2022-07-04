#include "test_metaprogramming.h"
#include "test_utils.hpp"

#include "utils/lazy.hpp"

ehanc::test test_sum_type()
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
                   false, "int + short == short");
  results.add_case(std::is_same_v<ehanc::sum_type_t<double, int>, int>,
                   false, "double + int == int");
  results.add_case(std::is_same_v<ehanc::sum_type_t<char, int>, char>,
                   false, "char + int == char");
  results.add_case(
      std::is_same_v<ehanc::sum_type_t<short, short, short, short, int>,
                     int>,
      true, "short + short + short + short + int == int");

  return results;
}

ehanc::test test_type_identity()
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

ehanc::test test_is_type_in_pack()
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

ehanc::test test_peel_first()
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

ehanc::test test_peel_last()
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

ehanc::test test_is_pack_uniform()
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

ehanc::test test_pack_size()
{
  ehanc::test results;

  results.add_case(ehanc::pack_size_v<int>, std::size_t{1});
  results.add_case(ehanc::pack_size_v<int, char>, std::size_t{2});
  results.add_case(ehanc::pack_size_v<int, bool, float>, std::size_t{3});
  results.add_case(ehanc::pack_size_v<int, double, long, short>,
                   std::size_t{4});
  results.add_case(ehanc::pack_size_v<int, char, bool, int, int>,
                   std::size_t{5});

  return results;
}

void test_metaprogramming()
{
  ehanc::run_test("ehanc::sum_type", &test_sum_type);
  ehanc::run_test("ehanc::type_identity", &test_type_identity);
  ehanc::run_test("ehanc::is_type_in_pack", &test_is_type_in_pack);
  ehanc::run_test("ehanc::peel_first", &test_peel_first);
  ehanc::run_test("ehanc::peel_last", &test_peel_last);
  ehanc::run_test("ehanc::is_pack_uniform", &test_is_pack_uniform);
  ehanc::run_test("ehanc::pack_size", &test_pack_size);
}
