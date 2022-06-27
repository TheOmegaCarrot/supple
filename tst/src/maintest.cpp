#include <iostream>

#include "test_utils.hpp"
#include "utils/term_colors.h"

#include "test_algorithm.h"
#include "test_etc.h"
#include "test_lazy.h"

int main()
{

  ehanc::test_section("Algorithm", &test_algorithm);

  ehanc::test_section("Lazy", &test_lazy);

  ehanc::test_section("Etc", &test_etc);

  return 0;
}
