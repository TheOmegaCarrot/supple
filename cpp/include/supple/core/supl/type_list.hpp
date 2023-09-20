/* {{{ doc */
/**
 * @file type_list.hpp
 *
 * @brief Type list operations
 *
 * @details This header contains various metafunctions for operations
 * on type lists.
 *
 * @author Ethan Hancock
 *
 * @copyright MIT Public License
 */
/* }}} */

#ifndef SUPPLE_CORE_TYPE_LIST_HPP
#define SUPPLE_CORE_TYPE_LIST_HPP

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"

namespace supl::tl {

///////////////////////////////////////////// Type List

/* {{{ doc */
/**
 * @brief A list of types
 */
/* }}} */
template <typename... Pack>
struct type_list { };

///////////////////////////////////////////// type_pair

/* {{{ doc */
/**
 * @brief Type system equivalent to `std::pair`
 *
 * @details Provides inner type aliases `first` and `second`
 */
/* }}} */
template <typename First, typename Second>
struct type_pair {
  using first = First;
  using second = Second;
};

///////////////////////////////////////////// translate

/* {{{ doc */
/**
 * @brief Translate one type list to a different type of type list
 *
 * @details For example:
 * `static_assert(
 * std::is_same_v<
 * supl::tl::translate_t<std::tuple<int, char, bool>, supl::tl::type_list>,
 * supl::tl::type_list<int, char, bool>>)`
 */
/* }}} */
template <typename FROM_LIST, template <typename...> typename TO_LIST>
struct translate;

template <template <typename...> typename FROM_LIST,
          template <typename...>
          typename TO_LIST,
          typename... Pack>
struct translate<FROM_LIST<Pack...>, TO_LIST>
    : type_identity<TO_LIST<Pack...>> { };

template <typename FROM_LIST, template <typename...> typename TO_LIST>
using translate_t = typename translate<FROM_LIST, TO_LIST>::type;

///////////////////////////////////////////// func_wrapper

/* {{{ doc */
/**
 * @brief Wraps a unary metafunction in a concrete type.
 *
 * @details This exists to allow for a `type_list` of metafunctions.
 */
/* }}} */
template <template <typename> typename Func>
struct func_wrapper {
  template <typename T>
  using func = Func<T>;

  template <typename T>
  using func_t = typename func<T>::type;

