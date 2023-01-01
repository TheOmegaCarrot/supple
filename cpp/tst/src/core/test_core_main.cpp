#include <iostream>

#include <supl/test_runner.hpp>

#include "supl/test_algorithm.h"
#include "supl/test_crtp.h"
#include "supl/test_fake_ranges.h"
#include "supl/test_functional.h"
#include "supl/test_iterators.h"
#include "supl/test_tuple_algo.h"
#include "supl/test_utility.h"

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_algorithm());

  runner.add_section(test_tuple_algo());

  runner.add_section(test_fake_ranges());

  runner.add_section(test_iterators());

  runner.add_section(test_utility());

  runner.add_section(test_functional());

  runner.add_section(test_crtp());

  return runner.run();
}
