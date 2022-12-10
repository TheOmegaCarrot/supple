#ifndef SUPPLEMENTARIES_TYPE_LIST_HPP
#define SUPPLEMENTARIES_TYPE_LIST_HPP

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
struct type_list {};

///////////////////////////////////////////// make_list

/* {{{ doc */
/**
 * @brief Creates a type list of the specified type
 */
/* }}} */
template <template <typename...> typename LIST, typename... Pack>
struct make_list : type_identity<LIST<Pack...>> {};

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

template <typename T, template <typename...> typename LIST,
          typename... Pack>
struct contains<T, LIST<Pack...>> : is_type_in_pack<T, Pack...> {};

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
struct size<LIST<Pack...>> : index_constant<sizeof...(Pack)> {};

template <typename LIST>
constexpr inline std::size_t size_v = size<LIST>::value;

///////////////////////////////////////////// empty

/* {{{ doc */
/**
 * @brief Determines if a type list is empty
 */
/* }}} */
template <typename LIST>
struct empty : std::conditional_t<size_v<LIST> == 0, std::true_type,
                                  std::false_type> {};

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

template <template <typename...> typename LIST, typename... Pack1,
          typename... Pack2>
struct concat<LIST<Pack1...>, LIST<Pack2...>>
    : type_identity<LIST<Pack1..., Pack2...>> {};

template <typename LIST1, typename LIST2, typename LIST3,
          typename... LISTS>
struct concat<LIST1, LIST2, LIST3, LISTS...>
    : concat<typename concat<LIST1, LIST2>::type, LIST3, LISTS...> {};

template <typename LIST>
struct concat<LIST> : type_identity<LIST> {};

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

template <template <typename...> typename LIST, typename Front,
          typename... Pack>
struct front<LIST<Front, Pack...>> : type_identity<Front> {};

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

template <std::size_t Idx, template <typename...> typename LIST,
          typename T, typename... Pack, std::size_t Current>
struct at_index<Idx, LIST<T, Pack...>, Current>
    : std::conditional_t<Idx == Current, type_identity<T>,
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
struct back : at_index<size_v<LIST> - 1, LIST> {};

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

template <template <typename...> typename LIST, typename... T,
          typename... Pack>
struct push_back<LIST<Pack...>, T...>
    : type_identity<LIST<Pack..., T...>> {};

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

template <template <typename...> typename LIST, typename... Pack,
          typename... T>
struct push_front<LIST<Pack...>, T...>
    : type_identity<LIST<T..., Pack...>> {};

template <typename LIST, typename... T>
using push_front_t = typename push_front<LIST, T...>::type;

///////////////////////////////////////////// front_n

namespace impl {

template <template <typename...> typename LIST, typename... Pack,
          std::size_t... Idxs>
auto front_n_impl(LIST<Pack...>, std::index_sequence<Idxs...>)
    -> LIST<at_index_t<Idxs, LIST<Pack...>>...>;

} // namespace impl

/* {{{ doc */
/**
 * @brief Creates a type list containing the first `N` elements
 * of `LIST`, in the same order
 */
/* }}} */
template <typename LIST, std::size_t N>
struct front_n
    : type_identity<decltype(impl::front_n_impl(
          std::declval<LIST>(), std::make_index_sequence<N> {}))> {
  static_assert(N <= size_v<LIST>);
};

template <typename LIST, std::size_t N>
using front_n_t = typename front_n<LIST, N>::type;

///////////////////////////////////////////// back_n

namespace impl {

template <template <typename...> typename LIST, typename... Pack,
          std::size_t... Idxs, std::size_t Offset>
auto back_n_impl(LIST<Pack...>, std::index_sequence<Idxs...>,
                 index_constant<Offset>)
    -> LIST<at_index_t<Idxs + Offset, LIST<Pack...>>...>;

} // namespace impl

/* {{{ doc */
/**
 * @brief Creates a type list containing the last `N` elements
 * of `LIST`, in the same order
 */
