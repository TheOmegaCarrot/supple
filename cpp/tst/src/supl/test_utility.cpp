#include <array>
#include <cstddef>
#include <list>
#include <type_traits>
#include <vector>

#include "supl/test_utility.h"

#include "supl/utility.hpp"

struct copy_counter {
  copy_counter()                                       = default;
  ~copy_counter()                                      = default;
  copy_counter(copy_counter&&)                         = default;
  auto operator=(const copy_counter&) -> copy_counter& = default;
  auto operator=(copy_counter&&) -> copy_counter&      = default;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static int copy_count;

  copy_counter([[maybe_unused]] const copy_counter& src)
  {
    ++copy_count;
  }
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
int copy_counter::copy_count {0};

static auto test_explicit_copy() -> ehanc::test
{
  ehanc::test results;
  copy_counter test;

  results.add_case(copy_counter::copy_count, 0,
                   "Counter not properly initialized");

  [[maybe_unused]] copy_counter test2 = supl::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 1,
                   "Incorrect number of copies");

  [[maybe_unused]] copy_counter test3 = supl::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 2,
                   "Incorrect number of copies");

  [[maybe_unused]] copy_counter test4 = supl::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 3,
                   "Incorrect number of copies");

  [[maybe_unused]] copy_counter test5 = supl::explicit_copy(test);

  results.add_case(copy_counter::copy_count, 4,
                   "Incorrect number of copies");

  return results;
}

static auto test_to_stream() -> ehanc::test
{
  ehanc::test results;

  using namespace std::literals;

  std::stringstream str1;
  std::tuple test1 {1, "hello", true};
  supl::to_stream(str1, test1);
  results.add_case(str1.str(), "( 1, hello, true )"s, "tuple");

  std::stringstream str2;
  std::pair test2 {42, "Neat"s};
  supl::to_stream(str2, test2);
  results.add_case(str2.str(), "( 42, Neat )"s, "pair");

  std::stringstream str3;
  std::vector test3 {1, 2, 42, 81};
  supl::to_stream(str3, test3);
  results.add_case(str3.str(), "[ 1, 2, 42, 81 ]"s, "vector");

  std::stringstream str4;
  std::list<std::pair<int, bool>> test4 {
      {1,  true},
      {2, false},
      {5,  true}
  };
  supl::to_stream(str4, test4);
  results.add_case(str4.str(),
                   "[ ( 1, true ), ( 2, false ), ( 5, true ) ]"s,
                   "List of tuples");

  std::stringstream str5;
  supl::to_stream(str5, 1);
  results.add_case(str5.str(), "1"s, "int");

  std::stringstream str6;
  supl::to_stream(str6, std::vector<int> {});
  results.add_case(str6.str(), "[ ]"s, "empty vector");

  std::stringstream str7;
  supl::to_stream(str7, std::tuple<> {});
  results.add_case(str7.str(), "( )"s);

  supl::to_stream(str7, true);
  results.add_case(str7.str(), "( )true"s);

  str7 << true;
  results.add_case(str7.str(), "( )true1"s);

  std::stringstream str8;
  supl::to_stream(str8, std::tuple<int> {5});
  results.add_case(str8.str(), "( 5 )"s);

  std::stringstream str9;
  std::tuple test9 {test1, test3, false};
  supl::to_stream(str9, test9);

  results.add_case(str9.str(),
                   "( ( 1, hello, true ), [ 1, 2, 42, 81 ], false )"s);

  return results;
}

static auto test_to_string() -> ehanc::test
{
  ehanc::test results;

  using namespace std::literals;

  std::tuple test1 {1, "hello", true};
  results.add_case(supl::to_string(test1), "( 1, hello, true )"s, "tuple");

  std::pair test2 {42, "Neat"s};
  results.add_case(supl::to_string(test2), "( 42, Neat )"s, "pair");

  std::vector test3 {1, 2, 42, 81};
  results.add_case(supl::to_string(test3), "[ 1, 2, 42, 81 ]"s, "vector");

  std::list<std::pair<int, bool>> test4 {
      {1,  true},
      {2, false},
      {5,  true}
  };
  results.add_case(supl::to_string(test4),
                   "[ ( 1, true ), ( 2, false ), ( 5, true ) ]"s,
                   "List of tuples");

  results.add_case(supl::to_string(1), "1"s, "int");

  results.add_case(supl::to_string(std::vector<int> {}), "[ ]"s,
                   "empty vector");

  results.add_case(supl::to_string(std::tuple<> {}), "( )"s);

  results.add_case(supl::to_string(std::tuple<int> {5}), "( 5 )"s);

  std::tuple test5 {test1, test3, false};

  results.add_case(supl::to_string(test5),
                   "( ( 1, hello, true ), [ 1, 2, 42, 81 ], false )"s);

  return results;
}

static auto test_size_t_literals() -> ehanc::test
{
  ehanc::test results;

  using supl::size_t_literal::operator""_z;
  /* using namespace supl::literals::size_t_literal; */ // also works
  /* using namespace supl::literals; */                 // also works
  std::size_t i {500};
  auto j {500_z};

  results.add_case(std::is_same_v<decltype(j), std::size_t>, true,
                   "Type is not std::size_t");
  results.add_case(i, j, "Value is not as expected");

  return results;
}

void test_utility()
{
  ehanc::run_test("supl::explicit_copy", &test_explicit_copy);
  ehanc::run_test("supl::to_stream", &test_to_stream);
  ehanc::run_test("supl::to_string", &test_to_string);
  ehanc::run_test("supl::literals::size_t_literal::operator\"\"_z",
                  &test_size_t_literals);
}
