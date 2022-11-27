#ifndef SUPPLEMENTARIES_FAKE_RANGES_HPP
#define SUPPLEMENTARIES_FAKE_RANGES_HPP

// Still a work in progress
// The goal is to have every STL algorithm and `supl` algorithm
// have a `fr` counterpart

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <utility>

#include "algorithm.hpp"

namespace supl::fr {

template <typename Container, typename Pred>
auto all_of(Container&& container, Pred&& pred) noexcept(
    noexcept(std::all_of(std::begin(container), std::end(container),
                         std::forward<Pred>(pred)))) -> bool
{
  return std::all_of(std::begin(container), std::end(container),
                     std::forward<Pred>(pred));
}

template <typename Container, typename Pred>
auto any_of(Container&& container, Pred&& pred) noexcept(
    noexcept(std::any_of(std::begin(container), std::end(container),
                         std::forward<Pred>(pred)))) -> bool
{
  return std::any_of(std::begin(container), std::end(container),
                     std::forward<Pred>(pred));
}

template <typename Container, typename Pred>
auto none_of(Container&& container, Pred&& pred) noexcept(
    noexcept(std::none_of(std::begin(container), std::end(container),
                          std::forward<Pred>(pred)))) -> bool
{
  return std::none_of(std::begin(container), std::end(container),
                      std::forward<Pred>(pred));
}

template <typename Container, typename Func>
auto for_each(Container&& container, Func&& func) noexcept(
    noexcept(std::for_each(std::begin(container), std::end(container),
                           std::forward<Func>(func)))) -> Func
{
  return std::for_each(std::begin(container), std::end(container),
                       std::forward<Func>(func));
}

template <typename Container, typename Size, typename Func>
auto for_each_n(Container&& container, Size n, Func&& func) noexcept(
    noexcept(std::for_each_n(std::begin(container), std::end(container), n,
                             std::forward<Func>(func)))) -> Func
{
  return std::for_each_n(std::begin(container), std::end(container), n,
                         std::forward<Func>(func));
}

template <typename Container, typename T>
auto count(Container&& container, const T& value) noexcept(noexcept(
    std::count(std::begin(container), std::end(container), value))) ->
    typename std::iterator_traits<
        decltype(std::begin(container))>::difference_type
{
  return std::count(std::begin(container), std::end(container), value);
}

template <typename Container, typename Pred>
auto count_if(Container&& container, Pred&& pred) noexcept(
    noexcept(std::count(std::begin(container), std::end(container), pred)))
    -> typename std::iterator_traits<
        decltype(std::begin(container))>::difference_type
{
  return std::count_if(std::begin(container), std::end(container),
                       std::forward<Pred>(pred));
}

} // namespace supl::fr

#endif
