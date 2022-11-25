#ifndef UTILS_TUPLE_ALGO_HPP
#define UTILS_TUPLE_ALGO_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"

namespace ehanc {

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Not
 * intended to be called outside ::ehanc::for_each_in_tuple.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Function type.
 *
 * @tparam Inds Sequence of integers for template wizardry.
 *
 * @param tup Tuple to apply function to a member of.
 *
 * @param func Function to apply to a member of tuple.
 */
/* }}} */
template <typename Tuple, typename Func, std::size_t... Inds>
constexpr void
for_each_in_tuple_impl(Tuple&& tup, Func&& func,
                       std::index_sequence<Inds...>) noexcept
{
  (func(std::get<Inds>(tup)), ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Would not
 * have been possible without what I learned from this CppCon talk:
 * https://www.youtube.com/watch?v=15etE6WcvBY
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Visitor function type.
 *
 * @param tup Tuple to apply a visitor function to every element of.
 *
 * @param func Visitor function.
 */
/* }}} */
template <typename Tuple, typename Func>
constexpr void for_each_in_tuple(const Tuple& tup, Func&& func) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  ::ehanc::impl::for_each_in_tuple_impl(tup, std::forward<Func>(func),
                                        seq);
}

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside ::ehanc::for_each_in_tuple.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Function type.
 *
 * @tparam Inds Sequence of integers for template wizardry.
 *
 * @param tup Tuple to apply function to a member of.
 *
 * @param func Function to apply to a member of tuple.
 *
 * @return Tuple of values mapped from `tup` through `func`.
 */
/* }}} */
template <typename Tuple, typename Func, std::size_t... Inds>
constexpr auto tuple_transform_impl(const Tuple& tup, Func&& func,
                                    std::index_sequence<Inds...>) noexcept
{
  return std::tuple {func(std::get<Inds>(tup))...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside ::ehanc::for_each_in_tuple.
 * Would not have been possible without what I learned from this CppCon talk:
 * https://www.youtube.com/watch?v=15etE6WcvBY
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Visitor function type.
 *
 * @param tup Tuple to apply a visitor function to every element of.
 *
 * @param func Visitor function.
 *
 * @return Tuple of values mapped from `tup` through `func`.
 */
/* }}} */
template <typename Tuple, typename Func>
[[nodiscard]] constexpr auto tuple_transform(const Tuple& tup,
                                             Func&& func) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::ehanc::impl::tuple_transform_impl(tup, std::forward<Func>(func),
                                             seq);
}

namespace impl {

template <typename Tuple, typename T, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_push_back_impl(const Tuple& tup, T&& data,
                     std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::get<Inds>(tup)..., std::forward<T>(data)};
}

} // namespace impl

template <typename Tuple, typename T>
[[nodiscard]] constexpr auto tuple_push_back(const Tuple& tup,
                                             T&& data) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::ehanc::impl::tuple_push_back_impl(tup, std::forward<T>(data),
                                             seq);
}

namespace impl {

template <typename Tuple, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_pop_back_impl(const Tuple& tup,
                    std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::get<Inds>(tup)...};
}

} // namespace impl

template <typename Tuple>
[[nodiscard]] constexpr auto tuple_pop_back(const Tuple& tup) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return ::ehanc::impl::tuple_pop_back_impl(tup, seq);
}

namespace impl {

template <typename Tuple, typename T, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_push_front_impl(const Tuple& tup, T&& data,
                      std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::forward<T>(data), std::get<Inds>(tup)...};
}

} // namespace impl

template <typename Tuple, typename T>
[[nodiscard]] constexpr auto tuple_push_front(const Tuple& tup,
                                              T&& data) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::ehanc::impl::tuple_push_front_impl(tup, std::forward<T>(data),
                                              seq);
}

namespace impl {

template <typename Tuple, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_pop_front_impl(const Tuple& tup,
                     std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::get<Inds + 1>(tup)...};
}

} // namespace impl

template <typename Tuple>
[[nodiscard]] constexpr auto tuple_pop_front(const Tuple& tup) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return ::ehanc::impl::tuple_pop_front_impl(tup, seq);
}

namespace impl {

template <typename Tuple, typename T, std::size_t... pre_idxs,
          std::size_t... post_idxs>
[[nodiscard]] constexpr auto
tuple_insert_impl(const Tuple& tup, T&& data,
                  std::index_sequence<pre_idxs...>,

                  std::index_sequence<post_idxs...>)
{
  constexpr std::size_t idx {sizeof...(pre_idxs)};

  return std::tuple {std::get<pre_idxs>(tup)..., std::forward<T>(data),
                     std::get<post_idxs + idx>(tup)...};
}

} // namespace impl

template <typename Tuple, typename T, std::size_t Idx>
[[nodiscard]] constexpr auto
tuple_insert(const Tuple& tup, ehanc::index_constant<Idx>, T&& data)
{
  static_assert(Idx <= std::tuple_size_v<Tuple>, "Index out of bounds");

  auto pre_seq {std::make_index_sequence<Idx> {}};
  auto post_seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - Idx> {}};

  return ::ehanc::impl::tuple_insert_impl(tup, std::forward<T>(data),
                                          pre_seq, post_seq);
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
constexpr auto tuple_count_if_impl(const Tuple& tup, Pred&& pred,
                                   std::index_sequence<Inds...>) noexcept
    -> std::size_t
{
  return (static_cast<std::size_t>(pred(std::get<Inds>(tup))) + ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a generic predicate to every element of a tuple,
 * returning the number of elements for which that predicate is true.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Pred Predicate type
 *
 * @param tup Tuple to apply predicate to elements of.
 *
 * @param pred Generic predicate to apply.
 *
 * @return Number of elements of `tup` such that `pred(tup)` returns `true`.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_count_if(const Tuple& tup,
                                            Pred&& pred) noexcept
    -> std::size_t
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::ehanc::impl::tuple_count_if_impl(tup, std::forward<Pred>(pred),
                                            seq);
}

} // namespace ehanc

#endif
