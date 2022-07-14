#include <iostream>

#include "test_utils.hpp"
#include "utils/term_colors.h"

#include "test_algorithm.h"
#include "test_etc.h"
#include "test_iterators.h"
#include "test_lazy.h"
#include "test_metaprogramming.h"

auto main() -> int
{

  ehanc::test_section("Algorithm", &test_algorithm);

  ehanc::test_section("Iterators", &test_iterators);

  ehanc::test_section("Lazy", &test_lazy);

  ehanc::test_section("Metaprogramming", &test_metaprogramming);

  ehanc::test_section("Etc", &test_etc);

  return 0;
}
