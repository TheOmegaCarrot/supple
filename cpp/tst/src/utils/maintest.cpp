#include <iostream>

#include "test_utils.hpp"
#include "utils/term_colors.h"

#include "utils/test_algorithm.h"
#include "utils/test_etc.h"
#include "utils/test_iterators.h"
#include "utils/test_metaprogramming.h"
#include "utils/test_tuple_algo.h"
#include "utils/test_type_list.h"

auto main() -> int
{

  ehanc::test_section("Algorithm", &test_algorithm);

  ehanc::test_section("Tuple Algorithms", &test_tuple_algo);

  ehanc::test_section("Iterators", &test_iterators);

  ehanc::test_section("Metaprogramming", &test_metaprogramming);

  ehanc::test_section("Type List", &test_type_list);

  ehanc::test_section("Etc", &test_etc);

  return 0;
}
