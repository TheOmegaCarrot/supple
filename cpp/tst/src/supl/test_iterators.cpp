#include "supl/test_iterators.h"
#include "test_utils.hpp"

#include <array>
#include <forward_list>
#include <numeric>
#include <vector>

#include "supl/etc.hpp"

static auto test_last() -> supl::test
{
  supl::test results;
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

static auto test_clast() -> supl::test
{
  supl::test results;
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

static auto test_sequence_iterator() -> supl::test
{
  supl::test results;

  supl::sequence_iterator begin(0);
  supl::sequence_iterator end(10);

  std::for_each(begin, end, [&results, j {0}](const int i) mutable {
    results.add_case(i, j++);
  });

  supl::sequence_iterator begin2(10, supl::decrement<int>);
  supl::sequence_iterator end2(0);

  std::for_each(begin2, end2, [&results, j {10}](const int i) mutable {
    results.add_case(i, j--);
  });

  supl::sequence_iterator begin3(0, [j {0}](int& i) mutable { i = ++j; });
  supl::sequence_iterator end3(10);

  std::for_each(begin3, end3, [&results, j {0}](const int i) mutable {
    results.add_case(i, j++);
  });

  return results;
}

static auto test_sequence() -> supl::test
{
  supl::test results;

  int j {0};
  for ( int i : supl::sequence(0, 10) ) {
    results.add_case(i, j++);
  }

  j = 10;
  for ( int i : supl::sequence(10, 0, supl::decrement<int>) ) {
    results.add_case(i, j--);
  }

  supl::sequence seq {supl::sequence(0, 10)};
  results.add_case(seq.empty(), false, "Should not report empty");

  supl::sequence empty_sequence {supl::sequence(2, 2)};
  results.add_case(empty_sequence.empty(), true, "Should report empty");

  for ( int i : empty_sequence ) {
    results.add_case(
        false, true,
        "Iteration over empty sequence should not happen\nValue: "
            + std::to_string(i));
  }

  return results;
}

static auto test_generative_iterator() -> supl::test
{
  supl::test results;

  supl::generative_iterator begin([j {0}]() mutable { return j++; });
  supl::generative_iterator<decltype(begin)::value_type> end(10);

  std::for_each(begin, end, [&results, j {0}](const int i) mutable {
    results.add_case(i, j++);
  });

  supl::generative_iterator begin2([j {10}]() mutable { return j--; });
  supl::generative_iterator end2(begin2, 0);

  std::for_each(begin2, end2, [&results, j {10}](const int i) mutable {
    results.add_case(i, j--);
  });

  return results;
}

static auto test_generative_sequence() -> supl::test
{
  supl::test results;

  int k {0};
  for ( int i : supl::generative_sequence(
            5, [j {0}]() mutable { return j++; }) ) {
    results.add_case(i, k++);
  }

  return results;
}

void test_iterators()
{
  supl::run_test("supl::last", &test_last);
  supl::run_test("supl::clast", &test_clast);
  supl::run_test("supl::sequence_iterator", &test_sequence_iterator);
  supl::run_test("supl::sequence", &test_sequence);
  supl::run_test("supl::generative_iterator", &test_generative_iterator);
  supl::run_test("supl::generative_sequence", &test_generative_sequence);
}
