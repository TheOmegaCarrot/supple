#include <vector>

#include "test_utils.hpp"
#include "utils/test_fake_ranges.h"

static auto test_all_of() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 2};
  std::vector vec {2, 3, 4, 5, 2};

  results.add_case(ehanc::fr::all_of(vec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(ehanc::fr::all_of(cvec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(ehanc::fr::all_of(vec, [](auto&& i) { return i > 3; }),
                   false);
  results.add_case(ehanc::fr::all_of(cvec, [](auto&& i) { return i > 3; }),
                   false);

  return results;
}

static auto test_any_of() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 2};
  std::vector vec {2, 3, 4, 5, 2};

  results.add_case(ehanc::fr::any_of(vec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(ehanc::fr::any_of(cvec, [](auto&& i) { return i > 1; }),
                   true);
  results.add_case(ehanc::fr::any_of(vec, [](auto&& i) { return i > 9; }),
                   false);
  results.add_case(ehanc::fr::any_of(cvec, [](auto&& i) { return i > 9; }),
                   false);

  return results;
}

static auto test_none_of() -> ehanc::test
{
  ehanc::test results;

  const std::vector cvec {2, 3, 4, 5, 2};
  std::vector vec {2, 3, 4, 5, 2};

  results.add_case(ehanc::fr::none_of(vec, [](auto&& i) { return i < 1; }),
                   true);
  results.add_case(
      ehanc::fr::none_of(cvec, [](auto&& i) { return i < 1; }), true);
  results.add_case(ehanc::fr::none_of(vec, [](auto&& i) { return i < 9; }),
                   false);
  results.add_case(
      ehanc::fr::none_of(cvec, [](auto&& i) { return i < 9; }), false);

  return results;
}

void test_fake_ranges()
{
  ehanc::run_test("all_of", &test_all_of);
  ehanc::run_test("any_of", &test_any_of);
  ehanc::run_test("none_of", &test_none_of);
}
