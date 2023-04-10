#include "supl/predicates.hpp"
#include "supl/test_results.hpp"

auto main() -> int
{
  supl::test_results results;

  const auto is_lower_vowel {supl::equals_any_of('a', 'e', 'i', 'o', 'u')};

  results.enforce_true(is_lower_vowel('a'), "a");
  results.enforce_false(is_lower_vowel('b'), "b");
  results.enforce_false(is_lower_vowel('c'), "c");
  results.enforce_false(is_lower_vowel('d'), "d");
  results.enforce_true(is_lower_vowel('e'), "e");
  results.enforce_false(is_lower_vowel('f'), "f");
  results.enforce_false(is_lower_vowel('g'), "g");
  results.enforce_false(is_lower_vowel('h'), "h");
  results.enforce_true(is_lower_vowel('i'), "i");
  results.enforce_false(is_lower_vowel('j'), "j");
  results.enforce_false(is_lower_vowel('k'), "k");
  results.enforce_false(is_lower_vowel('l'), "l");
  results.enforce_false(is_lower_vowel('m'), "m");
  results.enforce_false(is_lower_vowel('n'), "n");
  results.enforce_true(is_lower_vowel('o'), "o");
  results.enforce_false(is_lower_vowel('p'), "p");
  results.enforce_false(is_lower_vowel('q'), "q");
  results.enforce_false(is_lower_vowel('r'), "r");
  results.enforce_false(is_lower_vowel('s'), "s");
  results.enforce_false(is_lower_vowel('t'), "t");
  results.enforce_true(is_lower_vowel('u'), "u");
  results.enforce_false(is_lower_vowel('v'), "v");
  results.enforce_false(is_lower_vowel('w'), "w");
  results.enforce_false(is_lower_vowel('x'), "x");
  results.enforce_false(is_lower_vowel('y'), "y");
  results.enforce_false(is_lower_vowel('z'), "z");

  return results.print_and_return();
}
