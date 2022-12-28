#include "supl/test_iterators.h"
#include "test_utils.hpp"

#include <array>
#include <deque>
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

static auto test_iterator() -> ehanc::test
{
  ehanc::test results;

  std::vector test_iterable_1 {1, 2, 3, 4, 5};
  std::deque test_iterable_2 {6, 7, 8, 9, 10};

  // increment and decrement
  supl::experimental::iterator test_1 {test_iterable_1.begin()};
  results.add_case(*test_1, 1);
  ++test_1;
  results.add_case(*test_1, 2);
  --test_1;
  results.add_case(*test_1, 1);

  supl::experimental::iterator test_2 {test_1++};
  results.add_case(*test_1, 2);
  results.add_case(*test_2, 1);

  ++test_2;
  supl::experimental::iterator test_3 {test_2--};
  results.add_case(*test_2, 1);
  results.add_case(*test_3, 2);

  test_1 = test_iterable_2.begin();
  results.add_case(*test_1, 6);
  ++test_1;

  supl::experimental::iterator test_4 {test_1};
  results.add_case(*test_1, 7);
  results.add_case(*test_4, 7);

  // equality
  supl::experimental::iterator test_a {test_iterable_1.begin()};
  supl::experimental::iterator test_b {test_iterable_1.begin()};

  // same type - good cast
  results.add_case(test_a == test_b, true);
  results.add_case(test_a != test_b, false);
  ++test_b;

  // different type - bad cast
  test_b = test_iterable_2.begin();
  results.add_case(test_a == test_b, false);
  results.add_case(test_a != test_b, true);

  return results;
}

void test_iterators()
{
  ehanc::run_test("supl::last", &test_last);
  ehanc::run_test("supl::clast", &test_clast);
  ehanc::run_test("supl::iterator", &test_iterator);
}

static_assert(supl::is_iterator_tag_v<std::forward_iterator_tag>);
static_assert(supl::is_iterator_tag_v<std::bidirectional_iterator_tag>);
static_assert(supl::is_iterator_tag_v<std::random_access_iterator_tag>);
static_assert(not supl::is_iterator_tag_v<int>);
static_assert(
    not supl::is_iterator_tag_v<supl::stream_adapter<std::vector<int>>>);
