#include "test_iterators.h"
#include "test_utils.hpp"

#include <array>
#include <numeric>
#include <vector>

ehanc::test test_forward_distance()
{
  ehanc::test results;
  std::array<int, 5> test{1, 2, 3, 4, 5};

  results.add_case(ehanc::forward_distance(test.cbegin(), test.cend()),
                   size_t{5});

  return results;
}

ehanc::test test_last()
{
  ehanc::test results;
  std::array<int, 5> test1{};
  std::iota(test1.begin(), test1.end(), 1);

  std::array<int, 2> test2{2, 3};
  std::vector<int> test3;

  results.add_case(*ehanc::last(test1), 5);
  results.add_case(*ehanc::last(test2), 3);
  results.add_case(ehanc::last(test3) == std::begin(test3), true);

  return results;
}

ehanc::test test_clast()
{
  ehanc::test results;
  std::array<int, 5> test1{};
  std::iota(test1.begin(), test1.end(), 1);

  std::array<int, 2> test2{2, 3};
  std::vector<int> test3;

  results.add_case(*ehanc::clast(test1), 5);
  results.add_case(*ehanc::clast(test2), 3);
  results.add_case(ehanc::clast(test3) == std::cbegin(test3), true);

  return results;
}

ehanc::test test_sequence_iterator()
{
  ehanc::test results;

  ehanc::sequence_iterator begin{0};
  ehanc::sequence_iterator end{10};

  std::for_each(begin, end, [&results, j{0}](const int i) mutable {
    results.add_case(i, j++);
  });

  return results;
}

ehanc::test test_sequence()
{
  ehanc::test results;

  int j{0};
  for ( int i : ehanc::sequence(0, 10) ) {
    results.add_case(i, j++);
  }

  return results;
}

void test_iterators()
{
  ehanc::run_test("ehanc::forward_distance", &test_forward_distance);
  ehanc::run_test("ehanc::last", &test_last);
  ehanc::run_test("ehanc::clast", &test_clast);
  ehanc::run_test("ehanc::sequence_iterator", &test_sequence_iterator);
  ehanc::run_test("ehanc::sequence", &test_sequence);
}