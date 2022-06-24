#include "test_etc.h"

#include "utils/lazy.hpp"

ehanc::test test_all_pass_cont()
{
  ehanc::test result;
  std::vector<int> yes{1, 2, 3, 4, 2, 3};
  std::array no{1, 2, 4, 9, 2, 3};
  auto checker = [](const int i) -> bool { return i < 6; };

  result.add_case(ehanc::all_pass(yes, checker), true,
                  "Simple true case failed");
  result.add_case(ehanc::all_pass(no, checker), false,
                  "Simple false case failed");

  return result;
}

ehanc::test test_all_pass_itr()
{
  ehanc::test result;
  std::vector<int> yes{1, 2, 3, 4, 2, 3};
  std::array no{1, 2, 4, 9, 2, 3};
  auto checker = [](const int i) -> bool { return i < 6; };

  result.add_case(ehanc::all_pass(yes.cbegin(), yes.cend(), checker), true,
                  "Simple true case failed");
  result.add_case(ehanc::all_pass(no.cbegin(), no.cend(), checker), false,
                  "Simple false case failed");

  return result;
}

ehanc::test test_min_size()
{
  ehanc::test result;
  std::array<int, 5> test1;
  std::array<int, 8> test2;
  std::array<int, 3> test3;
  std::array<int, 6> test4;

  result.add_case(ehanc::min_size(test1, test2, test3, test4), size_t{3});

  return result;
}

ehanc::test test_sum_type()
{
  ehanc::test result;

  result.add_case(std::is_same_v<ehanc::sum_type_t<int, int>, int>, true,
                  "int + int == int");
  result.add_case(std::is_same_v<ehanc::sum_type_t<short, short>, int>,
                  true, "short + short == int");
  result.add_case(
      std::is_same_v<ehanc::sum_type_t<double, double>, double>, true,
      "double + double == double");
  result.add_case(std::is_same_v<ehanc::sum_type_t<double, int>, double>,
                  true, "double + int == double");
  result.add_case(std::is_same_v<ehanc::sum_type_t<double, float>, double>,
                  true, "double + float == double");
  result.add_case(
      std::is_same_v<ehanc::sum_type_t<ehanc::lazy<int>, int>, int>, true,
      "ehanc::lazy<int> + int == int");
  result.add_case(std::is_same_v<ehanc::sum_type_t<int, short>, short>,
                  false, "int + short == short");
  result.add_case(std::is_same_v<ehanc::sum_type_t<double, int>, int>,
                  false, "double + int == int");
  result.add_case(std::is_same_v<ehanc::sum_type_t<char, int>, char>,
                  false, "char + int == char");

  return result;
}

ehanc::test test_comparison_metafunctions()
{
  ehanc::test result;

  result.add_case(ehanc::equal_v<5, 5>, true, "equal_v<5, 5>");
  result.add_case(ehanc::equal_v<1, 8>, false, "equal_v<1, 8>");
  result.add_case(ehanc::less_than_v<1, 8>, true, "less_than_v<1, 8>");
  result.add_case(ehanc::less_than_v<8, 8>, false, "less_than_v<8, 8>");
  result.add_case(ehanc::less_than_v<9, 8>, false, "less_than_v<9, 8>");
  result.add_case(ehanc::greater_than_v<1, 8>, false,
                  "greater_than_v<1, 8>");
  result.add_case(ehanc::greater_than_v<8, 8>, false,
                  "greater_than_v<8, 8>");
  result.add_case(ehanc::greater_than_v<9, 8>, true,
                  "greater_than_v<9, 8>");
  result.add_case(ehanc::less_eq_v<1, 8>, true, "less_eq_v<1, 8>");
  result.add_case(ehanc::less_eq_v<8, 8>, true, "less_eq_v<8, 8>");
  result.add_case(ehanc::less_eq_v<9, 8>, false, "less_eq_v<9, 8>");
  result.add_case(ehanc::greater_eq_v<1, 8>, false, "greater_eq_v<1, 8>");
  result.add_case(ehanc::greater_eq_v<8, 8>, true, "greater_eq_v<8, 8>");
  result.add_case(ehanc::greater_eq_v<9, 8>, true, "greater_eq_v<9, 8>");

  return result;
}

ehanc::test test_bool_op_metafunctions()
{
  ehanc::test result;

  result.add_case(ehanc::bool_and_v<true, true>, true,
                  "bool_and_v<true, true>");
  result.add_case(ehanc::bool_and_v<true, false>, false,
                  "bool_and_v<true, false>");
  result.add_case(ehanc::bool_and_v<false, true>, false,
                  "bool_and_v<false, true>");
  result.add_case(ehanc::bool_and_v<false, false>, false,
                  "bool_and_v<false, false>");
  result.add_case(ehanc::bool_or_v<true, true>, true,
                  "bool_or_v<true, true>");
  result.add_case(ehanc::bool_or_v<true, false>, true,
                  "bool_or_v<true, false>");
  result.add_case(ehanc::bool_or_v<false, true>, true,
                  "bool_or_v<false, true>");
  result.add_case(ehanc::bool_or_v<false, false>, false,
                  "bool_or_v<false, false>");

  return result;
}

ehanc::test test_size_t_literals()
{
  ehanc::test result;

  using namespace ehanc::literals::size_t_literal;
  /* using namespace ehanc::literals; */ // also works
  std::size_t i{500};
  auto j{500_z};

  result.add_case(std::is_same_v<decltype(j), std::size_t>, true,
                  "Type is not std::size_t");
  result.add_case(i, j, "Value is not as expected");

  return result;
}

void test_etc()
{
  ehanc::run_test("ehanc::all_pass (container)", &test_all_pass_cont);
  ehanc::run_test("ehanc::all_pass (iterators)", &test_all_pass_itr);
  ehanc::run_test("ehanc::min_size", &test_min_size);
  ehanc::run_test("ehanc::sum_type", &test_sum_type);
  ehanc::run_test("comparison metafunctions",
                  &test_comparison_metafunctions);
  ehanc::run_test("boolean ops metafunctions",
                  &test_bool_op_metafunctions);
  ehanc::run_test("ehanc::operator\"\"_z", &test_size_t_literals);
}
