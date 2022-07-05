#include "test_iterators.h"
#include "test_utils.hpp"

#include <array>
#include <numeric>
#include <vector>

auto test_forward_distance() -> ehanc::test
{
  ehanc::test results;
  std::array<int, 5> test{1, 2, 3, 4, 5};

  results.add_case(ehanc::forward_distance(test.cbegin(), test.cend()),
                   size_t{5});

  return results;
}

auto test_last() -> ehanc::test
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

auto test_clast() -> ehanc::test
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

auto test_sequence_iterator() -> ehanc::test
{
  ehanc::test results;

  ehanc::sequence_iterator begin(0);
  ehanc::sequence_iterator end(10);

  std::for_each(begin, end, [&results, j{0}](const int i) mutable {
    results.add_case(i, j++);
  });

  ehanc::sequence_iterator begin2(10, ehanc::decrement<int>);
  ehanc::sequence_iterator end2(0);

  std::for_each(begin2, end2, [&results, j{10}](const int i) mutable {
    results.add_case(i, j--);
  });

  ehanc::sequence_iterator begin3(0, [j{0}](int& i) mutable { i = ++j; });
  ehanc::sequence_iterator end3(10);

  std::for_each(begin3, end3, [&results, j{0}](const int i) mutable {
    results.add_case(i, j++);
  });

  return results;
}

auto test_sequence() -> ehanc::test
{
  ehanc::test results;

  int j{0};
  for ( int i : ehanc::sequence(0, 10) ) {
    results.add_case(i, j++);
  }

  j = 10;
  for ( int i : ehanc::sequence(10, 0, ehanc::decrement<int>) ) {
    results.add_case(i, j--);
  }

  return results;
}

auto test_generative_iterator() -> ehanc::test
{
  ehanc::test results;

  ehanc::generative_iterator begin([j{0}]() mutable { return j++; });
  ehanc::generative_iterator<decltype(begin)::value_type> end(10);

  std::for_each(begin, end, [&results, j{0}](const int i) mutable {
    results.add_case(i, j++);
  });

  ehanc::generative_iterator begin2([j{10}]() mutable { return j--; });
  ehanc::generative_iterator end2(begin2, 0);

  std::for_each(begin2, end2, [&results, j{10}](const int i) mutable {
    results.add_case(i, j--);
  });

  return results;
}

auto test_generative_sequence() -> ehanc::test
{
  ehanc::test results;

  int k{0};
  for ( int i :
        ehanc::generative_sequence(5, [j{0}]() mutable { return j++; }) ) {
    results.add_case(i, k++);
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
  ehanc::run_test("ehanc::generative_iterator", &test_generative_iterator);
  ehanc::run_test("ehanc::generative_sequence", &test_generative_sequence);
}