  template <typename T>
  constexpr inline static decltype(func<T>::value) func_v = func<T>::value;
};

///////////////////////////////////////////// apply_wrapped

template <typename Func, typename... Args>
struct apply_wrapped : Func::template func<Args...> { };

template <typename Func, typename... Args>
using apply_wrapped_t = typename apply_wrapped<Func, Args...>::type;

template <typename Func, typename... Args>
constexpr inline auto apply_wrapped_v =
  apply_wrapped<Func, Args...>::value;

///////////////////////////////////////////// make_list

/* {{{ doc */
/**
 * @brief Creates a type list of the specified type
 */
/* }}} */
template <template <typename...> typename LIST, typename... Pack>
using make_list = type_identity<LIST<Pack...>>;

template <template <typename...> typename LIST, typename... Pack>
using make_list_t = typename make_list<LIST, Pack...>::type;

///////////////////////////////////////////// contains

/* {{{ doc */
/**
 * @brief Determines if type `T` is contained within type list `LIST`
 */
/* }}} */
template <typename T, typename LIST>
struct contains;

template <typename T,
          template <typename...>
          typename LIST,
          typename... Pack>
struct contains<T, LIST<Pack...>> : is_type_in_pack<T, Pack...> { };

template <typename T, typename LIST>
constexpr inline bool contains_v = contains<T, LIST>::value;

///////////////////////////////////////////// size

/* {{{ doc */
/**
 * @brief Computes the size of a type list
 */
/* }}} */
template <typename LIST>
struct size;

template <template <typename...> typename LIST, typename... Pack>
struct size<LIST<Pack...>> : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<LIST<Pack...>&> : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<LIST<Pack...>&&> : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<const LIST<Pack...>&> : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<const LIST<Pack...>&&> : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<volatile LIST<Pack...>&> : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<volatile LIST<Pack...>&&>
    : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<const volatile LIST<Pack...>&>
    : index_constant<sizeof...(Pack)> { };

template <template <typename...> typename LIST, typename... Pack>
struct size<const volatile LIST<Pack...>&&>
    : index_constant<sizeof...(Pack)> { };

template <typename LIST>
constexpr inline std::size_t size_v = size<LIST>::value;

///////////////////////////////////////////// empty

/* {{{ doc */
/**
 * @brief Determines if a type list is empty
 */
/* }}} */
template <typename LIST>
using empty =
  std::conditional_t<size_v<LIST> == 0, std::true_type, std::false_type>;

template <typename LIST>
constexpr inline bool empty_v = empty<LIST>::value;

///////////////////////////////////////////// concat

/* {{{ doc */
/**
 * @brief Concatonates an arbitrary number of type lists
 */
/* }}} */
template <typename... LISTS>
struct concat;

template <template <typename...> typename LIST,
          typename... Pack1,
          typename... Pack2>
struct concat<LIST<Pack1...>, LIST<Pack2...>>
    : type_identity<LIST<Pack1..., Pack2...>> { };

template <typename LIST1,
          typename LIST2,
          typename LIST3,
          typename... LISTS>
struct concat<LIST1, LIST2, LIST3, LISTS...>
    : concat<typename concat<LIST1, LIST2>::type, LIST3, LISTS...> { };

template <typename LIST>
struct concat<LIST> : type_identity<LIST> { };

template <typename... LISTS>
using concat_t = typename concat<LISTS...>::type;

///////////////////////////////////////////// front

/* {{{ doc */
/**
 * @brief Retrieves to first element of a type list
 */
/* }}} */
template <typename LIST>
struct front;

template <template <typename...> typename LIST,
          typename Front,
          typename... Pack>
struct front<LIST<Front, Pack...>> : type_identity<Front> { };

template <typename LIST>
using front_t = typename front<LIST>::type;

///////////////////////////////////////////// at_index

/* {{{ doc */
/**
 * @brief Retrieves type at index `Idx` within type list `LIST`.
 * Result is unspecified if parameter `Current` is specified.
 */
/* }}} */
template <std::size_t Idx, typename LIST, std::size_t Current = 0>
struct at_index;

template <std::size_t Idx,
          template <typename...>
          typename LIST,
          typename T,
          typename... Pack,
          std::size_t Current>
struct at_index<Idx, LIST<T, Pack...>, Current>
    : std::conditional_t<Idx == Current,
                         type_identity<T>,
                         at_index<Idx, LIST<Pack...>, Current + 1>> {
  static_assert(Idx - Current <= sizeof...(Pack), "Index out of bounds");
};

template <std::size_t Idx, typename LIST>
using at_index_t = typename at_index<Idx, LIST>::type;

///////////////////////////////////////////// back

/* {{{ doc */
/**
 * @brief Retrieves the last element of a type list
 */
/* }}} */
template <typename LIST>
using back = at_index<size_v<LIST> - 1, LIST>;

template <typename LIST>
using back_t = typename back<LIST>::type;

///////////////////////////////////////////// push_back

/* {{{ doc */
/**
 * @brief Appends an arbitrary number of types to the end
 * of a type list
 */
/* }}} */
template <typename LIST, typename... T>
struct push_back;

template <template <typename...> typename LIST,
          typename... T,
          typename... Pack>
struct push_back<LIST<Pack...>, T...>
    : type_identity<LIST<Pack..., T...>> { };

template <typename LIST, typename... T>
using push_back_t = typename push_back<LIST, T...>::type;

///////////////////////////////////////////// push_front

/* {{{ doc */
/**
 * @brief Prepends an arbitrary number of types to the beginning
 * of a type list
 */
/* }}} */
template <typename LIST, typename... T>
struct push_front;

template <template <typename...> typename LIST,
          typename... Pack,
          typename... T>
struct push_front<LIST<Pack...>, T...>
    : type_identity<LIST<T..., Pack...>> { };

template <typename LIST, typename... T>
using push_front_t = typename push_front<LIST, T...>::type;

///////////////////////////////////////////// front_n

namespace impl {

