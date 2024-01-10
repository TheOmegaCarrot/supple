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
 * @note To the reader of this header, I must apologize.
 * A portion of the function declarations cause a misformat
 * with clang-tidy.
 * It is my opinion that it is more reasable as it is now,
 * compared leaving the misformat, or enclosing many
 * small sections in clang-tidy off blocks.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_TUPLE_ALGO_HPP
#define SUPPLE_CORE_TUPLE_ALGO_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "internal/algorithm/min_max.hpp"
#include "metaprogramming.hpp"
#include "type_list.hpp"

namespace supl {

/* {{{ doc */
/**
 * @brief `get` ADL two-step using `std::get` for index
 */
/* }}} */
template <std::size_t Idx, typename Gettable>
constexpr auto get(Gettable&& tup) noexcept -> decltype(auto)
{
  using std::get;
  return get<Idx>(std::forward<Gettable>(tup));
}

/* {{{ doc */
/**
 * @brief `get` ADL two-step using `std::get` for type
 */
/* }}} */
template <typename Type, typename Gettable>
constexpr auto get(Gettable&& tup) noexcept -> decltype(auto)
{
  using std::get;
  return get<Type>(std::forward<Gettable>(tup));
}
}  // namespace supl

namespace supl::tuple {

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
  constexpr void for_each_impl(
    Tuple&& tup,
    Func&& func,
    std::index_sequence<
      Inds...>) noexcept(noexcept((func(::supl::get<Inds>(tup)), ...)))
  {
    (func(::supl::get<Inds>(tup)), ...);
  }

}  // namespace impl

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
constexpr void for_each(Tuple&& tup, Func&& func) noexcept(noexcept(
  impl::for_each_impl(tup,
                      std::forward<Func>(func),
                      std::make_index_sequence<tl::size_v<Tuple>> {})))
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  impl::for_each_impl(
    std::forward<Tuple>(tup), std::forward<Func>(func), seq);
}

namespace impl {
  template <typename Tuple, typename Func, std::size_t... Idxs>
  constexpr void for_each_reverse_impl(Tuple&& tup,
                                       Func&& func,
                                       std::index_sequence<Idxs...>)
  {
    ((func(::supl::get<tl::size_v<Tuple> - Idxs - 1>(tup))), ...);
  }
}  // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple in reverse order.
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
constexpr void for_each_reverse(Tuple&& tup, Func&& func) noexcept(
  noexcept(impl::for_each_reverse_impl(
    tup,
    func,
    std::make_index_sequence<tl::size_v<Tuple>> {})))
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::for_each_reverse_impl(
    std::forward<Tuple>(tup), std::forward<Func>(func), seq);
}

namespace impl {
  template <std::size_t Max_Index,
            std::size_t Current_Idx = 0,
            typename Func,
            typename... Tuples>
  constexpr void zip_impl(Func&& func, Tuples&&... tuples)
  /* noexcept(TODO) */
  {
    if constexpr ( Max_Index == Current_Idx ) {
      return;
    } else {
      func(std::get<Current_Idx>(tuples)...);
      zip_impl<Max_Index, Current_Idx + 1>(
        std::forward<Func>(func), std::forward<Tuples>(tuples)...);
    }
  }
}  // namespace impl

template <typename Func, typename... Tuples>
constexpr void zip(Func&& func, Tuples&&... tuples) /* noexcept(TODO) */
{
  constexpr std::size_t max_index {min(tl::size_v<Tuples>...)};
  impl::zip_impl<max_index>(std::forward<Func>(func),
                            std::forward<Tuples>(tuples)...);
}

namespace impl {
  template <typename Tuple1,
            typename Tuple2,
            typename Func,
            std::size_t... Idxs>
  constexpr void for_each_both_impl(
    Tuple1&& tup1,
    Tuple2&& tup2,
    Func&& func,
    std::index_sequence<
      Idxs...>) noexcept(noexcept((func(std::get<Idxs>(tup1),
                                        std::get<Idxs>(tup2)),
                                   ...)))
  {
    (func(std::get<Idxs>(tup1), std::get<Idxs>(tup2)), ...);
  }
}  // namespace impl

template <typename Tuple1, typename Tuple2, typename Func>
constexpr void
for_each_both(Tuple1&& tup1, Tuple2&& tup2, Func&& func) noexcept(
  noexcept(impl::for_each_both_impl(
    tup1,
    tup2,
    func,
    std::make_index_sequence<std::min(tl::size_v<Tuple1>,
                                      tl::size_v<Tuple2>)> {})))
{
  constexpr auto seq {std::make_index_sequence<std::min(
    tl::size_v<Tuple1>, tl::size_v<Tuple2>)> {}};
  impl::for_each_both_impl(std::forward<Tuple1>(tup1),
                           std::forward<Tuple2>(tup2),
                           std::forward<Func>(func),
                           seq);
}

namespace impl {
  template <typename Tuple,
            typename Func,
            std::size_t... Idxs,
            std::size_t Begin>
  constexpr void for_each_in_subtuple_impl(
    const Tuple& tup,
    Func&& func,
    std::index_sequence<Idxs...>,
    index_constant<
      Begin>) noexcept(noexcept((func(::supl::get<Idxs + Begin>(tup)),
                                 ...)))
  {
    (func(::supl::get<Idxs + Begin>(tup)), ...);
  }

}  // namespace impl

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
template <std::size_t Begin,
          std::size_t End,
          typename Tuple,
          typename Func>
