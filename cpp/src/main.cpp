#include <iostream>
#include <list>
#include <utility>

#include "supl/all.h"
#include "supl/metaprogramming.hpp"

/// @cond

// NOLINTBEGIN

template <typename... Ts>
auto peeler(Ts...)
{
  return supl::peel_first_t<Ts...>();
}

template <typename... Ts>
auto uniform(Ts...) -> bool
{
  return supl::is_pack_uniform_v<Ts...>;
}

auto main([[maybe_unused]] const int argc,
          [[maybe_unused]] const char* const* const argv) -> int
{

  /* std::cout << std::boolalpha */
  /*           << std::is_same_v<decltype(peeler()), void> << '\n' */
  /*           << uniform() << '\n' */
  /*           << '\n'; */

  std::cout << supl::to_string(
      std::tuple<int, char, double> {12, 'q', 15.932})
            << '\n';
  std::cout << supl::to_string(std::pair<int, char> {3, 'c'}) << '\n';
  std::cout << supl::to_string(std::vector<int> {1, 2, 5, 8}) << '\n';
  std::cout << supl::to_string(std::list<char> {'a', 'p', 'p', 'l', 'e'})
            << '\n';
  std::cout << supl::to_string(supl::sequence(1, 10)) << '\n';
  std::cout << supl::to_string(0) << '\n';

  std::tuple example {true, 3.14, 42, 'p'};
  std::string what {"what"};
  std::cout << supl::to_string(supl::tuple_push_back(example, what))
            << '\n';
  std::cout << supl::to_string(supl::tuple_pop_back(example)) << '\n';
  std::cout << supl::to_string(supl::tuple_push_front(example, what))
            << '\n';
  std::cout << supl::to_string(supl::tuple_pop_front(example)) << '\n';
  std::cout << supl::to_string(
      supl::tuple_insert(example, supl::index_constant<2> {}, what))
            << '\n';

  return 0;
}

// NOLINTEND