  template <template <typename...> typename LIST,
            typename... Pack,
            std::size_t... Idxs>
  auto front_n_impl(LIST<Pack...>, std::index_sequence<Idxs...>)
    -> LIST<at_index_t<Idxs, LIST<Pack...>>...>;

}  // namespace impl

/* {{{ doc */
/**
 * @brief Creates a type list containing the first `N` elements
 * of `LIST`, in the same order
 */
/* }}} */
template <typename LIST, std::size_t N>
struct front_n
    : type_identity<decltype(impl::front_n_impl(
        std::declval<LIST>(),
        std::make_index_sequence<N> {}))> {
  static_assert(N <= size_v<LIST>);
};

template <typename LIST, std::size_t N>
using front_n_t = typename front_n<LIST, N>::type;

///////////////////////////////////////////// back_n

namespace impl {

  template <template <typename...> typename LIST,
            typename... Pack,
            std::size_t... Idxs,
            std::size_t Offset>
  auto back_n_impl(LIST<Pack...>,
                   std::index_sequence<Idxs...>,
                   index_constant<Offset>)
    -> LIST<at_index_t<Idxs + Offset, LIST<Pack...>>...>;

}  // namespace impl

/* {{{ doc */
/**
 * @brief Creates a type list containing the last `N` elements
 * of `LIST`, in the same order
 */
/* }}} */
template <typename LIST, std::size_t N>
struct back_n
    : type_identity<decltype(impl::back_n_impl(
        std::declval<LIST>(),
        std::make_index_sequence<N> {},
        index_constant<size_v<LIST> - N> {}))> {
  static_assert(N <= size_v<LIST>);
};

template <typename LIST, std::size_t N>
using back_n_t = typename back_n<LIST, N>::type;

///////////////////////////////////////////// drop_front_n

/* {{{ doc */
/**
 * @brief Creates a new type list containing all except the first `N`
 * elements of `LIST`, in the same order
 */
/* }}} */
template <typename LIST, std::size_t N>
using drop_front_n = back_n<LIST, size_v<LIST> - N>;

template <typename LIST, std::size_t N>
using drop_front_n_t = typename drop_front_n<LIST, N>::type;

///////////////////////////////////////////// drop_back_n

/* {{{ doc */
/**
 * @brief Creates a new type list containing all except the last `N`
 * elements of `LIST`, in the same order
 */
/* }}} */
template <typename LIST, std::size_t N>
using drop_back_n = front_n<LIST, size_v<LIST> - N>;

template <typename LIST, std::size_t N>
using drop_back_n_t = typename drop_back_n<LIST, N>::type;

///////////////////////////////////////////// pop_back

/* {{{ doc */
/**
 * @brief Removes the last element of a type list
 */
/* }}} */
template <typename LIST>
using pop_back = front_n<LIST, size_v<LIST> - 1>;

template <typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

///////////////////////////////////////////// pop_front

/* {{{ doc */
/**
 * @brief Removes the front element of a type list
 */
/* }}} */
template <typename LIST>
struct pop_front;

template <template <typename...> typename LIST,
          typename Popped,
          typename... Remaining>
struct pop_front<LIST<Popped, Remaining...>>
    : type_identity<LIST<Remaining...>> { };

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

///////////////////////////////////////////// sublist

namespace impl {

