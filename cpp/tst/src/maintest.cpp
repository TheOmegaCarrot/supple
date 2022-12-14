#include <iostream>

#include "supl/term_colors.h"
#include "test_utils.hpp"

#include "supl/test_algorithm.h"
#include "supl/test_fake_ranges.h"
#include "supl/test_functional.h"
#include "supl/test_iterators.h"
#include "supl/test_tuple_algo.h"
#include "supl/test_utility.h"

auto main() -> int
{

  ehanc::test_section("Algorithm", &test_algorithm);

  ehanc::test_section("Tuple Algorithms", &test_tuple_algo);

  ehanc::test_section("Fake Ranges", &test_fake_ranges);

  ehanc::test_section("Iterators", &test_iterators);

  ehanc::test_section("Utility", &test_utility);

  ehanc::test_section("Functional", &test_functional);

  return 0;
}
