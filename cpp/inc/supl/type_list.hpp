#ifndef SUPPLEMENTARIES_TYPE_LIST_HPP
#define SUPPLEMENTARIES_TYPE_LIST_HPP

#include <type_traits>

#include "metaprogramming.hpp"

namespace supl::tl {

///////////////////////////////////////////// Type List

template <typename... Pack>
struct type_list {};

///////////////////////////////////////////// type pair

template <typename First, typename Second>
struct type_pair {
  using first  = First;
  using second = Second;
};

///////////////////////////////////////////// contains

template <typename T, typename LIST>
struct contains;

template <typename T, template <typename...> typename LIST,
          typename... Pack>
struct contains<T, LIST<Pack...>> : is_type_in_pack<T, Pack...> {};

template <typename T, typename LIST>
constexpr inline bool contains_v = contains<T, LIST>::value;

///////////////////////////////////////////// size

template <typename LIST>
struct size;

template <template <typename...> typename LIST, typename... Pack>
struct size<LIST<Pack...>> : index_constant<sizeof...(Pack)> {};

template <typename LIST>
constexpr inline std::size_t size_v = size<LIST>::value;

///////////////////////////////////////////// empty

template <typename LIST>
struct empty : std::conditional_t<size_v<LIST> == 0, std::true_type,
                                  std::false_type> {};

template <typename LIST>
constexpr inline bool empty_v = empty<LIST>::value;

///////////////////////////////////////////// concat

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

template <typename LIST>
struct front;

template <template <typename...> typename LIST, typename Front,
          typename... Pack>
struct front<LIST<Front, Pack...>> : type_identity<Front> {};

template <typename LIST>
using front_t = typename front<LIST>::type;

///////////////////////////////////////////// at_index

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

template <typename LIST>
struct back : at_index<size_v<LIST> - 1, LIST> {};

template <typename LIST>
using back_t = typename back<LIST>::type;

///////////////////////////////////////////// push_back

template <typename LIST, typename... T>
struct push_back;

template <template <typename...> typename LIST, typename... T,
          typename... Pack>
struct push_back<LIST<Pack...>, T...>
    : type_identity<LIST<Pack..., T...>> {};

template <typename LIST, typename... T>
using push_back_t = typename push_back<LIST, T...>::type;

///////////////////////////////////////////// push_front

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

template <typename LIST, std::size_t N>
struct back_n : type_identity<decltype(impl::back_n_impl(
                    std::declval<LIST>(), std::make_index_sequence<N> {},
                    index_constant<size_v<LIST> - N> {}))> {
  static_assert(N <= size_v<LIST>);
};

template <typename LIST, std::size_t N>
using back_n_t = typename back_n<LIST, N>::type;

///////////////////////////////////////////// drop_front_n

template <typename LIST, std::size_t N>
struct drop_front_n : back_n<LIST, size_v<LIST> - N> {};

template <typename LIST, std::size_t N>
using drop_front_n_t = typename drop_front_n<LIST, N>::type;

///////////////////////////////////////////// drop_back_n

template <typename LIST, std::size_t N>
struct drop_back_n : front_n<LIST, size_v<LIST> - N> {};

template <typename LIST, std::size_t N>
using drop_back_n_t = typename drop_back_n<LIST, N>::type;

///////////////////////////////////////////// pop_back

template <typename LIST>
struct pop_back : front_n<LIST, size_v<LIST> - 1> {};

template <typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

///////////////////////////////////////////// pop_front

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

///////////////////////////////////////////// erase

///////////////////////////////////////////// all_of

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

template <typename LIST, template <typename> typename PRED>
struct transform;

template <template <typename...> typename LIST,
          template <typename> typename PRED, typename... Pack>
struct transform<LIST<Pack...>, PRED>
    : type_identity<LIST<typename PRED<Pack>::type...>> {};

template <typename LIST, template <typename> typename PRED>
using transform_t = typename transform<LIST, PRED>::type;

///////////////////////////////////////////// rotate_left

template <typename LIST>
struct rotate_left;

template <template <typename...> typename LIST, typename Front,
          typename... Pack>
struct rotate_left<LIST<Front, Pack...>>
    : type_identity<LIST<Pack..., Front>> {};

template <typename LIST>
using rotate_left_t = typename rotate_left<LIST>::type;

///////////////////////////////////////////// rotate_right

template <typename LIST>
struct rotate_right;

template <template <typename...> typename LIST, typename... Pack>
struct rotate_right<LIST<Pack...>>
    : type_identity<concat_t<LIST<back_t<LIST<Pack...>>>,
                             pop_back_t<LIST<Pack...>>>> {};

template <typename LIST>
using rotate_right_t = typename rotate_right<LIST>::type;

///////////////////////////////////////////// reorder

template <typename LIST, std::size_t... Idxs>
struct reorder;

template <template <typename...> typename LIST, std::size_t... Idxs,
          typename... Pack>
struct reorder<LIST<Pack...>, Idxs...>
    : type_identity<LIST<at_index_t<Idxs, LIST<Pack...>>...>> {
  static_assert(((Idxs < size_v<LIST<Pack...>>)&&...),
                "Index out of bounds");
};

template <typename LIST, std::size_t... Idxs>
using reorder_t = typename reorder<LIST, Idxs...>::type;

///////////////////////////////////////////// split

template <typename LIST, std::size_t Idx>
struct split
    : type_identity<
          type_pair<front_n_t<LIST, Idx>, drop_front_n_t<LIST, Idx>>> {
  static_assert(Idx <= size_v<LIST>, "Index out of bounds");
};

template <typename LIST, std::size_t Idx>
using split_t = typename split<LIST, Idx>::type;

} // namespace supl::tl

#endif