  template <template <typename...> typename LIST,
            std::size_t... Idxs,
            std::size_t Begin,
            typename... Pack>
  auto sublist_impl(LIST<Pack...>,
                    std::index_sequence<Idxs...>,
                    index_constant<Begin>)
    -> LIST<at_index_t<Idxs + Begin, LIST<Pack...>>...>;

}  // namespace impl

/* {{{ doc */
/**
 * @brief Creates a type list made up of the elements of `LIST`
 * from the half-open range [Begin, End)
 */
/* }}} */
template <typename LIST, std::size_t Begin, std::size_t End>
struct sublist
    : type_identity<decltype(impl::sublist_impl(
        std::declval<LIST>(),
        std::make_index_sequence<End - Begin> {},
        index_constant<Begin> {}))> {
  static_assert(Begin <= End, "Illegal index range");
  static_assert(Begin <= size_v<LIST>, "Index out of bounds");
  static_assert(End <= size_v<LIST>, "Index out of bounds");
};

template <typename LIST, std::size_t Begin, std::size_t End>
using sublist_t = typename sublist<LIST, Begin, End>::type;

///////////////////////////////////////////// insert

/* {{{ doc */
/**
 * @brief Inserts an arbitrary number of types into an arbitrary
 * index of an existing type list.
 *
 * @tparam Idx The index at which the first inserted element will appear
 */
/* }}} */
template <typename LIST, std::size_t Idx, typename... Inserted>
struct insert;

template <template <typename...> typename LIST,
          std::size_t Idx,
          typename... Inserted,
          typename... Pack>
struct insert<LIST<Pack...>, Idx, Inserted...>
    : concat<
        // [0, Idx - 1) of input
        front_n_t<LIST<Pack...>, Idx>,
        // New elements
        LIST<Inserted...>,
        // [Idx, <END>) of input
        drop_front_n_t<LIST<Pack...>, Idx>> {
  static_assert(Idx <= sizeof...(Pack), "Index out of bounds");
};

template <typename LIST, std::size_t Idx, typename... Inserted>
using insert_t = typename insert<LIST, Idx, Inserted...>::type;

///////////////////////////////////////////// erase

/* {{{ doc */
/**
 * @brief Erases element of `LIST` at index `Idx`
 */
/* }}} */
template <typename LIST, std::size_t Idx>
struct erase;

template <template <typename...> typename LIST,
          std::size_t Idx,
          typename... Pack>
struct erase<LIST<Pack...>, Idx>
    : concat<front_n_t<LIST<Pack...>, Idx>,
             drop_front_n_t<LIST<Pack...>, Idx + 1>> {
  static_assert(Idx <= sizeof...(Pack), "Index out of bounds");
};

template <typename LIST, std::size_t Idx>
using erase_t = typename erase<LIST, Idx>::type;

///////////////////////////////////////////// replace

template <typename LIST, std::size_t Idx, typename T>
struct replace;

template <template <typename...> typename LIST,
          std::size_t Idx,
          typename T,
          typename... Pack>
struct replace<LIST<Pack...>, Idx, T>
    : concat<front_n_t<LIST<Pack...>, Idx>,
             LIST<T>,
             drop_front_n_t<LIST<Pack...>, Idx + 1>> {
  static_assert(Idx <= sizeof...(Pack), "Index out of bounds");
};

template <typename LIST, std::size_t Idx, typename T>
using replace_t = typename replace<LIST, Idx, T>::type;

///////////////////////////////////////////// all_of

/* {{{ doc */
/**
 * @brief Determines if all types in `LIST` satisfy predicate `PRED`
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct all_of;

template <template <typename...> typename LIST,
          typename... Pack,
          template <typename>
          typename PRED>
struct all_of<LIST<Pack...>, PRED>
    : std::bool_constant<(PRED<Pack>::value && ...)> { };

template <template <typename...> typename LIST,
          template <typename>
          typename PRED>
struct all_of<LIST<>, PRED> : std::true_type { };

template <typename LIST, template <typename> typename PRED>
constexpr inline bool all_of_v = all_of<LIST, PRED>::value;

///////////////////////////////////////////// any_of

/* {{{ doc */
/**
 * @brief Determines if any types in `LIST` satisfy predicate `PRED`
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct any_of;

template <template <typename...> typename LIST,
          typename... Pack,
          template <typename>
          typename PRED>
struct any_of<LIST<Pack...>, PRED>
    : std::bool_constant<(PRED<Pack>::value || ...)> { };

template <template <typename...> typename LIST,
          template <typename>
          typename PRED>
struct any_of<LIST<>, PRED> : std::false_type { };

template <typename LIST, template <typename> typename PRED>
constexpr inline bool any_of_v = any_of<LIST, PRED>::value;

///////////////////////////////////////////// none_of

/* {{{ doc */
/**
 * @brief Determines if no types in `LIST` satisfy predicate `PRED`
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
using none_of = std::negation<any_of<LIST, PRED>>;

template <typename LIST, template <typename> typename PRED>
constexpr inline bool none_of_v = none_of<LIST, PRED>::value;

///////////////////////////////////////////// count_if

/* {{{ doc */
/**
 * @brief Counts the number of elements in `LIST` for which `PRED` returns `true`.
 * See tests for examples.
 * 
 * @pre `PRED` must be a unary type metafunction returning `bool`.
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct count_if;

template <template <typename...> typename LIST,
          typename... Pack,
          template <typename>
          typename PRED>
struct count_if<LIST<Pack...>, PRED>
    : index_constant<(static_cast<std::size_t>(PRED<Pack>::value) + ...)> {
};

template <template <typename...> typename LIST,
          template <typename>
          typename PRED>
struct count_if<LIST<>, PRED> : index_constant<0> { };

template <typename LIST, template <typename> typename PRED>
constexpr inline std::size_t count_if_v = count_if<LIST, PRED>::value;

///////////////////////////////////////////// count

/* {{{ doc */
/**
 * @brief Counts the number of times in type `Query` appears in type list `LIST`.
 */
/* }}} */
template <typename LIST, typename Query>
struct count;

template <template <typename...> typename LIST,
          typename... Pack,
          typename Query>
struct count<LIST<Pack...>, Query>
    : index_constant<(static_cast<std::size_t>(std::is_same_v<Pack, Query>)
                      + ...)> { };

template <template <typename...> typename LIST, typename Query>
struct count<LIST<>, Query> : index_constant<0> { };

template <typename LIST, typename Query>
constexpr inline std::size_t count_v = count<LIST, Query>::value;

///////////////////////////////////////////// transform

/* {{{ doc */
/**
 * @brief Produces a type list by applying `PRED` to each element of `LIST`
 *
 * ex. `transform_t<type_list<int, char, bool>, std::add_const>
 * -> type_list<const int, const char, const bool>`
 */
/* }}} */
template <typename LIST, template <typename> typename FUNC>
struct transform;

template <template <typename...> typename LIST,
          template <typename>
          typename FUNC,
          typename... Pack>
struct transform<LIST<Pack...>, FUNC>
    : type_identity<LIST<typename FUNC<Pack>::type...>> { };

template <typename LIST, template <typename> typename FUNC>
using transform_t = typename transform<LIST, FUNC>::type;

///////////////////////////////////////////// rotate_left

/* {{{ doc */
/**
 * @brief Rotates a type list left.
 * In other words, the first element is moved to the end.
 */
/* }}} */
template <typename LIST>
struct rotate_left;

template <template <typename...> typename LIST,
          typename Front,
          typename... Pack>
struct rotate_left<LIST<Front, Pack...>>
    : type_identity<LIST<Pack..., Front>> { };

template <typename LIST>
using rotate_left_t = typename rotate_left<LIST>::type;

///////////////////////////////////////////// rotate_right

/* {{{ doc */
/**
 * @brief Rotates a type list right.
 * In other words, the last element is moved to the beginning.
 */
/* }}} */
template <typename LIST>
struct rotate_right;

template <template <typename...> typename LIST, typename... Pack>
struct rotate_right<LIST<Pack...>>
    : type_identity<
        concat_t<LIST<back_t<LIST<Pack...>>>, pop_back_t<LIST<Pack...>>>> {
};

template <typename LIST>
using rotate_right_t = typename rotate_right<LIST>::type;

///////////////////////////////////////////// reorder

/* {{{ doc */
/**
 * @brief Arbitrarily reorders a type list. Duplication of elements
 * and omitting of elements are permitted.
 *
 * ex. `reorder_t<type_list<int, char, bool, void>, 2, 3, 1, 0, 2, 3, 1, 0>
 * -> type_list<bool, void, char, int, bool, void, char, int>`
 */
/* }}} */
template <typename LIST, std::size_t... Idxs>
struct reorder;

template <template <typename...> typename LIST,
          std::size_t... Idxs,
          typename... Pack>
struct reorder<LIST<Pack...>, Idxs...>
    : type_identity<LIST<at_index_t<Idxs, LIST<Pack...>>...>> {
  static_assert(((Idxs < sizeof...(Pack)) && ...), "Index out of bounds");
};

template <typename LIST, std::size_t... Idxs>
using reorder_t = typename reorder<LIST, Idxs...>::type;

///////////////////////////////////////////// reverse

namespace impl {

