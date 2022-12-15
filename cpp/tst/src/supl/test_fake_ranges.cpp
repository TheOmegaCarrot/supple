#include <vector>

#include "supl/test_fake_ranges.h"
#include "test_utils.hpp"

static auto test_all_of() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 2};
  std::vector vec {2, 3, 4, 5, 2};

  results.add_case(supl::fr::all_of(vec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(supl::fr::all_of(cvec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(supl::fr::all_of(vec, [](auto&& i) { return i > 3; }),
                   false);
  results.add_case(supl::fr::all_of(cvec, [](auto&& i) { return i > 3; }),
                   false);

  return results;
}

static auto test_any_of() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 2};
  std::vector vec {2, 3, 4, 5, 2};

  results.add_case(supl::fr::any_of(vec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(supl::fr::any_of(cvec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(supl::fr::any_of(vec, [](auto&& i) { return i > 9; }),
                   false);
  results.add_case(supl::fr::any_of(cvec, [](auto&& i) { return i > 9; }),
                   false);

  return results;
}

static auto test_none_of() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 2};
  std::vector vec {2, 3, 4, 5, 2};

  results.add_case(supl::fr::none_of(vec, [](auto&& i) { return i < 1; }),
                   true);
  results.add_case(supl::fr::none_of(cvec, [](auto&& i) { return i < 1; }),
                   true);
  results.add_case(supl::fr::none_of(vec, [](auto&& i) { return i < 9; }),
                   false);
  results.add_case(supl::fr::none_of(cvec, [](auto&& i) { return i < 9; }),
                   false);

  return results;
}

static auto test_count() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 5};

  auto count {supl::fr::count(cvec, 5)};
  auto correct {static_cast<decltype(count)>(2)};

  results.add_case(count, correct);

  return results;
}

static auto test_iota() -> ehanc::test
{
  ehanc::test results;

  constexpr static std::array<std::size_t, 10> expected1 {1, 2, 3, 4, 5,
                                                          6, 7, 8, 9, 10};
  constexpr static auto result1 {[]() {
    std::array<std::size_t, 10> retval {};
    for ( std::size_t i : supl::fr::iota<std::size_t> {1, 11} ) {
      retval.at(i - 1) = i;
    }
    return retval;
  }()}; // IILE

  results.add_case(result1, expected1);

  std::array<std::size_t, 10> expected2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result2 {[]() {
    std::array<std::size_t, 10> retval {};
    std::copy(supl::fr::iota<std::size_t>::iterator {1},
              supl::fr::iota<std::size_t>::iterator {11}, retval.begin());
    return retval;
  }()}; // IILE

  results.add_case(result2, expected2);

  return results;
}

void test_fake_ranges()
{
  ehanc::run_test("supl::fr::all_of", &test_all_of);
  ehanc::run_test("supl::fr::any_of", &test_any_of);
  ehanc::run_test("supl::fr::none_of", &test_none_of);
  /* ehanc::run_test("supl::fr::for_each", &test_for_each); */
  /* ehanc::run_test("supl::fr::for_each_n", &test_for_each_n); */
  ehanc::run_test("supl::fr::count", &test_count);
  /* ehanc::run_test("supl::fr::count_if", &test_count_if); */
  ehanc::run_test("supl::fr::iota", &test_iota);
}