constexpr void
for_each_in_subtuple(const Tuple& tup, Func&& func) noexcept(noexcept(
  impl::for_each_in_subtuple_impl(tup,
                                  std::forward<Func>(func),
                                  std::make_index_sequence<End - Begin> {},
                                  index_constant<Begin> {})))
{
  static_assert(Begin < tl::size_v<Tuple>, "Begin out of bounds");
  static_assert(End <= tl::size_v<Tuple>, "Begin out of bounds");

  constexpr auto seq {std::make_index_sequence<End - Begin> {}};

  impl::for_each_in_subtuple_impl(
    tup, std::forward<Func>(func), seq, index_constant<Begin> {});
}

namespace impl {
  /* {{{ doc */
  /**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside transform
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
  template <template <typename...> typename Tuple,
            typename... Elems,
            typename Func,
            std::size_t... Inds>
  constexpr auto transform_impl(
    const Tuple<Elems...>& tup,
    Func&& func,
    std::index_sequence<
      Inds...>) noexcept((noexcept(func(::supl::get<Inds>(tup))) && ...)
                         && (std::is_nothrow_constructible_v<
                               decltype(func(::supl::get<Inds>(tup)))>
                             && ...))
    -> Tuple<decltype(func(::supl::get<Inds>(tup)))...>
  {
    return {func(::supl::get<Inds>(tup))...};
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto
transform(const Tuple& tup, Func&& func) noexcept(noexcept(
  impl::transform_impl(tup,
                       std::forward<Func>(func),
                       std::make_index_sequence<tl::size_v<Tuple>> {})))
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::transform_impl(tup, std::forward<Func>(func), seq);
}

namespace impl {
  template <typename Tuple, typename Pred, std::size_t... Inds>
  [[nodiscard]] constexpr auto any_of_impl(
    const Tuple& tup,
    Pred&& pred,
    std::index_sequence<
      Inds...>) noexcept(noexcept((pred(::supl::get<Inds>(tup)) || ...)))
    -> bool
  {
    static_assert(
      std::conjunction_v<
        std::
          is_invocable_r<bool, Pred, decltype(::supl::get<Inds>(tup))>...>,
      "Predicate must be invocable returning a bool with every "
      "type in the tuple");

    return (pred(::supl::get<Inds>(tup)) || ...);
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto
any_of(const Tuple& tup, Pred&& pred) noexcept(noexcept(impl::any_of_impl(
  tup,
  std::forward<Pred>(pred),
  std::make_index_sequence<tl::size_v<Tuple>> {}))) -> bool
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::any_of_impl(tup, std::forward<Pred>(pred), seq);
}

namespace impl {
  template <typename Tuple, typename Pred, std::size_t... Inds>
  [[nodiscard]] constexpr auto all_of_impl(
    const Tuple& tup,
    Pred&& pred,
    std::index_sequence<
      Inds...>) noexcept(noexcept((pred(::supl::get<Inds>(tup)) && ...)))
    -> bool
  {
    static_assert(
      std::conjunction_v<
        std::
          is_invocable_r<bool, Pred, decltype(::supl::get<Inds>(tup))>...>,
      "Predicate must be invocable returning a bool with every "
      "type in the tuple");

    return (pred(::supl::get<Inds>(tup)) && ...);
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto
all_of(const Tuple& tup, Pred&& pred) noexcept(noexcept(impl::all_of_impl(
  tup,
  std::forward<Pred>(pred),
  std::make_index_sequence<tl::size_v<Tuple>> {}))) -> bool
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::all_of_impl(tup, std::forward<Pred>(pred), seq);
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
[[nodiscard]] constexpr auto none_of(
  const Tuple& tup,
  Pred&& pred) noexcept(noexcept(any_of(tup, std::forward<Pred>(pred))))
  -> bool
{
  return not any_of(tup, std::forward<Pred>(pred));
}

namespace impl {
  template <typename Tuple, typename Pred, std::size_t... Inds>
  constexpr auto
  count_if_impl(const Tuple& tup,
                Pred&& pred,
                std::index_sequence<
                  Inds...>) noexcept(noexcept((static_cast<std::
                                                             size_t>(pred(
                                                 ::supl::get<Inds>(tup)))
                                               + ...))) -> std::size_t
  {
    return (static_cast<std::size_t>(pred(::supl::get<Inds>(tup))) + ...);
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto
count_if(const Tuple& tup, Pred&& pred) noexcept(noexcept(
  impl::count_if_impl(tup,
                      std::forward<Pred>(pred),
                      std::make_index_sequence<tl::size_v<Tuple>> {})))
  -> std::size_t
{
  // This (and much of this file) could be much nicer in C++20
  // no impl function, just IILE
  /* return [&]<std::size_t... Inds>(std::index_sequence<Inds...>) { */
  /*   return (static_cast<std::size_t>(pred(::supl::get<Inds>(tup))) + ...); */
  /* }(std::make_index_sequence<tl::size_v<Tuple>> {}); */

  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::count_if_impl(tup, std::forward<Pred>(pred), seq);
}

namespace impl {

