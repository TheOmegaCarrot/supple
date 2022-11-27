#include <iostream>

#include "supl/term_colors.h"
#include "test_utils.hpp"

#include "supl/test_algorithm.h"
#include "supl/test_etc.h"
#include "supl/test_iterators.h"
#include "supl/test_metaprogramming.h"
#include "supl/test_tuple_algo.h"

auto main() -> int
{

  supl::test_section("Algorithm", &test_algorithm);

  supl::test_section("Tuple Algorithms", &test_tuple_algo);

  supl::test_section("Iterators", &test_iterators);

  supl::test_section("Metaprogramming", &test_metaprogramming);

  supl::test_section("Etc", &test_etc);

  return 0;
}
