#include "supl/test_iterators.h"
#include "test_utils.hpp"

#include <array>
#include <forward_list>
#include <iterator>
#include <numeric>
#include <vector>

#include "supl/iterators.hpp"
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

void test_iterators()
{
  ehanc::run_test("supl::last", &test_last);
  ehanc::run_test("supl::clast", &test_clast);
}

static_assert(supl::is_iterator_tag_v<std::forward_iterator_tag>);
static_assert(supl::is_iterator_tag_v<std::bidirectional_iterator_tag>);
static_assert(supl::is_iterator_tag_v<std::random_access_iterator_tag>);
static_assert(not supl::is_iterator_tag_v<int>);
static_assert(
    not supl::is_iterator_tag_v<supl::stream_adapter<std::vector<int>>>);