  template <typename OLD_LIST, typename NEW_LIST>
  struct reverse_impl;

  template <template <typename...> typename LIST,
            typename Front,
            typename... Old_Pack,
            typename... New_Pack>
  struct reverse_impl<LIST<Front, Old_Pack...>, LIST<New_Pack...>>
      : reverse_impl<LIST<Old_Pack...>, LIST<Front, New_Pack...>> { };

  template <template <typename...> typename LIST, typename... Pack>
  struct reverse_impl<LIST<>, LIST<Pack...>>
      : type_identity<LIST<Pack...>> { };

}  // namespace impl

template <typename LIST>
struct reverse;

template <template <typename...> typename LIST, typename... Pack>
struct reverse<LIST<Pack...>>
    : impl::reverse_impl<LIST<Pack...>, LIST<>> { };

template <template <typename...> typename LIST>
struct reverse<LIST<>> : type_identity<LIST<>> { };

template <typename LIST>
using reverse_t = typename reverse<LIST>::type;

///////////////////////////////////////////// split

/* {{{ doc */
/**
 * @brief Splits a type list into a `tl::type_pair` of two type lists
 *
 * @tparam Idx Index of the element just after the split point
 *
 * ex. `split_t<type_list<int, char, bool, void>, 3>
 * -> tl::type_pair<type_list<int, char, bool>, type_list<void>>`
 */
/* }}} */
template <typename LIST, std::size_t Idx>
struct split
    : type_identity<
        type_pair<front_n_t<LIST, Idx>, drop_front_n_t<LIST, Idx>>> {
  static_assert(Idx <= size_v<LIST>, "Index out of bounds");
};

template <typename LIST, std::size_t Idx>
using split_t = typename split<LIST, Idx>::type;

///////////////////////////////////////////// swap

/* {{{ doc */
/**
 * @brief Swaps type list elements at index `Idx1` and `Idx2`
 */
/* }}} */
template <typename LIST, std::size_t Idx1, std::size_t Idx2>
struct swap
    : concat<
        // Unaltered first subrange
        front_n_t<LIST, std::min(Idx1, Idx2)>,
        // Swap
        reorder_t<LIST, std::max(Idx1, Idx2)>,
        // Unaltered second subrange
        sublist_t<LIST, std::min(Idx1, Idx2) + 1, std::max(Idx1, Idx2)>,
        // Swap
        reorder_t<LIST, std::min(Idx1, Idx2)>,
        // Unaltered third subrange
        drop_front_n_t<LIST, std::max(Idx1, Idx2) + 1>
        // end concat
        > { };

// No-op specialization
template <typename LIST, std::size_t Idx>
struct swap<LIST, Idx, Idx> : type_identity<LIST> { };

template <typename LIST, std::size_t Idx1, std::size_t Idx2>
using swap_t = typename swap<LIST, Idx1, Idx2>::type;

///////////////////////////////////////////// interleave

/* {{{ doc */
/**
 * @brief Interleaves two type lists. New type list will
 * alternate between elements of the first and second arguments.
 *
 * ex. `interleave_t<type_list<int, char, bool>, type_list<void, float, long>>
 * -> type_list<int, void, char, float, bool, long>`
 *
 * @pre The two type lists must be of the same lenth.
 */
/* }}} */
template <typename LIST1, typename LIST2>
struct interleave;

template <template <typename...> typename LIST,
          typename... Pack1,
          typename... Pack2>
struct interleave<LIST<Pack1...>, LIST<Pack2...>>
    : concat<LIST<Pack1, Pack2>...> {
  static_assert(sizeof...(Pack1) == sizeof...(Pack2),
                "Lists must be of the same "
                "length");
};

template <typename LIST1, typename LIST2>
using interleave_t = typename interleave<LIST1, LIST2>::type;

///////////////////////////////////////////// has_duplicates

/* {{{ doc */
/**
 * @brief Determines if a type list contains duplicate types anywhere.
 *
 * ex. `type_list<int, char, bool, int> -> true`
 *
 * ex. `type_list<int, char, bool, const int> -> false`
 *
 * ex. `type_list<int, char, bool> -> false`
 */
/* }}} */
template <typename LIST>
struct has_duplicates;

template <template <typename...> typename LIST,
          typename First,
          typename... Pack>
struct has_duplicates<LIST<First, Pack...>>
    : std::conditional_t<is_type_in_pack_v<First, Pack...>,
                         std::true_type,
                         has_duplicates<LIST<Pack...>>> { };

template <template <typename...> typename LIST,
          typename Almost_Last,
          typename Last>
struct has_duplicates<LIST<Almost_Last, Last>>
    : std::is_same<Almost_Last, Last> { };

// An empty list has no duplicates
template <template <typename...> typename LIST>
struct has_duplicates<LIST<>> : std::false_type { };

template <typename LIST>
constexpr inline bool has_duplicates_v = has_duplicates<LIST>::value;

///////////////////////////////////////////// find

/* {{{ doc */
/**
 * @brief Finds a type in a type list.
 *
 * @details If the sought type is contained in the list,
 * the returned size_t will be the index of that type in the list.
 * If the type is not found, the size of the list will be returned
 * (as the size is the smallest nonnegative illegal index).
 */
/* }}} */
template <typename LIST, typename Sought, std::size_t Idx = 0>
struct find;

template <template <typename...> typename LIST,
          typename Front,
          typename... Pack,
          typename Sought,
          std::size_t Idx>
struct find<LIST<Front, Pack...>, Sought, Idx>
    : std::conditional_t<std::is_same_v<Front, Sought>,
                         index_constant<Idx>,
                         find<LIST<Pack...>, Sought, Idx + 1>> { };

template <template <typename...> typename LIST,
          typename Sought,
          std::size_t Idx>
struct find<LIST<>, Sought, Idx> : index_constant<Idx> { };

template <typename LIST, typename Sought, std::size_t Idx = 0>
constexpr inline std::size_t find_v = find<LIST, Sought, Idx>::value;

///////////////////////////////////////////// deduplicate

namespace impl {

