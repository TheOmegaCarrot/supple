#include "supl/test_iterators.h"
#include "test_utils.hpp"

#include <array>
#include <forward_list>
#include <numeric>
#include <vector>

#include "supl/utility.hpp"

static auto test_last() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::iota(test1.begin(), test1.end(), 1);

  std::array<int, 2> test2 {2, 3};
  std::vector<int> test3;
  std::forward_list<int> test4 {1, 2, 3, 4};

  results.add_case(*supl::last(test1), 5);
  results.add_case(*supl::last(test2), 3);
  results.add_case(supl::last(test3) == std::begin(test3), true);
  results.add_case(*supl::last(test4), 4);

  return results;
}

static auto test_clast() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test1 {};
  std::iota(test1.begin(), test1.end(), 1);

  std::array<int, 2> test2 {2, 3};
  std::vector<int> test3;
  std::forward_list<int> test4 {1, 2, 3, 4};

  results.add_case(*supl::clast(test1), 5);
  results.add_case(*supl::clast(test2), 3);
  results.add_case(supl::clast(test3) == std::cbegin(test3), true);
  results.add_case(*supl::clast(test4), 4);

  return results;
}

static auto test_iota() -> ehanc::test
{
  ehanc::test results;

  constexpr static std::array<std::size_t, 10> expected1 {1, 2, 3, 4, 5,
                                                          6, 7, 8, 9, 10};
  constexpr static auto result1 {[]() {
    std::array<std::size_t, 10> retval {};
    for ( std::size_t i : supl::iota<std::size_t> {1, 10} ) {
      retval.at(i - 1) = i;
    }
    return retval;
  }()}; // IILE

  results.add_case(result1, expected1);

  std::array<std::size_t, 10> expected2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result2 {[]() {
    std::array<std::size_t, 10> retval {};
    std::copy(supl::iota<std::size_t>::iterator {1},
              supl::iota<std::size_t>::iterator {11}, retval.begin());
    return retval;
  }()}; // IILE

  results.add_case(result2, expected2);

  return results;
}

void test_iterators()
{
  ehanc::run_test("supl::last", &test_last);
  ehanc::run_test("supl::clast", &test_clast);
  ehanc::run_test("supl::iota", &test_iota);
}
