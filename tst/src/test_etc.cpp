#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "test_etc.h"

#include "utils/etc.hpp"
#include "utils/lazy.hpp"

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

auto test_explicit_copy() -> ehanc::test
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

auto test_to_string() -> ehanc::test
{
  ehanc::test results;

  using namespace std::literals;

  std::tuple test1 {1, "hello", true};
  results.add_case(ehanc::to_string(test1), "( 1, hello, true )"s,
                   "tuple");

  std::pair test2 {42, "Neat"s};
  results.add_case(ehanc::to_string(test2), "( 42, Neat )"s, "pair");

  std::vector test3 {1, 2, 42, 81};
  results.add_case(ehanc::to_string(test3), "[ 1, 2, 42, 81 ]"s, "vector");

  results.add_case(ehanc::to_string(1), "1"s, "int");

  return results;
}

auto test_size_t_literals() -> ehanc::test
{
  ehanc::test results;

  using namespace ehanc::size_t_literal;
  /* using namespace ehanc::literals::size_t_literal; */ // also works
  /* using namespace ehanc::literals; */                 // also works
  std::size_t i {500};
  auto j {500_z};

  results.add_case(std::is_same_v<decltype(j), std::size_t>, true,
                   "Type is not std::size_t");
  results.add_case(i, j, "Value is not as expected");

  return results;
}

void test_etc()
{
  ehanc::run_test("ehanc::explicit_copy", &test_explicit_copy);
  ehanc::run_test("ehanc::to_string", &test_to_string);
  ehanc::run_test("ehanc::literals::size_t_literal::operator\"\"_z",
                  &test_size_t_literals);
}
