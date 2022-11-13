#ifndef UTILS_FAKE_RANGES_HPP
#define UTILS_FAKE_RANGES_HPP

#include "algorithm.hpp"
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <utility>

namespace ehanc::fr {

template <typename Container, typename Pred>
auto all_of(Container&& container, Pred&& pred) -> bool
{
  auto begin {std::begin(container)};
  auto end {std::end(container)};

  return std::all_of(begin, end, std::forward<Pred>(pred));
}

template <typename Container, typename Pred>
auto any_of(Container&& container, Pred&& pred) -> bool
{
  auto begin {std::begin(container)};
  auto end {std::end(container)};

  return std::any_of(begin, end, std::forward<Pred>(pred));
}

template <typename Container, typename Pred>
auto none_of(Container&& container, Pred&& pred) -> bool
{
  auto begin {std::begin(container)};
  auto end {std::end(container)};

  return std::none_of(begin, end, std::forward<Pred>(pred));
}

} // namespace ehanc::fr

#endif