/* }}} */
template <typename LIST, std::size_t N>
struct back_n : type_identity<decltype(impl::back_n_impl(
                    std::declval<LIST>(), std::make_index_sequence<N> {},
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
struct drop_front_n : back_n<LIST, size_v<LIST> - N> {};

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
struct drop_back_n : front_n<LIST, size_v<LIST> - N> {};

template <typename LIST, std::size_t N>
using drop_back_n_t = typename drop_back_n<LIST, N>::type;

///////////////////////////////////////////// pop_back

/* {{{ doc */
/**
 * @brief Removes the last element of a type list
 */
/* }}} */
template <typename LIST>
struct pop_back : front_n<LIST, size_v<LIST> - 1> {};

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

template <template <typename...> typename LIST, typename Popped,
          typename... Remaining>
struct pop_front<LIST<Popped, Remaining...>>
    : type_identity<LIST<Remaining...>> {};

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

///////////////////////////////////////////// sublist

namespace impl {

template <template <typename...> typename LIST, std::size_t... Idxs,
          std::size_t Begin, typename... Pack>
auto sublist_impl(LIST<Pack...>, std::index_sequence<Idxs...>,
                  index_constant<Begin>)
    -> LIST<at_index_t<Idxs + Begin, LIST<Pack...>>...>;

} // namespace impl

/* {{{ doc */
/**
 * @brief Creates a type list made up of the elements of `LIST`
 * from the half-open range [Begin, End)
 */
/* }}} */
template <typename LIST, std::size_t Begin, std::size_t End>
struct sublist
    : type_identity<decltype(impl::sublist_impl(
          std::declval<LIST>(), std::make_index_sequence<End - Begin> {},
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

template <template <typename...> typename LIST, std::size_t Idx,
          typename... Inserted, typename... Pack>
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

template <template <typename...> typename LIST, std::size_t Idx,
          typename... Pack>
struct erase<LIST<Pack...>, Idx>
    : concat<front_n_t<LIST<Pack...>, Idx>,
             drop_front_n_t<LIST<Pack...>, Idx + 1>> {};

template <typename LIST, std::size_t Idx>
using erase_t = typename erase<LIST, Idx>::type;

///////////////////////////////////////////// all_of

/* {{{ doc */
/**
 * @brief Determines if all types in `LIST` satisfy predicate `PRED`
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct all_of : std::conditional_t<PRED<front_t<LIST>>::value,
                                   all_of<pop_front_t<LIST>, PRED>,
                                   std::false_type> {};

template <template <typename...> typename LIST,
          template <typename> typename PRED>
struct all_of<LIST<>, PRED> : std::true_type {};

template <typename LIST, template <typename> typename PRED>
constexpr inline bool all_of_v = all_of<LIST, PRED>::value;

///////////////////////////////////////////// any_of

/* {{{ doc */
/**
 * @brief Determines if any types in `LIST` satisfy predicate `PRED`
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct any_of
    : std::conditional_t<PRED<front_t<LIST>>::value, std::true_type,
                         any_of<pop_front_t<LIST>, PRED>> {};

template <template <typename...> typename LIST,
          template <typename> typename PRED>
struct any_of<LIST<>, PRED> : std::false_type {};

template <typename LIST, template <typename> typename PRED>
constexpr inline bool any_of_v = any_of<LIST, PRED>::value;

///////////////////////////////////////////// none_of

/* {{{ doc */
/**
 * @brief Determines if no types in `LIST` satisfy predicate `PRED`
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct none_of
    : std::conditional_t<PRED<front_t<LIST>>::value, std::false_type,
                         none_of<pop_front_t<LIST>, PRED>> {};

template <template <typename...> typename LIST,
          template <typename> typename PRED>
struct none_of<LIST<>, PRED> : std::true_type {};

template <typename LIST, template <typename> typename PRED>
constexpr inline bool none_of_v = none_of<LIST, PRED>::value;

///////////////////////////////////////////// transform

/* {{{ doc */
/**
 * @brief Produces a type list by applying `PRED` to each element of `LIST`
 *
 * ex. transform_t<type_list<int, char, bool>, std::add_const>
 * -> type_list<const int, const char, const bool>
 */
/* }}} */
template <typename LIST, template <typename> typename PRED>
struct transform;

template <template <typename...> typename LIST,
          template <typename> typename PRED, typename... Pack>
struct transform<LIST<Pack...>, PRED>
    : type_identity<LIST<typename PRED<Pack>::type...>> {};

template <typename LIST, template <typename> typename PRED>
using transform_t = typename transform<LIST, PRED>::type;

///////////////////////////////////////////// rotate_left

/* {{{ doc */
/**
 * @brief Rotates a type list left.
 * In other words, the first element is moved to the end.
 */
/* }}} */
template <typename LIST>
struct rotate_left;

template <template <typename...> typename LIST, typename Front,
          typename... Pack>
struct rotate_left<LIST<Front, Pack...>>
    : type_identity<LIST<Pack..., Front>> {};

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
    : type_identity<concat_t<LIST<back_t<LIST<Pack...>>>,
                             pop_back_t<LIST<Pack...>>>> {};

template <typename LIST>
using rotate_right_t = typename rotate_right<LIST>::type;

///////////////////////////////////////////// reorder

/* {{{ doc */
/**
 * @brief Arbitrarily reorders a type list. Duplication of elements
 * and omitting of elements are permitted.
 *
 * ex. reorder_t<type_list<int, char, bool, void>, 2, 3, 1, 0, 2, 3, 1, 0>
 * -> type_list<bool, void, char, int, bool, void, char, int>
 */
/* }}} */
template <typename LIST, std::size_t... Idxs>
struct reorder;

template <template <typename...> typename LIST, std::size_t... Idxs,
          typename... Pack>
struct reorder<LIST<Pack...>, Idxs...>
    : type_identity<LIST<at_index_t<Idxs, LIST<Pack...>>...>> {
  static_assert(((Idxs < sizeof...(Pack)) && ...), "Index out of bounds");
};

template <typename LIST, std::size_t... Idxs>
using reorder_t = typename reorder<LIST, Idxs...>::type;

///////////////////////////////////////////// split

/* {{{ doc */
/**
 * @brief Splits a type list into a `std::pair` of two type lists
 *
 * @tparam Idx Index of the element just after the split point
 *
 * ex. split_t<type_list<int, char, bool, void>, 3>
 * -> std::pair<type_list<int, char, bool>, type_list<void>>
 */
/* }}} */
template <typename LIST, std::size_t Idx>
struct split
    : type_identity<
          std::pair<front_n_t<LIST, Idx>, drop_front_n_t<LIST, Idx>>> {
  static_assert(Idx <= size_v<LIST>, "Index out of bounds");
};

template <typename LIST, std::size_t Idx>
using split_t = typename split<LIST, Idx>::type;

///////////////////////////////////////////// swap

// Not yet working

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
          > {};

// No-op specialization
template <typename LIST, std::size_t Idx>
struct swap<LIST, Idx, Idx> : type_identity<LIST> {};

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

template <template <typename...> typename LIST, typename... Pack1,
          typename... Pack2>
struct interleave<LIST<Pack1...>, LIST<Pack2...>>
    : concat<LIST<Pack1, Pack2>...> {
  static_assert(sizeof...(Pack1) == sizeof...(Pack2),
                "Lists must be of the same length");
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

template <template <typename...> typename LIST, typename First,
          typename... Pack>
struct has_duplicates<LIST<First, Pack...>>
    : std::conditional_t<is_type_in_pack_v<First, Pack...>, std::true_type,
                         has_duplicates<LIST<Pack...>>> {};

template <template <typename...> typename LIST, typename Almost_Last,
          typename Last>
struct has_duplicates<LIST<Almost_Last, Last>>
    : std::is_same<Almost_Last, Last> {};

template <typename LIST>
constexpr inline bool has_duplicates_v = has_duplicates<LIST>::value;

} // namespace supl::tl

#endif