  template <typename LIST, typename REBUILD>
  struct deduplicate_impl;

  template <template <typename...> typename LIST,
            typename LFront,
            typename... LPack,
            typename... Rebuild_Pack>
  struct deduplicate_impl<LIST<LFront, LPack...>, LIST<Rebuild_Pack...>>
      : std::conditional_t<
          is_type_in_pack_v<LFront, Rebuild_Pack...>,
          deduplicate_impl<LIST<LPack...>, LIST<Rebuild_Pack...>>,
          deduplicate_impl<LIST<LPack...>,
                           push_back_t<LIST<Rebuild_Pack...>, LFront>>> {
  };

  // base case- done recursing
  template <template <typename...> typename LIST, typename... Rebuild_Pack>
  struct deduplicate_impl<LIST<>, LIST<Rebuild_Pack...>>
      : type_identity<LIST<Rebuild_Pack...>> { };

}  // namespace impl

/* {{{ doc */
/**
 * @brief Removes duplicate types
 *
 * @details Creates a new list based on the input, with
 * only a single copy of each type in the input list,
 * preserving ordering.
 *
 * ex. `deduplicate_t<type_list<int, char, int, bool, int&, char, char>>
 * -> type_list<int, char, bool, int&>`
 */
/* }}} */
template <typename LIST>
struct deduplicate;

template <template <typename...> typename LIST, typename... Pack>
struct deduplicate<LIST<Pack...>>
    : std::conditional_t<has_duplicates_v<LIST<Pack...>>,
                         impl::deduplicate_impl<LIST<Pack...>, LIST<>>,
                         type_identity<LIST<Pack...>>> { };

template <typename LIST>
using deduplicate_t = typename deduplicate<LIST>::type;

///////////////////////////////////////////// equal

/* {{{ doc */
/**
 * @brief Determine if two type_list-like types contain the same list.
 *
 * @details Can compare two type lists of different templates.
 * ex. `equal_v<type_list<int, char>, std::tuple<int, char>> -> true`
 *
 * If the template deduced as the list type takes a single pack of types
 * as its template argument, then it works here, and can be compared to
 * any other list-like template.
 */
/* }}} */
template <typename LIST1, typename LIST2>
struct equal;

template <template <typename...> typename LIST1,
          template <typename...>
          typename LIST2,
          typename... Pack1,
          typename... Pack2>
struct equal<LIST1<Pack1...>, LIST2<Pack2...>>
    : std::is_same<type_list<Pack1...>, type_list<Pack2...>> { };

template <typename LIST1, typename LIST2>
constexpr inline bool equal_v = equal<LIST1, LIST2>::value;

///////////////////////////////////////////// multi_apply

/* {{{ doc */
/**
 * @brief Applies a series of type -> type metafunctions,
 * and returns a `type_list` of the results of calling each
 * metafunction on `T`
 *
 * ex. `multi_apply_t<int, std::add_const, std::add_lvalue_reference>
 * -> type_list<const int, int&>`
 */
/* }}} */
template <typename T, template <typename> typename... Funcs>
using multi_apply =
  type_identity<tl::type_list<typename Funcs<T>::type...>>;

template <typename T, template <typename> typename... Funcs>
using multi_apply_t = typename multi_apply<T, Funcs...>::type;

///////////////////////////////////////////// enumerate

template <typename T, std::size_t Idx>
struct type_index_pair {
  using type = T;
  constexpr static inline std::size_t value {Idx};
};

namespace impl {
  template <typename LIST, typename Sequence>
  struct enumerate_impl;

