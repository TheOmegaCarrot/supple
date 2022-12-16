/* {{{ doc */
/**
 * @file tuple_algo.hpp
 *
 * @brief Tuple operations
 *
 * @details This header contains functions for operations on tuples.
 * It is worth noting that the algorithms which both take and return a tuple
 * do copy elements from the input. Therefore it is not recommended to use
 * them for tuples which contain elements which are expensive to copy.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLEMENTARIES_TUPLE_ALGO_HPP
#define SUPPLEMENTARIES_TUPLE_ALGO_HPP

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"
#include "type_list.hpp"

namespace supl {

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Not
 * intended to be called outside for_each_in_tuple.
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
  (std::invoke(func, std::get<Inds>(tup)), ...);
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
 *
 * @pre `Func` must be invocable with every element of `tup`.
 * Failure to meet this precondition is a compile-time error.
 */
/* }}} */
template <typename Tuple, typename Func>
constexpr void for_each_in_tuple(const Tuple& tup, Func&& func) noexcept
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  impl::for_each_in_tuple_impl(tup, std::forward<Func>(func), seq);
}

namespace impl {

template <typename Tuple, typename Func, std::size_t... Idxs,
          std::size_t Begin>
constexpr void for_each_in_subtuple_impl(const Tuple& tup, Func&& func,
                                         std::index_sequence<Idxs...>,
                                         index_constant<Begin>)
{
  (std::invoke(func, std::get<Idxs + Begin>(tup)), ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a subtuple.
 * Function is applied to all elements in the half-open range [Begin, End).
 *
 * @tparam Begin Begin of half-open range of inputs
 *
 * @tparam End End of half-open range of inputs
 *
 * @tparam Tuple Input tuple type
 *
 * @param func Visitor function
 *
 * @pre `Func` must be invocable with every element of `tup`
 * in the half-open range [Begin, End).
 * Failure to meet this precondition is a compile-time error.
 */
/* }}} */
template <std::size_t Begin, std::size_t End, typename Tuple,
          typename Func>
constexpr void for_each_in_subtuple(const Tuple& tup, Func&& func)
{

  static_assert(Begin < std::tuple_size_v<Tuple>, "Begin out of bounds");
  static_assert(End <= std::tuple_size_v<Tuple>, "Begin out of bounds");

  constexpr auto seq {std::make_index_sequence<End - Begin> {}};

  impl::for_each_in_subtuple_impl(tup, std::forward<Func>(func), seq,
                                  index_constant<Begin> {});
}

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside tuple_transform
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
  return std::tuple {std::invoke(func, std::get<Inds>(tup))...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside for_each_in_tuple.
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
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return impl::tuple_transform_impl(tup, std::forward<Func>(func), seq);
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_any_of_impl(const Tuple& tup, Pred&& pred,
                  std::index_sequence<Inds...>) noexcept -> bool
{
  static_assert(std::conjunction_v<std::is_invocable_r<
                    bool, Pred, decltype(std::get<Inds>(tup))>...>,
                "Predicate must be invocable returning a bool with every "
                "type in the tuple");

  return (std::invoke(pred, std::get<Inds>(tup)) || ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Determine if any elements of a tuple satisfy a unary predicate
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Pred Type of unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @param tup Input tuple
 *
 * @param pred Unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @return Returns `true` if `pred` applied to any element of `tup`
 * returns true. Returns `false` otherwise.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_any_of(const Tuple& tup,
                                          Pred&& pred) noexcept -> bool
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return impl::tuple_any_of_impl(tup, std::forward<Pred>(pred), seq);
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_all_of_impl(const Tuple& tup, Pred&& pred,
                  std::index_sequence<Inds...>) noexcept -> bool
{
  static_assert(std::conjunction_v<std::is_invocable_r<
                    bool, Pred, decltype(std::get<Inds>(tup))>...>,
                "Predicate must be invocable returning a bool with every "
                "type in the tuple");

  return (std::invoke(pred, std::get<Inds>(tup)) && ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Determine if all elements of a tuple satisfy a unary predicate
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Pred Type of unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @param tup Input tuple
 *
 * @param pred Unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @return Returns `true` if `pred` applied to all elements of `tup`
 * return true. Returns `false` otherwise.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_all_of(const Tuple& tup,
                                          Pred&& pred) noexcept -> bool
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return impl::tuple_all_of_impl(tup, std::forward<Pred>(pred), seq);
}

/* {{{ doc */
/**
 * @brief Determine if no elements of a tuple satisfy a unary predicate
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Pred Type of unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @param tup Input tuple
 *
 * @param pred Unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @return Returns `true` if `pred` applied to no elements of `tup`
 * return true. Returns `false` otherwise.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_none_of(const Tuple& tup,
                                           Pred&& pred) noexcept -> bool
{
  return not tuple_any_of(tup, std::forward<Pred>(pred));
}

namespace impl {

template <typename Tuple, typename... Ts, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_push_back_impl(const Tuple& tup, std::index_sequence<Inds...>,
                     Ts&&... data) noexcept
    -> tl::push_back_t<Tuple, remove_cvref_t<Ts>...>
{
  return {std::get<Inds>(tup)..., std::forward<Ts>(data)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with `data` appended to the end
 * ex. tuple_push_back(std::tuple{2, 4.8}, true) == std::tuple{2, 4.8, true}
 *
 * @tparam Tuple Tuple type
 *
 * @tparam T Type of data to append
 *
 * @param tup Tuple to append to
 *
 * @param data Value to append
 *
 * @return New tuple with `data` appended
 */
/* }}} */
template <typename Tuple, typename... Ts>
[[nodiscard]] constexpr auto tuple_push_back(const Tuple& tup,
                                             Ts&&... data) noexcept
    -> tl::push_back_t<Tuple, remove_cvref_t<Ts>...>
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return impl::tuple_push_back_impl(tup, seq, std::forward<Ts>(data)...);
}

namespace impl {

template <typename Tuple, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_pop_back_impl(const Tuple& tup,
                    std::index_sequence<Inds...>) noexcept
    -> tl::pop_back_t<Tuple>
{
  return {std::get<Inds>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with the last element removed
 * ex. tuple_pop_back(std::tuple{2, 4.8, true}) == std::tuple{2, 4.8}
 *
 * @tparam Tuple Tuple type
 *
 * @param tup Tuple to remove the last element from
 *
 * @return New tuple with last element removed
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto tuple_pop_back(const Tuple& tup) noexcept
    -> tl::pop_back_t<Tuple>
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return impl::tuple_pop_back_impl(tup, seq);
}

namespace impl {

template <template <typename...> typename Tuple, typename... Ts,
          typename... Pack, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_push_front_impl(const Tuple<Pack...>& tup,
                      std::index_sequence<Inds...>, Ts&&... data) noexcept
    -> tl::push_front_t<Tuple<Pack...>, remove_cvref_t<Ts>...>
{
  return {std::forward<Ts>(data)..., std::get<Inds>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with `data` prepended to the beginning
 *
 * @tparam Tuple Tuple type
 *
 * @tparam T Type of data to prepend
 *
 * @param tup Tuple to prepend to
 *
 * @param data Value to prepend
 *
 * @return New tuple with `data` prepended
 */
/* }}} */
template <typename Tuple, typename... Ts>
[[nodiscard]] constexpr auto tuple_push_front(const Tuple& tup,
                                              Ts&&... data) noexcept
    -> tl::push_front_t<Tuple, remove_cvref_t<Ts>...>
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return impl::tuple_push_front_impl(tup, seq, std::forward<Ts>(data)...);
}

namespace impl {

template <typename Tuple, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_pop_front_impl(const Tuple& tup,
                     std::index_sequence<Inds...>) noexcept
    -> tl::pop_front_t<Tuple>
{
  return {std::get<Inds + 1>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with the first element removed
 * ex. tuple_pop_back(std::tuple{2, 4.8, true}) == std::tuple{2, 4.8}
 *
 * @tparam Tuple Tuple type
 *
 * @param tup Tuple to remove the first element from
 *
 * @return New tuple with first element removed
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto tuple_pop_front(const Tuple& tup) noexcept
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return impl::tuple_pop_front_impl(tup, seq);
}

namespace impl {

template <typename Tuple, std::size_t... Idxs>
[[nodiscard]] constexpr auto
tuple_rotate_left_impl(const Tuple& tup,
                       std::index_sequence<Idxs...>) noexcept
    -> tl::rotate_left_t<Tuple>
{
  return {std::get<Idxs + 1>(tup)..., std::get<0>(tup)};
}

} // namespace impl

template <typename Tuple>
[[nodiscard]] constexpr auto tuple_rotate_left(const Tuple& tup) noexcept
    -> tl::rotate_left_t<Tuple>
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return impl::tuple_rotate_left_impl(tup, seq);
}

namespace impl {

template <typename Tuple, std::size_t... Idxs>
[[nodiscard]] constexpr auto
tuple_rotate_right_impl(const Tuple& tup,
                        std::index_sequence<Idxs...>) noexcept
    -> tl::rotate_right_t<Tuple>
{
  return {std::get<std::tuple_size_v<Tuple> - 1>(tup),
          std::get<Idxs>(tup)...};
}

} // namespace impl

template <typename Tuple>
[[nodiscard]] constexpr auto tuple_rotate_right(const Tuple& tup) noexcept
    -> tl::rotate_right_t<Tuple>
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return impl::tuple_rotate_right_impl(tup, seq);
}

namespace impl {

template <typename Tuple, typename... Ts, std::size_t... pre_idxs,
          std::size_t... post_idxs>
[[nodiscard]] constexpr auto
tuple_insert_impl(const Tuple& tup, std::index_sequence<pre_idxs...>,
                  std::index_sequence<post_idxs...>, Ts&&... data) noexcept
    -> tl::insert_t<Tuple, sizeof...(pre_idxs), remove_cvref_t<Ts>...>
{
  constexpr std::size_t idx {sizeof...(pre_idxs)};

  return {std::get<pre_idxs>(tup)..., std::forward<Ts>(data)...,
          std::get<post_idxs + idx>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Inserts value(s) into an arbitrary index of a tuple.
 * ex. tuple_insert(std::tuple{3, true}, supl::index_constant<1>, 5.8)
 * == std::tuple{3, 5.8, true}
 *
 * @tparam Tuple Tuple type
 *
 * @tparam Ts Type(s) of data to insert
 *
 * @tparam Idx Index to insert at
 *
 * @param tup Tuple to insert into
 *
 * @param data Value(s) to insert
 *
 * @return New tuple with `data` inserted at index `Idx`
 */
/* }}} */
template <std::size_t Idx, typename Tuple, typename... Ts>
[[nodiscard]] constexpr auto tuple_insert(const Tuple& tup,
                                          Ts&&... data) noexcept
    -> tl::insert_t<Tuple, Idx, remove_cvref_t<Ts>...>
{
  static_assert(Idx <= std::tuple_size_v<Tuple>, "Index out of bounds");

  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - Idx> {}};

  return impl::tuple_insert_impl(tup, pre_seq, post_seq,
                                 std::forward<Ts>(data)...);
}

namespace impl {

template <typename Tuple, std::size_t... pre_idxs,
          std::size_t... post_idxs>
[[nodiscard]] constexpr auto
tuple_erase_impl(const Tuple& tup, std::index_sequence<pre_idxs...>,
                 std::index_sequence<post_idxs...>) noexcept
    -> tl::erase_t<Tuple, sizeof...(pre_idxs)>
{
  constexpr std::size_t idx {sizeof...(pre_idxs)};

  return {std::get<pre_idxs>(tup)...,
          std::get<post_idxs + idx + 1>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Erases element from a tuple at a given index.
 *
 * @tparam Tuple Type of input tuple.
 *
 * @tparam Idx Index at which to erase.
 *
 * @param tup Input tuple
 *
 * @return New tuple with element at index `Idx` erased.
 */
/* }}} */
template <std::size_t Idx, typename Tuple>
[[nodiscard]] constexpr auto tuple_erase(const Tuple& tup) noexcept
    -> tl::erase_t<Tuple, Idx>
{
  static_assert(Idx < std::tuple_size_v<Tuple>, "Index out of bounds");

  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - Idx - 1> {}};

  return impl::tuple_erase_impl(tup, pre_seq, post_seq);
}

template <std::size_t... Idxs, typename Tuple>
[[nodiscard]] constexpr auto tuple_reorder(const Tuple& tup) noexcept
    -> tl::reorder_t<Tuple, Idxs...>
{
  static_assert(((Idxs < std::tuple_size_v<Tuple>)&&...));

  return {std::get<Idxs>(tup)...};
}

namespace impl {
template <typename Tuple, std::size_t... Pre_Idxs,
          std::size_t... Post_Idxs, std::size_t Idx>
[[nodiscard]] constexpr auto
tuple_split_impl(const Tuple& tup, std::index_sequence<Pre_Idxs...>,
                 std::index_sequence<Post_Idxs...>,
                 index_constant<Idx>) noexcept -> tl::split_t<Tuple, Idx>
{
  return {{std::get<Pre_Idxs>(tup)...},
          {std::get<Post_Idxs + Idx>(tup)...}};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Splits a tuple at an arbitrary index.
 * Element at index `Idx` will be the first element of the second tuple.
 * ex. tuple_split(std::tuple{1, true, 'g'}, index_constant<1>) ==
 * std::pair{std::tuple{1}, std::tuple{true, 'g'}}
 *
 * @tparam Tuple Input tuple type.
 *
 * @tparam Idx Index at which to split.
 *
 * @param tup Tuple to split.
 *
 * @return Pair of tuples split from `tup` at index `Idx`.
 * ex. tuple_split(std::tuple{1, true, 'g'}, index_constant<1>) ==
 * std::pair{std::tuple{1}, std::tuple{true, 'g'}}
 */
/* }}} */
template <std::size_t Idx, typename Tuple>
[[nodiscard]] constexpr auto tuple_split(const Tuple& tup) noexcept
    -> tl::split_t<Tuple, Idx>
{
  static_assert(Idx < std::tuple_size_v<Tuple>, "Index out of bounds");

  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - Idx> {}};

  return impl::tuple_split_impl(tup, pre_seq, post_seq,
                                index_constant<Idx> {});
}

namespace impl {

template <typename Tuple, std::size_t... Inds, std::size_t Begin,
          std::size_t End>
[[nodiscard]] constexpr auto
subtuple_impl(const Tuple& tup, std::index_sequence<Inds...>,
              index_constant<Begin>, index_constant<End>) noexcept
    -> tl::sublist_t<Tuple, Begin, End>
{
  return {std::get<Inds + Begin>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Creates a subtuple from a range of indices.
 * Returned tuple will be created from the half-open range [begin, end).
 *
 * @tparam Tuple Tuple type
 *
 * @tparam begin Beginning index
 *
 * @tparam end Ending index
 *
 * @param tup Tuple to extract a subtuple from
 */
/* }}} */
template <std::size_t Begin, std::size_t End, typename Tuple>
[[nodiscard]] constexpr auto subtuple(const Tuple& tup)
    -> tl::sublist_t<Tuple, Begin, End>
{
  static_assert(Begin < std::tuple_size_v<Tuple>, "Begin out of bounds");
  static_assert(End <= std::tuple_size_v<Tuple>, "Begin out of bounds");

  constexpr auto seq {std::make_index_sequence<End - Begin> {}};

  return impl::subtuple_impl(tup, seq, index_constant<Begin> {},
                             index_constant<End> {});
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
constexpr auto tuple_count_if_impl(const Tuple& tup, Pred&& pred,
                                   std::index_sequence<Inds...>) noexcept
    -> std::size_t
{
  return (static_cast<std::size_t>(std::invoke(pred, std::get<Inds>(tup)))
          + ...);
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
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return impl::tuple_count_if_impl(tup, std::forward<Pred>(pred), seq);
}

namespace impl {

template <typename Tuple1, typename Tuple2, std::size_t... Idxs>
constexpr auto tuple_interleave_impl(const Tuple1& tup1,
                                     const Tuple2& tup2,
                                     std::index_sequence<Idxs...>) noexcept
    -> tl::interleave_t<Tuple1, Tuple2>
{
  return {std::tuple_cat(
      std::tuple {std::get<Idxs>(tup1), std::get<Idxs>(tup2)}...)};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Interleaves two tuples of the same length.
 *
 * ex. `tuple_interleave(std::tuple{3, true, 3.14},
 * std::tuple{'g', "neat"s, 42}) ==
 * std::tuple{3, 'g', true, "neat"s, 3.14, 42}`
 *
 * @param tup1 First tuple
 *
 * @param tup2 Second tuple
 *
 * @pre Both tuples must be of exactly the same length.
 * Failure to meet this precondition is a compile-time error.
 *
 * @return New tuple created by interleaving the two input tuples.
 * See example.
 */
/* }}} */
template <typename Tuple1, typename Tuple2>
[[nodiscard]] constexpr auto tuple_interleave(const Tuple1& tup1,
                                              const Tuple2& tup2) noexcept
    -> tl::interleave_t<Tuple1, Tuple2>
{
  static_assert(std::tuple_size_v<Tuple1> == std::tuple_size_v<Tuple2>,
                "Tuples must be of the same length");
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple1>> {}};

  return impl::tuple_interleave_impl(tup1, tup2, seq);
}

namespace impl {

template <typename Tuple, std::size_t... Idxs>
[[nodiscard]] constexpr auto
tuple_front_n_impl(const Tuple& tup, std::index_sequence<Idxs...>)
    -> tl::front_n_t<Tuple, sizeof...(Idxs)>
{
  return {std::get<Idxs>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple consisting only of the first `Count` elements
 * of `tup`
 *
 * @tparam Count Number of elements in output tuple
 *
 * @tparam Tuple Type of input tuple
 *
 * @param tup Input tuple
 *
 * @return New tuple consisting of the first `Count` elements of `tup`
 */
/* }}} */
template <std::size_t Count, typename Tuple>
[[nodiscard]] constexpr auto tuple_front_n(const Tuple& tup) noexcept
    -> tl::front_n_t<Tuple, Count>
{
  static_assert(Count <= std::tuple_size_v<Tuple>);

  constexpr auto seq {std::make_index_sequence<Count> {}};

  return impl::tuple_front_n_impl(tup, seq);
}

namespace impl {

template <typename Tuple, std::size_t... Idxs, std::size_t Offset>
[[nodiscard]] constexpr auto
tuple_back_n_impl(const Tuple& tup, std::index_sequence<Idxs...>,
                  supl::index_constant<Offset>)
    -> tl::back_n_t<Tuple, sizeof...(Idxs)>
{
  return {std::get<Idxs + Offset>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple consisting only of the last `Count` elements
 * of `tup`
 *
 * @tparam Count Number of elements in output tuple
 *
 * @tparam Tuple Type of input tuple
 *
 * @param tup Input tuple
 *
 * @return New tuple consisting of the last `Count` elements of `tup`
 */
/* }}} */
template <std::size_t Count, typename Tuple>
[[nodiscard]] constexpr auto tuple_back_n(const Tuple& tup) noexcept
    -> tl::back_n_t<Tuple, Count>
{
  static_assert(Count <= std::tuple_size_v<Tuple>);

  constexpr auto seq {std::make_index_sequence<Count> {}};
  constexpr auto offset {
      supl::index_constant<std::tuple_size_v<Tuple> - Count> {}};

  return impl::tuple_back_n_impl(tup, seq, offset);
}

/* {{{ doc */
/**
 * @brief Swaps two elements of a tuple. `Idx1 < Idx2` or `Idx2 < Idx1`,
 * or `Idx1 == Idx2` all work.
 * Swapping `Idx1` and `Idx2` does not change the result.
 *
 * @tparam Idx1 Index of element to swap with element at index Idx2
 *
 * @tparam Idx2 Index of element to swap with element at index Idx1
 *
 * @tparam Tuple Type of input tuple
 *
 * @param tup Input tuple
 *
 * @return Tuple with elements at `Idx1` and `Idx2` swapped
 * compared to input tuple
 */
/* }}} */
template <std::size_t Idx1, std::size_t Idx2, typename Tuple>
[[nodiscard]] constexpr auto tuple_elem_swap(const Tuple& tup) noexcept
    -> supl::tl::swap_t<Tuple, Idx1, Idx2>
{

  constexpr std::size_t min_idx {std::min(Idx1, Idx2)};
  constexpr std::size_t max_idx {std::max(Idx1, Idx2)};
  constexpr std::size_t tup_size {std::tuple_size_v<Tuple>};

  if constexpr ( Idx1 == Idx2 ) {

    return tup;

  } else if constexpr ( max_idx != tup_size - 1 ) {

    return std::tuple_cat(tuple_front_n<min_idx>(tup),
                          std::tuple {std::get<max_idx>(tup)},
                          subtuple<min_idx + 1, max_idx>(tup),
                          std::tuple {std::get<min_idx>(tup)},
                          tuple_back_n<tup_size - max_idx - 1>(tup));

  } else if constexpr ( max_idx == tup_size - 1 ) {

    return std::tuple_cat(tuple_front_n<min_idx>(tup),
                          std::tuple {std::get<max_idx>(tup)},
                          subtuple<min_idx + 1, max_idx>(tup),
                          std::tuple {std::get<min_idx>(tup)});
  }
}

namespace impl {

template <template <typename> typename Transform, typename Tuple,
          std::size_t... Idxs>
[[nodiscard]] constexpr auto
tuple_type_transform_impl(const Tuple& tup,
                          std::index_sequence<Idxs...>) noexcept
    -> tl::transform_t<Tuple, Transform>
{
  return {
      static_cast<typename Transform<tl::at_index_t<Idxs, Tuple>>::type>(
          std::get<Idxs>(tup))...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Transform types in tuple with unary type -> type metafunction.
 *
 * @details Suggested use: use with `supl::make_const_ref`to create a tuple
 * of const lvalue references to use before applying a tuple algorithm
 * to avoid expensive copies.
 * 
 * @pre For every type `T` in the input tuple,
 * `T` must be convertible to `Transform<T>::type` via `static_cast`.
 * Failure to meet this precondition is a compile-time error.
 *
 * @pre For every type `T` in the input tuple,
 * `Transform<T>::type` must be well formed.
 * Failure to meet this precondition is a compile-time error.
 *
 * @tparam Transform Unary type -> type metafunction used to transform
 * types in input tuple.
 *
 * @param tup Input tuple
 */
/* }}} */
template <template <typename> typename Transform, typename Tuple>
[[nodiscard]] constexpr auto
tuple_type_transform(const Tuple& tup) noexcept
    -> tl::transform_t<Tuple, Transform>
{
  constexpr auto seq {
      std::make_index_sequence<std::tuple_size_v<Tuple>> {}};

  return impl::tuple_type_transform_impl<Transform>(tup, seq);
}

} // namespace supl

#endif
