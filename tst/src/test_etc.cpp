#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "test_etc.h"

#include "utils/etc.hpp"
#include "utils/lazy.hpp"

ehanc::test test_all_pass_cont()
{
  ehanc::test results;
  std::vector<int> yes{1, 2, 3, 4, 2, 3};
  std::array no{1, 2, 4, 9, 2, 3};
  auto checker = [](const int i) -> bool { return i < 6; };

  results.add_case(ehanc::all_pass(yes, checker), true,
                   "Simple true case failed");
  results.add_case(ehanc::all_pass(no, checker), false,
                   "Simple false case failed");

  return results;
}

ehanc::test test_all_pass_itr()
{
  ehanc::test results;
  std::vector<int> yes{1, 2, 3, 4, 2, 3};
  std::array no{1, 2, 4, 9, 2, 3};
  auto checker = [](const int i) -> bool { return i < 6; };

  results.add_case(ehanc::all_pass(yes.cbegin(), yes.cend(), checker),
                   true, "Simple true case failed");
  results.add_case(ehanc::all_pass(no.cbegin(), no.cend(), checker), false,
                   "Simple false case failed");

  return results;
}

ehanc::test test_min_size()
{
  ehanc::test results;
  std::array<int, 5> test1{};
  std::array<int, 8> test2{};
  std::array<int, 3> test3{};
  std::array<int, 6> test4{};

  results.add_case(ehanc::min_size(test1, test2, test3, test4),
                   std::size_t{3});

  return results;
}

struct copy_counter {
  copy_counter()               = default;
  ~copy_counter()              = default;
  copy_counter(copy_counter&&) = default;
  copy_counter& operator=(const copy_counter&) = default;
  copy_counter& operator=(copy_counter&&) = default;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static int copy_count;
  copy_counter([[maybe_unused]] const copy_counter& src)
  {
    ++copy_count;
  }
};
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
int copy_counter::copy_count{0};

ehanc::test test_explicit_copy()
{
  ehanc::test results;
  copy_counter test;

  results.add_case(copy_counter::copy_count, 0,
                   "Counter not properly initialized");

  [[maybe_unused]] copy_counter test2 = ehanc::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 1,
                   "Incorrect number of copies");

  [[maybe_unused]] copy_counter test3 = ehanc::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 2,
                   "Incorrect number of copies");

  [[maybe_unused]] copy_counter test4 = ehanc::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 3,
                   "Incorrect number of copies");

  [[maybe_unused]] copy_counter test5 = ehanc::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 4,
                   "Incorrect number of copies");

  return results;
}

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

  // I want this to work!!!
  /* results.add_case(ehanc::is_pack_uniform_v<int>, true); */

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

ehanc::test test_size_t_literals()
{
  ehanc::test results;

  using namespace ehanc::literals::size_t_literal;
  /* using namespace ehanc::literals; */ // also works
  std::size_t i{500};
  auto j{500_z};

  results.add_case(std::is_same_v<decltype(j), std::size_t>, true,
                   "Type is not std::size_t");
  results.add_case(i, j, "Value is not as expected");

  return results;
}

void test_etc()
{
  ehanc::run_test("ehanc::all_pass (container)", &test_all_pass_cont);
  ehanc::run_test("ehanc::all_pass (iterators)", &test_all_pass_itr);
  ehanc::run_test("ehanc::min_size", &test_min_size);
  ehanc::run_test("ehanc::explicit_copy", &test_explicit_copy);
  ehanc::run_test("ehanc::sum_type", &test_sum_type);
  ehanc::run_test("type_identity", &test_type_identity);
  ehanc::run_test("is_type_in_pack", &test_is_type_in_pack);
  ehanc::run_test("is_pack_uniform", &test_is_pack_uniform);
  ehanc::run_test("pack_size", &test_pack_size);
  ehanc::run_test("ehanc::operator\"\"_z", &test_size_t_literals);
}