  template <template <typename...> typename LIST,
            typename... Pack,
            std::size_t... Idxs>
  struct enumerate_impl<LIST<Pack...>, std::index_sequence<Idxs...>>
      : type_identity<LIST<type_index_pair<Pack, Idxs>...>> { };
}  // namespace impl

/* {{{ doc */
/**
 * @brief Associate each element of a type list with its index
 *
 * @details Example:
 * `enumerate_t<type_list<int, char>>
 * -> type_list<type_index_pair<int, 0>, type_index_pair<char, 1>>`
 */
/* }}} */
template <typename LIST>
using enumerate =
  impl::enumerate_impl<LIST, std::make_index_sequence<size_v<LIST>>>;

template <typename LIST>
using enumerate_t = typename enumerate<LIST>::type;

///////////////////////////////////////////// is_permutation

namespace impl {
  template <typename LIST1, typename LIST2>
  struct is_permutation_impl;

  template <template <typename...> typename LIST1,
            template <typename...>
            typename LIST2,
            typename... Pack1,
            typename... Pack2>
  // clang-format off
  struct is_permutation_impl<LIST1<Pack1...>, LIST2<Pack2...>>
      : std::bool_constant<
      (tl::contains_v<Pack1, LIST2<Pack2...>> && ...)
      && (tl::contains_v<Pack2, LIST1<Pack1...>> && ...)
      && ((tl::count_v<LIST1<Pack1...>, Pack2 > == tl::count_v<LIST2<Pack2...>, Pack2>) && ...)
      && ((tl::count_v<LIST2<Pack2...>, Pack2 > == tl::count_v<LIST1<Pack1...>, Pack2>) && ...)
      > { };

