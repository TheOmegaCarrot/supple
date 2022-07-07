#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "test_etc.h"

#include "utils/etc.hpp"
#include "utils/lazy.hpp"

auto test_min_size() -> ehanc::test
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
int copy_counter::copy_count{0};

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

auto test_size_t_literals() -> ehanc::test
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
  ehanc::run_test("ehanc::min_size", &test_min_size);
  ehanc::run_test("ehanc::explicit_copy", &test_explicit_copy);
  ehanc::run_test("ehanc::literals::size_t_literal::operator\"\"_z",
                  &test_size_t_literals);
}
