#include <type_traits>

#include "supl/integer_sequence.hpp"

///////////////////////////////////////////// integer_sequence

static_assert(std::is_same_v<supl::integer_sequence<int, 1, 2, 3>,
                             decltype(supl::integer_sequence {
                               std::integer_sequence<int, 1, 2, 3> {}})>);

///////////////////////////////////////////// binary operations

static_assert(
  std::is_same_v<supl::integer_sequence<int, 5, 7, 9>,
                 decltype(supl::integer_sequence<short, 1, 2, 3> {}
                          + supl::integer_sequence<short, 4, 5, 6> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 3, 2, 1>,
                 decltype(supl::integer_sequence<int, 5, 4, 3> {}
                          - supl::integer_sequence<int, 2, 2, 2> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 10, 8, 6>,
                 decltype(supl::integer_sequence<int, 5, 4, 3> {}
                          * supl::integer_sequence<int, 2, 2, 2> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 2, 2, 1>,
                 decltype(supl::integer_sequence<int, 5, 4, 3> {}
                          / supl::integer_sequence<int, 2, 2, 2> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 0, 2, 5>,
                 decltype(supl::integer_sequence<int, 8, 6, 15> {}
                          % supl::integer_sequence<int, 2, 4, 10> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 4 & 3, 7 & 2, 9 & 5>,
                 decltype(supl::integer_sequence<int, 4, 7, 9> {}
                          & supl::integer_sequence<int, 3, 2, 5> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 4 | 3, 7 | 2, 9 | 5>,
                 decltype(supl::integer_sequence<int, 4, 7, 9> {}
                          | supl::integer_sequence<int, 3, 2, 5> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, 4 ^ 3, 7 ^ 2, 9 ^ 5>,
                 decltype(supl::integer_sequence<int, 4, 7, 9> {}
                          ^ supl::integer_sequence<int, 3, 2, 5> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<bool, true, true, false>,
                 decltype(supl::integer_sequence<int, 9, 7, 2> {}
                          > supl::integer_sequence<int, 5, 3, 6> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<bool, false, false, true>,
                 decltype(supl::integer_sequence<int, 9, 7, 2> {}
                          < supl::integer_sequence<int, 5, 3, 6> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<bool, true, true, false>,
                 decltype(supl::integer_sequence<int, 9, 7, 2> {}
                          >= supl::integer_sequence<int, 5, 7, 6> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<bool, false, true, true>,
                 decltype(supl::integer_sequence<int, 9, 7, 2> {}
                          <= supl::integer_sequence<int, 5, 7, 6> {})>);

static_assert(std::is_same_v<
              supl::integer_sequence<bool, true, false, true, false>,
              decltype(supl::integer_sequence<int, 42, 6, 1, 98> {}
                       == supl::integer_sequence<int, 42, 9, 1, 23> {})>);

static_assert(std::is_same_v<
              supl::integer_sequence<bool, false, true, false, true>,
              decltype(supl::integer_sequence<int, 42, 6, 1, 98> {}
                       != supl::integer_sequence<int, 42, 9, 1, 23> {})>);

///////////////////////////////////////////// unary operations

static_assert(
  std::is_same_v<supl::integer_sequence<int, 3, 8, 2>,
                 decltype(+supl::integer_sequence<char, 3, 8, 2> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, -3, -8, -2>,
                 decltype(-supl::integer_sequence<int, 3, 8, 2> {})>);

static_assert(
  std::is_same_v<
    supl::integer_sequence<bool, true, false, true, false>,
    decltype(! supl::
               integer_sequence<bool, false, true, false, true> {})>);

static_assert(
  std::is_same_v<supl::integer_sequence<int, ~3, ~8, ~2>,
                 decltype(~supl::integer_sequence<char, 3, 8, 2> {})>);

int main()
{ }
