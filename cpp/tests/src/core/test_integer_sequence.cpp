#include <type_traits>

#include "supl/integer_sequence.hpp"

///////////////////////////////////////////// integer_sequence

static_assert(std::is_same_v<supl::integer_sequence<int, 1, 2, 3>,
                             decltype(supl::integer_sequence {
                               std::integer_sequence<int, 1, 2, 3> {}})>);

///////////////////////////////////////////// operator +

static_assert(
  std::is_same_v<supl::integer_sequence<int, 5, 7, 9>,
                 decltype(supl::integer_sequence<short, 1, 2, 3> {}
                          + supl::integer_sequence<short, 4, 5, 6> {})>);

int main()
{ }