  // clang-format on

}  // namespace impl

/* {{{ doc */
/**
 * @brief Returns `true` if `LIST1` is a permutation of `LIST2`.
 */
/* }}} */
template <typename LIST1, typename LIST2>
struct is_permutation
    : std::conditional_t<
        tl::size_v<LIST1> == tl::size_v<LIST2>,
        std::conditional_t<tl::equal_v<LIST1, LIST2>,
                           std::true_type,
                           impl::is_permutation_impl<LIST1, LIST2>>,
        std::false_type> { };

template <typename LIST1, typename LIST2>
constexpr inline bool is_permutation_v =
  is_permutation<LIST1, LIST2>::value;

///////////////////////////////////////////// is_subset

namespace impl {
  template <typename LIST1, typename LIST2>
  struct is_subset_impl;

  template <template <typename...> typename LIST1,
            template <typename...>
            typename LIST2,
            typename... Pack1,
            typename... Pack2>
  struct is_subset_impl<LIST1<Pack1...>, LIST2<Pack2...>>
      : std::bool_constant<(contains_v<Pack1, LIST2<Pack2...>> && ...)> {
  };

}  // namespace impl

/* {{{ doc */
/**
 * @brief Determines if `LIST1` is a subset of `LIST2`.
 * `LIST1` is a subset of `LIST2` iff any of the below hold:
 * * `LIST1` is empty
 * * They are equal
 * * One is a permutation of the other
 * * Every type appearing in LIST1 appears in LIST2
 *
 * @pre `LIST1` and `LIST2` must not contain duplicate types.
 */
/* }}} */
template <typename LIST1, typename LIST2>
struct is_subset
    // clang-format off
    : std::conditional_t< empty_v<LIST1>,
        std::true_type,
        std::conditional_t< empty_v<LIST2>,
          std::false_type,
          std::conditional_t< tl::equal_v<LIST1,LIST2>,
            std::true_type,
            std::conditional_t< size_v<LIST1> == size_v<LIST2>,
              is_permutation<LIST1,LIST2>,
              impl::is_subset_impl<LIST1,LIST2>
            >
          > 
        >
      >
// clang-format on
{
  static_assert(! has_duplicates_v<LIST1>,
                "Precondition not satisfied, first type list contains "
                "duplicate types");
  static_assert(! has_duplicates_v<LIST2>,
                "Precondition not satisfied, second type list contains "
                "duplicate types");
};

template <typename LIST1, typename LIST2>
constexpr inline bool is_subset_v = is_subset<LIST1, LIST2>::value;

}  // namespace supl::tl

#endif