  template <typename Tuple, typename... Ts, std::size_t... Inds>
  [[nodiscard]] constexpr auto push_back_impl(
    const Tuple& tup,
    std::index_sequence<Inds...>,
    Ts&&... data) noexcept(tl::all_of_v<Tuple,
                                        std::is_nothrow_copy_constructible>
                           && (std::is_nothrow_copy_constructible_v<Ts>
                               && ...))
    -> tl::push_back_t<Tuple, remove_cvref_t<Ts>...>
  {
    return {::supl::get<Inds>(tup)..., std::forward<Ts>(data)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with `data` appended to the end
 * ex. push_back(std::tuple{2, 4.8}, true) == std::tuple{2, 4.8, true}
 *
 * @tparam Tuple Tuple type
 *
 * @tparam Ts Type of data to append
 *
 * @param tup Tuple to append to
 *
 * @param data Value to append
 *
 * @return New tuple with `data` appended
 */
/* }}} */
template <typename Tuple, typename... Ts>
[[nodiscard]] constexpr auto
push_back(const Tuple& tup, Ts&&... data) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>
  && (std::is_nothrow_copy_constructible_v<Ts> && ...))
  -> tl::push_back_t<Tuple, remove_cvref_t<Ts>...>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::push_back_impl(tup, seq, std::forward<Ts>(data)...);
}

namespace impl {

