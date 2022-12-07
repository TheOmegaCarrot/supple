#ifndef SUPPLEMENTARIES_TYPE_LIST_HPP
#define SUPPLEMENTARIES_TYPE_LIST_HPP

// Still a work in progress

#include <type_traits>

#include "metaprogramming.hpp"

namespace supl::tl {

///////////////////////////////////////////// Type List

template <typename... Pack>
struct type_list {};

///////////////////////////////////////////// contains

template <typename T, typename LIST>
struct contains;

template <typename T, template <typename...> typename LIST,
          typename... Pack>
struct contains<T, LIST<Pack...>> : ::supl::is_type_in_pack<T, Pack...> {};

template <typename T, typename LIST>
constexpr inline bool contains_v = ::supl::tl::contains<T, LIST>::value;

///////////////////////////////////////////// size

template <typename LIST>
struct size;

template <template <typename...> typename LIST, typename... Pack>
struct size<LIST<Pack...>> : ::supl::index_constant<sizeof...(Pack)> {};

template <typename LIST>
constexpr inline std::size_t size_v = ::supl::tl::size<LIST>::value;

///////////////////////////////////////////// empty

template <typename LIST>
struct empty : std::conditional_t<::supl::tl::size_v<LIST> == 0,
                                  std::true_type, std::false_type> {};

template <typename LIST>
constexpr inline bool empty_v = ::supl::tl::empty<LIST>::value;

///////////////////////////////////////////// concat

template <typename LIST1, typename LIST2>
struct concat;

template <template <typename...> typename LIST, typename... Pack1,
          typename... Pack2>
struct concat<LIST<Pack1...>, LIST<Pack2...>>
    : ::supl::type_identity<LIST<Pack1..., Pack2...>> {};

template <typename LIST1, typename LIST2>
using concat_t = typename ::supl::tl::concat<LIST1, LIST2>::type;

///////////////////////////////////////////// front

template <typename LIST>
struct front;

template <template <typename...> typename LIST, typename Front,
          typename... Pack>
struct front<LIST<Front, Pack...>> : ::supl::type_identity<Front> {};

template <typename LIST>
using front_t = typename ::supl::tl::front<LIST>::type;

///////////////////////////////////////////// at_index

template <std::size_t Idx, typename LIST, std::size_t Current = 0>
struct at_index;

template <std::size_t Idx, template <typename...> typename LIST,
          typename T, typename... Pack, std::size_t Current>
struct at_index<Idx, LIST<T, Pack...>, Current>
    : std::conditional_t<
          Idx == Current, ::supl::type_identity<T>,
          ::supl::tl::at_index<Idx, LIST<Pack...>, Current + 1>> {
  static_assert(Idx - Current <= sizeof...(Pack), "Index out of bounds");
};

template <std::size_t Idx, typename LIST>
using at_index_t = typename ::supl::tl::at_index<Idx, LIST>::type;

///////////////////////////////////////////// back

template <typename LIST>
struct back : ::supl::tl::at_index<size_v<LIST> - 1, LIST> {};

template <typename LIST>
using back_t = typename ::supl::tl::back<LIST>::type;

///////////////////////////////////////////// push_back

template <typename LIST, typename... T>
struct push_back;

template <template <typename...> typename LIST, typename... T,
          typename... Pack>
struct push_back<LIST<Pack...>, T...>
    : ::supl::type_identity<LIST<Pack..., T...>> {};

template <typename LIST, typename... T>
using push_back_t = typename ::supl::tl::push_back<LIST, T...>::type;

///////////////////////////////////////////// push_front

template <typename LIST, typename... T>
struct push_front;

template <template <typename...> typename LIST, typename... Pack,
          typename... T>
struct push_front<LIST<Pack...>, T...>
    : ::supl::type_identity<LIST<T..., Pack...>> {};

template <typename LIST, typename... T>
using push_front_t = typename ::supl::tl::push_front<LIST, T...>::type;

///////////////////////////////////////////// front_n

namespace impl {

template <template <typename...> typename LIST, typename... Pack,
          std::size_t... Idxs>
auto front_n_impl(LIST<Pack...>, std::index_sequence<Idxs...>)
    -> LIST<::supl::tl::at_index_t<Idxs, LIST<Pack...>>...>;

} // namespace impl

template <typename LIST, std::size_t N>
struct front_n
    : ::supl::type_identity<decltype(::supl::tl::impl::front_n_impl(
          std::declval<LIST>(), std::make_index_sequence<N> {}))> {};

template <typename LIST, std::size_t N>
using front_n_t = typename ::supl::tl::front_n<LIST, N>::type;

///////////////////////////////////////////// pop_back

template <typename LIST>
struct pop_back : ::supl::tl::front_n<LIST, ::supl::tl::size_v<LIST> - 1> {
};

template <typename LIST>
using pop_back_t = typename ::supl::tl::pop_back<LIST>::type;

///////////////////////////////////////////// pop_front

template <typename LIST>
struct pop_front;

template <template <typename...> typename LIST, typename Popped,
          typename... Remaining>
struct pop_front<LIST<Popped, Remaining...>>
    : ::supl::type_identity<LIST<Remaining...>> {};

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

///////////////////////////////////////////// sublist

namespace impl {

template <template <typename...> typename LIST, std::size_t... Idxs,
          std::size_t Begin, typename... Pack>
auto sublist_impl(LIST<Pack...>, std::index_sequence<Idxs...>,
                  ::supl::index_constant<Begin>)
    -> LIST<::supl::tl::at_index_t<Idxs + Begin, LIST<Pack...>>...>;

} // namespace impl

template <typename LIST, std::size_t Begin, std::size_t End>
struct sublist
    : ::supl::type_identity<decltype(::supl::tl::impl::sublist_impl(
          std::declval<LIST>(), std::make_index_sequence<End - Begin> {},
          ::supl::index_constant<Begin> {}))> {
  static_assert(Begin <= End, "Illegal index range");
  static_assert(Begin <= ::supl::tl::size_v<LIST>, "Index out of bounds");
  static_assert(End <= ::supl::tl::size_v<LIST>, "Index out of bounds");
};

template <typename LIST, std::size_t Begin, std::size_t End>
using sublist_t = typename ::supl::tl::sublist<LIST, Begin, End>::type;

///////////////////////////////////////////// insert

///////////////////////////////////////////// erase

///////////////////////////////////////////// all_of

template <typename LIST, template <typename> typename PRED>
struct all_of
    : std::conditional_t<
          PRED<::supl::tl::front_t<LIST>>::value,
          ::supl::tl::all_of<::supl::tl::pop_front_t<LIST>, PRED>,
          std::false_type> {};

template <template <typename...> typename LIST,
          template <typename> typename PRED>
struct all_of<LIST<>, PRED> : std::true_type {};

template <typename LIST, template <typename> typename PRED>
constexpr inline bool all_of_v = ::supl::tl::all_of<LIST, PRED>::value;

///////////////////////////////////////////// any_of

template <typename LIST, template <typename> typename PRED>
struct any_of
    : std::conditional_t<
          PRED<::supl::tl::front_t<LIST>>::value, std::true_type,
          ::supl::tl::any_of<::supl::tl::pop_front_t<LIST>, PRED>> {};

template <template <typename...> typename LIST,
          template <typename> typename PRED>
struct any_of<LIST<>, PRED> : std::false_type {};

template <typename LIST, template <typename> typename PRED>
constexpr inline bool any_of_v = ::supl::tl::any_of<LIST, PRED>::value;

///////////////////////////////////////////// none_of

} // namespace supl::tl

#endif
