#include <supl/test_results.hpp>

auto main() -> int
{
  supl::test_results results;

  const std::string a_string {"reorder"};
  const std::vector a_vector {1, 2, 3, 4};

  const std::
    tuple<int, const std::vector<int>&, const std::string&, char, bool>
      test_input {42, a_vector, a_string, 'j', true};

  const std::tuple<const std::vector<int>&,
                   bool,
                   char,
                   const std::string&,
                   const std::vector<int>&,
                   int>
    expected {a_vector, true, 'j', a_string, a_vector, 42};

  const auto result {supl::tuple::reorder<1, 4, 3, 2, 1, 0>(test_input)};

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}