  template <typename As,
            typename Param,
            typename Tuple,
            std::size_t... Idxs>
  [[nodiscard]] constexpr auto push_back_as_impl(
    const Tuple& tup,
    std::index_sequence<Idxs...>,
    Param&& data) noexcept(tl::all_of_v<Tuple,
                                        std::is_nothrow_copy_constructible>
                           && std::is_nothrow_constructible_v<As, Param>)
    -> tl::push_back_t<Tuple, As>
  {
    return {::supl::get<Idxs>(tup)..., std::forward<Param>(data)};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Push back with specified data type.
 *
 * @details The resulting tuple will contain copies of all input elements,
 * plus the appended `data`, which will be converted to the specified type.
 * The specified type may be a reference type, or may be any type
 * for which an implicit conversion from `Param` to `As` exists.
 *
 * @tparam As Exact type appended to resulting tuple
 *
 * @tparam Param Type of supplied parameter.
 * Must be implicitly convertible to `As`.
 *
 * @tparam Tuple Type of input tuple
 *
 * @param tup Input tuple
 *
 * @param data Input data of type `Param`,
 * will be used to initialize a value of type `As`.
 */
/* }}} */
template <typename As, typename Param, typename Tuple>
[[nodiscard]] constexpr auto
push_back_as(const Tuple& tup, Param&& data) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>
  && std::is_nothrow_constructible_v<As, Param>)
  -> tl::push_back_t<Tuple, As>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::push_back_as_impl<As>(tup, seq, std::forward<Param>(data));
}

namespace impl {
  template <typename Tuple, std::size_t... Inds>
  [[nodiscard]] constexpr auto
  pop_back_impl(const Tuple& tup, std::index_sequence<Inds...>) noexcept(
    tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
    -> tl::pop_back_t<Tuple>
  {
    return {::supl::get<Inds>(tup)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with the last element removed
 * ex. pop_back(std::tuple{2, 4.8, true}) == std::tuple{2, 4.8}
 *
 * @tparam Tuple Tuple type
 *
 * @param tup Tuple to remove the last element from
 *
 * @return New tuple with last element removed
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto pop_back(const Tuple& tup) noexcept(
  tl::all_of_v<tl::pop_back_t<Tuple>, std::is_nothrow_copy_constructible>)
  -> tl::pop_back_t<Tuple>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple> - 1> {}};
  return impl::pop_back_impl(tup, seq);
}

namespace impl {
  template <template <typename...> typename Tuple,
            typename... Ts,
            typename... Pack,
            std::size_t... Inds>
  [[nodiscard]] constexpr auto push_front_impl(
    const Tuple<Pack...>& tup,
    std::index_sequence<Inds...>,
    Ts&&... data) noexcept(tl::all_of_v<Tuple<Pack...>,
                                        std::is_nothrow_copy_constructible>
                           && (std::is_nothrow_copy_constructible_v<Ts>
                               && ...))
    -> tl::push_front_t<Tuple<Pack...>, remove_cvref_t<Ts>...>
  {
    return {std::forward<Ts>(data)..., ::supl::get<Inds>(tup)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with `data` prepended to the beginning
 *
 * @tparam Tuple Tuple type
 *
 * @tparam Ts Type of data to prepend
 *
 * @param tup Tuple to prepend to
 *
 * @param data Value to prepend
 *
 * @return New tuple with `data` prepended
 */
/* }}} */
template <typename Tuple, typename... Ts>
[[nodiscard]] constexpr auto
push_front(const Tuple& tup, Ts&&... data) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>
  && (std::is_nothrow_copy_constructible_v<Ts> && ...))
  -> tl::push_front_t<Tuple, remove_cvref_t<Ts>...>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::push_front_impl(tup, seq, std::forward<Ts>(data)...);
}

namespace impl {

  template <typename As,
            typename Param,
            typename Tuple,
            std::size_t... Idxs>
  [[nodiscard]] constexpr auto push_front_as_impl(
    const Tuple& tup,
    std::index_sequence<Idxs...>,
    Param&& data) noexcept(tl::all_of_v<Tuple,
                                        std::is_nothrow_copy_constructible>
                           && std::is_nothrow_constructible_v<As, Param>)
    -> tl::push_front_t<Tuple, As>
  {
    return {std::forward<Param>(data), ::supl::get<Idxs>(tup)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Push front with specified data type.
 *
 * @details The resulting tuple will contain copies of all input elements,
 * plus the prepended `data`, which will be converted to the specified type.
 * The specified type may be a reference type, or may be any type
 * for which an implicit conversion from `Param` to `As` exists.
 *
 * @tparam As Exact type appended to resulting tuple
 *
 * @tparam Param Type of supplied parameter.
 * Must be implicitly convertible to `As`.
 *
 * @tparam Tuple Type of input tuple
 *
 * @param tup Input tuple
 *
 * @param data Input data of type `Param`,
 * will be used to initialize a value of type `As`.
 */
/* }}} */
template <typename As, typename Param, typename Tuple>
[[nodiscard]] constexpr auto
push_front_as(const Tuple& tup, Param&& data) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>
  && std::is_nothrow_constructible_v<As, Param>)
  -> tl::push_front_t<Tuple, As>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::push_front_as_impl<As>(tup, seq, std::forward<Param>(data));
}

namespace impl {
  template <typename Tuple, std::size_t... Inds>
  [[nodiscard]] constexpr auto
  pop_front_impl(const Tuple& tup, std::index_sequence<Inds...>) noexcept(
    tl::all_of_v<tl::pop_front_t<Tuple>,
                 std::is_nothrow_copy_constructible>)
    -> tl::pop_front_t<Tuple>
  {
    return {::supl::get<Inds + 1>(tup)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with the first element removed
 * ex. pop_back(std::tuple{2, 4.8, true}) == std::tuple{2, 4.8}
 *
 * @tparam Tuple Tuple type
 *
 * @param tup Tuple to remove the first element from
 *
 * @return New tuple with first element removed
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto pop_front(const Tuple& tup) noexcept(
  tl::all_of_v<tl::pop_front_t<Tuple>, std::is_nothrow_copy_constructible>)
  -> tl::pop_front_t<Tuple>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple> - 1> {}};
  return impl::pop_front_impl(tup, seq);
}

namespace impl {
  template <typename Tuple, std::size_t... Idxs>
  [[nodiscard]] constexpr auto rotate_left_impl(
    const Tuple& tup,
    std::index_sequence<
      Idxs...>) noexcept(tl::all_of_v<Tuple,
                                      std::is_nothrow_copy_constructible>)
    -> tl::rotate_left_t<Tuple>
  {
    return {::supl::get<Idxs + 1>(tup)..., ::supl::get<0>(tup)};
  }

}  // namespace impl

template <typename Tuple>
[[nodiscard]] constexpr auto rotate_left(const Tuple& tup) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
  -> tl::rotate_left_t<Tuple>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple> - 1> {}};
  return impl::rotate_left_impl(tup, seq);
}

namespace impl {
  template <typename Tuple, std::size_t... Idxs>
  [[nodiscard]] constexpr auto rotate_right_impl(
    const Tuple& tup,
    std::index_sequence<
      Idxs...>) noexcept(tl::all_of_v<Tuple,
                                      std::is_nothrow_copy_constructible>)
    -> tl::rotate_right_t<Tuple>
  {
    return {::supl::get<tl::size_v<Tuple> - 1>(tup),
            ::supl::get<Idxs>(tup)...};
  }

}  // namespace impl

template <typename Tuple>
[[nodiscard]] constexpr auto rotate_right(const Tuple& tup) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
  -> tl::rotate_right_t<Tuple>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple> - 1> {}};
  return impl::rotate_right_impl(tup, seq);
}

namespace impl {
  template <typename Tuple,
            typename... Ts,
            std::size_t... pre_idxs,
            std::size_t... post_idxs>
  [[nodiscard]] constexpr auto insert_impl(
    const Tuple& tup,
    std::index_sequence<pre_idxs...>,
    std::index_sequence<post_idxs...>,
    Ts&&... data) noexcept(tl::all_of_v<Tuple,
                                        std::is_nothrow_copy_constructible>
                           && (std::is_nothrow_constructible_v<Ts, Ts>
                               && ...))
    -> tl::insert_t<Tuple, sizeof...(pre_idxs), remove_cvref_t<Ts>...>
  {
    constexpr std::size_t idx {sizeof...(pre_idxs)};

    return {::supl::get<pre_idxs>(tup)...,
            std::forward<Ts>(data)...,
            ::supl::get<post_idxs + idx>(tup)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Inserts value(s) into an arbitrary index of a tuple.
 * ex. insert<1>(std::tuple{3, true}, 5.8)
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
[[nodiscard]] constexpr auto
insert(const Tuple& tup, Ts&&... data) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>
  && (std::is_nothrow_constructible_v<Ts, Ts> && ...))
  -> tl::insert_t<Tuple, Idx, remove_cvref_t<Ts>...>
{
  static_assert(Idx <= tl::size_v<Tuple>, "Index out of bounds");

  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
    std::make_index_sequence<tl::size_v<Tuple> - Idx> {}};

  return impl::insert_impl(
    tup, pre_seq, post_seq, std::forward<Ts>(data)...);
}

namespace impl {
  template <typename Tuple,
            std::size_t... pre_idxs,
            std::size_t... post_idxs>
  [[nodiscard]] constexpr auto erase_impl(
    const Tuple& tup,
    std::index_sequence<pre_idxs...>,
    std::index_sequence<
      post_idxs...>) noexcept(tl::
                                all_of_v<
                                  tl::erase_t<Tuple, sizeof...(pre_idxs)>,
                                  std::is_nothrow_copy_constructible>)
    -> tl::erase_t<Tuple, sizeof...(pre_idxs)>
  {
    constexpr std::size_t idx {sizeof...(pre_idxs)};

    return {::supl::get<pre_idxs>(tup)...,
            ::supl::get<post_idxs + idx + 1>(tup)...};
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto erase(const Tuple& tup) noexcept(
  tl::all_of_v<tl::erase_t<Tuple, Idx>,
               std::is_nothrow_copy_constructible>)
  -> tl::erase_t<Tuple, Idx>
{
  static_assert(Idx < tl::size_v<Tuple>, "Index out of bounds");

  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
    std::make_index_sequence<tl::size_v<Tuple> - Idx - 1> {}};

  return impl::erase_impl(tup, pre_seq, post_seq);
}

namespace impl {
  template <typename Tuple,
            typename T,
            std::size_t... Pre_Idxs,
            std::size_t... Post_Idxs,
            std::size_t Idx>
  [[nodiscard]] constexpr auto replace_impl(
    const Tuple& tup,
    T&& data,
    std::index_sequence<Pre_Idxs...>,
    std::index_sequence<Post_Idxs...>,
    index_constant<
      Idx>) noexcept(tl::
                       all_of_v<
                         tl::replace_t<Tuple, Idx, remove_cvref_t<T>>,
                         std::is_nothrow_copy_constructible>)
    -> tl::replace_t<Tuple, Idx, remove_cvref_t<T>>
  {
    return {::supl::get<Pre_Idxs>(tup)...,
            std::forward<T>(data),
            ::supl::get<Post_Idxs + Idx + 1>(tup)...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Replace a single element of a tuple
 *
 * @tparam Idx Index of element to be replaced.
 * Must be explicitly specified.
 *
 * @tparam T Type to replace element at `Idx` with.
 * May be explicitly speciefied, or deduced.
 *
 * @tparam Tuple Type of input tuple.
 * May be explicitly speciefied, or deduced, but there is
 * no clear reason to explicitly specify.
 *
 * @param tup Input tuple
 *
 * @param data Value to use to replace element of input
 *
 * @return New tuple with element `Idx` of `tup`
 * replaced with `data` of type `T`.
 * ex. `replace<1>(std::tuple{42, 3.14}, 'k') == std::tuple{42, 'k'}`
 */
/* }}} */
template <std::size_t Idx, typename T, typename Tuple>
[[nodiscard]] constexpr auto replace(const Tuple& tup, T&& data) noexcept(
  tl::all_of_v<tl::replace_t<Tuple, Idx, remove_cvref_t<T>>,
               std::is_nothrow_copy_constructible>)
  -> tl::replace_t<Tuple, Idx, remove_cvref_t<T>>
{
  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
    std::make_index_sequence<tl::size_v<Tuple> - Idx - 1> {}};

  return impl::replace_impl(
    tup, std::forward<T>(data), pre_seq, post_seq, index_constant<Idx> {});
}

/* {{{ doc */
/**
 * @brief Arbitrarily reorder elements of a tuple
 *
 * @tparam Idxs Indices to get from input tuple to use to construct a new tuple.
 * Duplicate indices are permitted. (ex. `3, 2, 1, 0, 2`)
 * Omitted indices are permitted. (ex. `0, 1, 3, 4`)
 *
 * @tparam Tuple Type of input tuple
 *
 * @param tup Input tuple
 *
 * @return Tuple created from given indices.
 * ex. `reorder<3,2,3,1>(std::tuple{0,1,2,3}) == std::tuple{3,2,3,1}`
 */
/* }}} */
template <std::size_t... Idxs, typename Tuple>
[[nodiscard]] constexpr auto reorder(const Tuple& tup) noexcept(
  (std::is_nothrow_copy_constructible_v<decltype(::supl::get<Idxs>(tup))>
   && ...)) -> tl::reorder_t<Tuple, Idxs...>
{
  static_assert(((Idxs < tl::size_v<Tuple>) &&...));

  return {::supl::get<Idxs>(tup)...};
}

namespace impl {
  template <typename Tuple, std::size_t... Idxs>
  [[nodiscard]] constexpr auto
  reverse_impl(const Tuple& tup, std::index_sequence<Idxs...>) noexcept(
    tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
    -> tl::reverse_t<Tuple>
  {
    return {::supl::get<tl::size_v<Tuple> - Idxs - 1>(tup)...};
  }
}  // namespace impl

/* {{{ doc */
/**
 * @brief Reverse a tuple
 *
 * @details Returns the input tuple, but with the elements in
 * reverse order.
 *
 * ex. `reverse(std::tuple{42, 'g', false}) == std::tuple{false, 'g', 42}`
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto reverse(const Tuple& tup) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
  -> tl::reverse_t<Tuple>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};
  return impl::reverse_impl(tup, seq);
}

namespace impl {
  template <typename Tuple,
            std::size_t... Pre_Idxs,
            std::size_t... Post_Idxs,
            std::size_t Idx>
  [[nodiscard]] constexpr auto split_impl(
    const Tuple& tup,
    std::index_sequence<Pre_Idxs...>,
    std::index_sequence<Post_Idxs...>,
    index_constant<
      Idx>) noexcept(tl::all_of_v<Tuple,
                                  std::is_nothrow_copy_constructible>)
    -> std::pair<typename tl::split_t<Tuple, Idx>::first,
                 typename tl::split_t<Tuple, Idx>::second>
  {
    return {{::supl::get<Pre_Idxs>(tup)...},
            {::supl::get<Post_Idxs + Idx>(tup)...}};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Splits a tuple at an arbitrary index.
 * Element at index `Idx` will be the first element of the second tuple.
 * ex. split<1>(std::tuple{1, true, 'g'}) ==
 * std::pair{std::tuple{1}, std::tuple{true, 'g'}}
 *
 * @tparam Tuple Input tuple type.
 *
 * @tparam Idx Index at which to split.
 *
 * @param tup Tuple to split.
 *
 * @return Pair of tuples split from `tup` at index `Idx`.
 * ex. split<1>(std::tuple{1, true, 'g'}) ==
 * std::pair{std::tuple{1}, std::tuple{true, 'g'}}
 */
/* }}} */
template <std::size_t Idx, typename Tuple>
[[nodiscard]] constexpr auto split(const Tuple& tup) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
  -> std::pair<typename tl::split_t<Tuple, Idx>::first,
               typename tl::split_t<Tuple, Idx>::second>
{
  static_assert(Idx <= tl::size_v<Tuple>, "Index out of bounds");

  constexpr auto pre_seq {std::make_index_sequence<Idx> {}};
  constexpr auto post_seq {
    std::make_index_sequence<tl::size_v<Tuple> - Idx> {}};

  return impl::split_impl(tup, pre_seq, post_seq, index_constant<Idx> {});
}

namespace impl {
  template <typename Tuple,
            std::size_t... Inds,
            std::size_t Begin,
            std::size_t End>
  [[nodiscard]] constexpr auto subtuple_impl(
    const Tuple& tup,
    std::index_sequence<Inds...>,
    index_constant<Begin>,
    index_constant<
      End>) noexcept((std::
                        is_nothrow_copy_constructible_v<
                          decltype(::supl::get<Inds + Begin>(tup))>
                      && ...)) -> tl::sublist_t<Tuple, Begin, End>
  {
    return {::supl::get<Inds + Begin>(tup)...};
  }
}  // namespace impl

/* {{{ doc */
/**
 * @brief Creates a subtuple from a range of indices.
 * Returned tuple will be created from the half-open range [begin, end).
 *
 * @tparam Tuple Tuple type
 *
 * @tparam Begin Beginning index
 *
 * @tparam End Ending index
 *
 * @param tup Tuple to extract a subtuple from
 */
/* }}} */
template <std::size_t Begin, std::size_t End, typename Tuple>
[[nodiscard]] constexpr auto subtuple(const Tuple& tup) noexcept(
  noexcept(impl::subtuple_impl(tup,
                               std::make_index_sequence<End - Begin> {},
                               index_constant<Begin> {},
                               index_constant<End> {})))
  -> tl::sublist_t<Tuple, Begin, End>
{
  static_assert(Begin < tl::size_v<Tuple>, "Begin out of bounds");
  static_assert(End <= tl::size_v<Tuple>, "Begin out of bounds");

  constexpr auto seq {std::make_index_sequence<End - Begin> {}};

  return impl::subtuple_impl(
    tup, seq, index_constant<Begin> {}, index_constant<End> {});
}

namespace impl {
  template <typename Tuple1, typename Tuple2, std::size_t... Idxs>
  constexpr auto interleave_impl(
    const Tuple1& tup1,
    const Tuple2& tup2,
    std::index_sequence<
      Idxs...>) noexcept(tl::all_of_v<Tuple1,
                                      std::is_nothrow_copy_constructible>
                         && tl::all_of_v<
                           Tuple2,
                           std::is_nothrow_copy_constructible>)
    -> tl::interleave_t<Tuple1, Tuple2>
  {

    // GOAL: remove `tuple_cat`
    return {std::tuple_cat(
      std::tuple {::supl::get<Idxs>(tup1), ::supl::get<Idxs>(tup2)}...)};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Interleaves two tuples of the same length.
 *
 * ex. `interleave(std::tuple{3, true, 3.14},
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
[[nodiscard]] constexpr auto
interleave(const Tuple1& tup1, const Tuple2& tup2) noexcept(
  tl::all_of_v<Tuple1, std::is_nothrow_copy_constructible>
  && tl::all_of_v<Tuple2, std::is_nothrow_copy_constructible>)
  -> tl::interleave_t<Tuple1, Tuple2>
{
  static_assert(tl::size_v<Tuple1> == tl::size_v<Tuple2>,
                "Tuples must be of the same length");
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple1>> {}};

  return impl::interleave_impl(tup1, tup2, seq);
}

namespace impl {
  template <typename Tuple, std::size_t... Idxs>
  [[nodiscard]] constexpr auto alternating_split_impl(
    const Tuple& tup,
    std::index_sequence<
      Idxs...>) noexcept(tl::all_of_v<Tuple,
                                      std::is_nothrow_copy_constructible>)
    -> std::pair<std::tuple<tl::at_index_t<Idxs * 2, Tuple>...>,
                 std::tuple<tl::at_index_t<Idxs * 2 + 1, Tuple>...>>
  {
    return {{::supl::get<Idxs * 2>(tup)...},
            {::supl::get<Idxs * 2 + 1>(tup)...}};
  }
}  // namespace impl

/* {{{ doc */
/**
 * @brief Split a tuple into a pair of tuples,
 * each containing every other element.
 *
 * @details This is best explained with an example:
 * `alternating_split(tuple{42, true, 'e', 3.14})
 * == pair{tuple{42, 'e'}, tuple{true, 3.14}}`
 *
 * If the input tuple has an odd number of elements,
 * the final element will be dropped. ex.
 * `alternating_split(tuple{42, true, 'e', 3.14, 81})
 * == pair{tuple{42, 'e'}, tuple{true, 3.14}}`
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto alternating_split(const Tuple& tup) noexcept(
  tl::all_of_v<Tuple, std::is_nothrow_copy_constructible>)
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple> / 2> {}};
  return impl::alternating_split_impl(tup, seq);
}

namespace impl {
  template <std::size_t Chunk_Size,
            template <typename...>
            typename Tuple,
            typename... Elems,
            std::size_t... Chunk_Seq>
  [[nodiscard]] constexpr auto chunk_impl(
    const Tuple<Elems...>& tup,
    std::index_sequence<
      Chunk_Seq...>) noexcept(tl::
                                all_of_v<
                                  tl::type_list<Elems...>,
                                  std::is_nothrow_copy_constructible>)
  {
    return Tuple {subtuple<Chunk_Size * Chunk_Seq,
                           (Chunk_Size * Chunk_Seq) + Chunk_Size>(tup)...};
  }
}  // namespace impl

/* {{{ doc */
/**
 * @brief Split a tuple into chunks, preserving order
 *
 * @details Example:
 * `chunk<2>(std::tuple{42, 3.14, true, 'r'})
 * == std::tuple{std::tuple{42, 3.14}, std::tuple{true, 'r}}`
 *
 * @tparam Chunk_Size Size of the output chunks
 *
 * @tparam Tuple Template type of tuple
 *
 * @tparam Elems Types of tuple elements
 *
 * @param tup Input tuple
 */
/* }}} */
template <std::size_t Chunk_Size,
          template <typename...>
          typename Tuple,
          typename... Elems>
[[nodiscard]] constexpr auto chunk(const Tuple<Elems...>& tup) noexcept(
  tl::all_of_v<tl::type_list<Elems...>,
               std::is_nothrow_copy_constructible>)
{
  static_assert(Chunk_Size != 0, "Chunk size cannot be 0");
  static_assert(sizeof...(Elems) % Chunk_Size == 0,
                "Size of tuple must be a multiple of chunk size");

  if constexpr ( sizeof...(Elems) == 0 ) {
    // if empty tuple input, return empty tuple
    return Tuple<> {};
  } else {
    return impl::chunk_impl<Chunk_Size>(
      tup, std::make_index_sequence<sizeof...(Elems) / Chunk_Size> {});
  }
}

namespace impl {
  template <typename Tuple, std::size_t... Idxs>
  [[nodiscard]] constexpr auto
  front_n_impl(const Tuple& tup, std::index_sequence<Idxs...>) noexcept(
    tl::all_of_v<tl::front_n_t<Tuple, sizeof...(Idxs)>,
                 std::is_nothrow_copy_constructible>)
    -> tl::front_n_t<Tuple, sizeof...(Idxs)>
  {
    return {::supl::get<Idxs>(tup)...};
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto front_n(const Tuple& tup) noexcept(
  tl::all_of_v<tl::front_n_t<Tuple, Count>,
               std::is_nothrow_copy_constructible>)
  -> tl::front_n_t<Tuple, Count>
{
  static_assert(Count <= tl::size_v<Tuple>);

  constexpr auto seq {std::make_index_sequence<Count> {}};

  return impl::front_n_impl(tup, seq);
}

/* {{{ doc */
/**
 * @brief Get first element of tuple
 *
 * @note This is redundant to `::supl::get<0>`,
 * but has been added anyways for interface consistency.
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto front(Tuple&& tup) noexcept -> decltype(auto)
{
  return ::supl::get<0>(tup);
}

namespace impl {
  template <typename Tuple, std::size_t... Idxs, std::size_t Offset>
  [[nodiscard]] constexpr auto back_n_impl(
    const Tuple& tup,
    std::index_sequence<Idxs...>,
    index_constant<
      Offset>) noexcept(tl::all_of_v<tl::back_n_t<Tuple, sizeof...(Idxs)>,
                                     std::is_nothrow_copy_constructible>)
    -> tl::back_n_t<Tuple, sizeof...(Idxs)>
  {
    return {::supl::get<Idxs + Offset>(tup)...};
  }

}  // namespace impl

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
[[nodiscard]] constexpr auto back_n(const Tuple& tup) noexcept(
  tl::all_of_v<tl::back_n_t<Tuple, Count>,
               std::is_nothrow_copy_constructible>)
  -> tl::back_n_t<Tuple, Count>
{
  static_assert(Count <= tl::size_v<Tuple>);

  constexpr auto seq {std::make_index_sequence<Count> {}};
  constexpr auto offset {
    supl::index_constant<tl::size_v<Tuple> - Count> {}};

  return impl::back_n_impl(tup, seq, offset);
}

/* {{{ doc */
/**
 * @brief Get last element of tuple
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto back(Tuple&& tup) noexcept -> decltype(auto)
{
  return ::supl::get<tl::size_v<Tuple> - 1>(tup);
}

namespace impl {
  template <std::size_t Idx1,
            std::size_t Idx2,
            template <typename...>
            typename Tuple,
            typename... List_Pack,
            typename... Tuple_Pack,
            std::size_t... Idxs>
  [[nodiscard]] constexpr auto elem_swap_impl(
    tl::type_list<tl::type_index_pair<List_Pack, Idxs>...>,
    const Tuple<Tuple_Pack...>&
      tup) noexcept(tl::all_of_v<Tuple<List_Pack...>,
                                 std::is_nothrow_copy_constructible>)
    -> Tuple<List_Pack...>
  {
    return {::supl::get<Idxs>(tup)...};
  }
}  // namespace impl

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
template <std::size_t Idx1,
          std::size_t Idx2,
          template <typename...>
          typename Tuple,
          typename... Pack>
[[nodiscard]] constexpr auto elem_swap(const Tuple<Pack...>& tup) noexcept(
  tl::all_of_v<Tuple<Pack...>, std::is_nothrow_copy_constructible>)
  -> tl::swap_t<Tuple<Pack...>, Idx1, Idx2>
{
  if constexpr ( Idx1 == Idx2 ) {
    return tup;
  } else {
    return impl::elem_swap_impl<Idx1, Idx2>(
      tl::swap_t<tl::enumerate_t<tl::type_list<Pack...>>, Idx1, Idx2> {},
      tup);
  }
}

namespace impl {
  template <template <typename> typename Transform,
            typename Tuple,
            std::size_t... Idxs>
  [[nodiscard]] constexpr auto type_transform_impl(
    const Tuple& tup,
    std::index_sequence<
      Idxs...>) noexcept(tl::all_of_v<tl::transform_t<Tuple, Transform>,
                                      std::is_nothrow_copy_constructible>)
    -> tl::transform_t<Tuple, Transform>
  {
    return {
      static_cast<typename Transform<tl::at_index_t<Idxs, Tuple>>::type>(
        ::supl::get<Idxs>(tup))...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Transform types in tuple with unary type -> type metafunction.
 *
 * @details Suggested use: use with `supl::make_const_ref`to create a tuple
 * of const lvalue references to use before applying a tuple algorithm
 * to avoid expensive copies. Then use with `std::decay` to resolve the
 * references to only perform one copy.
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
[[nodiscard]] constexpr auto type_transform(const Tuple& tup) noexcept(
  tl::all_of_v<tl::transform_t<Tuple, Transform>,
               std::is_nothrow_copy_constructible>)
  -> tl::transform_t<Tuple, Transform>
{
  constexpr auto seq {std::make_index_sequence<tl::size_v<Tuple>> {}};

  return impl::type_transform_impl<Transform>(tup, seq);
}

namespace impl {

  template <typename... New_Types,
            template <typename...>
            typename Tuple,
            typename... Old_Types,
            std::size_t... Idxs>
  [[nodiscard]] constexpr auto convert_impl(
    const Tuple<Old_Types...>& tup,
    std::index_sequence<
      Idxs...>) noexcept((std::is_nothrow_constructible_v<New_Types,
                                                          Old_Types>
                          && ...)) -> Tuple<New_Types...>
  {
    return {static_cast<Old_Types>(::supl::get<Idxs>(tup))...};
  }

}  // namespace impl

/* {{{ doc */
/**
 * @brief Convert the elements of a tuple
 *
 * @details All conversions must be possible via `static_cast`.
 *
 * @tparam New_Types Types which elements of input tuple will
 * be converted to
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Old_Types Types of elements of input tuple
 *
 * @param tup Input tuple
 *
 * @return Tuple of `New_Types`
 */
/* }}} */
template <typename... New_Types,
          template <typename...>
          typename Tuple,
          typename... Old_Types>
[[nodiscard]] constexpr auto
convert(const Tuple<Old_Types...>& tup) noexcept(
  (std::is_nothrow_constructible_v<New_Types, Old_Types> && ...))
  -> Tuple<New_Types...>
{
  static_assert(
    sizeof...(New_Types) == sizeof...(Old_Types),
    "Must provide exactly the same number of types as in input tuple");

  constexpr auto seq {std::make_index_sequence<sizeof...(Old_Types)> {}};

  return impl::convert_impl<New_Types...>(tup, seq);
}

/* {{{ doc */
/**
 * @brief Create a tuple owning its elements
 *
 * @details Any references in the input tuple will correspond to
 * an unqualified, non-reference value.
 * This function is equivalent to
 * `supl::tuple::type_transform<supl::remove_cvref>`
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto resolve_refs(const Tuple& tup) noexcept(
  noexcept(type_transform<remove_cvref>(tup)))
  -> decltype(type_transform<remove_cvref>(tup))
{
  return type_transform<remove_cvref>(tup);
}

/* {{{ doc */
/**
 * @brief Create a tuple of const references
 */
/* }}} */
template <typename... Ts>
[[nodiscard]] constexpr auto make_cref_tuple(const Ts&... values) noexcept
  -> std::tuple<const Ts&...>
{
  return {values...};
}

}  // namespace supl::tuple

#endif
