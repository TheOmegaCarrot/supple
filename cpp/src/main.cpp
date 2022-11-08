#include <iostream>
#include <list>
#include <utility>

#include "utils/all.h"

/// @cond

// NOLINTBEGIN

template <typename... Ts>
auto peeler(Ts...)
{
  return ehanc::peel_first_t<Ts...>();
}

template <typename... Ts>
auto uniform(Ts...) -> bool
{
  return ehanc::is_pack_uniform_v<Ts...>;
}

auto main([[maybe_unused]] const int argc,
          [[maybe_unused]] const char* const* const argv) -> int
{

  /* std::cout << std::boolalpha */
  /*           << std::is_same_v<decltype(peeler()), void> << '\n' */
  /*           << uniform() << '\n' */
  /*           << '\n'; */

  std::cout << ehanc::to_string(
      std::tuple<int, char, double> {12, 'q', 15.932})
            << '\n';
  std::cout << ehanc::to_string(std::pair<int, char> {3, 'c'}) << '\n';
  std::cout << ehanc::to_string(std::vector<int> {1, 2, 5, 8}) << '\n';
  std::cout << ehanc::to_string(std::list<char> {'a', 'p', 'p', 'l', 'e'})
            << '\n';
  std::cout << ehanc::to_string(ehanc::sequence(1, 10)) << '\n';
  std::cout << ehanc::to_string(0) << '\n';

  return 0;
}

// NOLINTEND
