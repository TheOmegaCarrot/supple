#include <cstddef>
#include <tuple>
#include <type_traits>
#include <variant>

#include "supl/type_list.hpp"

///////////////////////////////////////////// translate

static_assert(
  std::is_same_v<supl::tl::translate_t<std::tuple<int, char, bool>,
                                       supl::tl::type_list>,
                 supl::tl::type_list<int, char, bool>>);

static_assert(
  std::is_same_v<supl::tl::translate_t<std::variant<int, char, bool>,
                                       supl::tl::type_list>,
                 supl::tl::type_list<int, char, bool>>);

static_assert(std::is_same_v<
              supl::tl::translate_t<supl::tl::type_list<int, char, bool>,
                                    supl::tl::type_list>,
              supl::tl::type_list<int, char, bool>>);

static_assert(std::is_same_v<
              supl::tl::translate_t<supl::tl::type_list<int, char, bool>,
                                    std::tuple>,
              std::tuple<int, char, bool>>);

static_assert(std::is_same_v<
              supl::tl::translate_t<std::tuple<int, const char*, bool&&>,
                                    supl::tl::type_list>,
              supl::tl::type_list<int, const char*, bool&&>>);

///////////////////////////////////////////// func_wrapper

using func_wrapper_value_test = supl::tl::func_wrapper<std::is_const>;
using func_wrapper_type_test = supl::tl::func_wrapper<std::add_const>;

static_assert(not func_wrapper_value_test::template func<int>::value);

static_assert(func_wrapper_value_test::template func<const int>::value);

static_assert(not func_wrapper_value_test::template func_v<int>);

static_assert(func_wrapper_value_test::template func_v<const int>);

static_assert(
  std::is_same_v<typename func_wrapper_type_test::func<int>::type,
                 const int>);

static_assert(
  std::is_same_v<func_wrapper_type_test::func_t<int>, const int>);

///////////////////////////////////////////// apply_wrapped

static_assert(
  not supl::tl::apply_wrapped<func_wrapper_value_test, int>::value);

static_assert(
  supl::tl::apply_wrapped<func_wrapper_value_test, const int>::value);

static_assert(not supl::tl::apply_wrapped_v<func_wrapper_value_test, int>);

static_assert(
  supl::tl::apply_wrapped_v<func_wrapper_value_test, const int>);

static_assert(
  std::is_same_v<typename func_wrapper_type_test::func<int>::type,
                 const int>);

static_assert(
  std::is_same_v<
    typename supl::tl::apply_wrapped<func_wrapper_type_test, int>::type,
    const int>);

static_assert(
  std::is_same_v<supl::tl::apply_wrapped_t<func_wrapper_type_test, int>,
                 const int>);

///////////////////////////////////////////// make_list

static_assert(
  std::is_same_v<
    supl::tl::make_list_t<supl::tl::type_list,
                          int,
                          char,
                          bool,
                          void*,
                          const int,
                          std::nullptr_t>,
    supl::tl::
      type_list<int, char, bool, void*, const int, std::nullptr_t>>);

///////////////////////////////////////////// contains

static_assert(
  supl::tl::contains_v<int, supl::tl::type_list<int, char, bool>>);

static_assert(
  supl::tl::contains_v<int, supl::tl::type_list<char, bool, int>>);

static_assert(
  supl::tl::contains_v<int, supl::tl::type_list<char, int, bool>>);

static_assert(
  not supl::tl::contains_v<int, supl::tl::type_list<char, bool, float>>);

static_assert(supl::tl::contains_v<int, std::tuple<int, char, bool>>);

static_assert(supl::tl::contains_v<int, std::tuple<char, bool, int>>);

static_assert(supl::tl::contains_v<int, std::tuple<char, int, bool>>);

static_assert(
  not supl::tl::contains_v<int, std::tuple<char, bool, float>>);

///////////////////////////////////////////// size

static_assert(supl::tl::size_v<supl::tl::type_list<>> == 0);

static_assert(
  supl::tl::size_v<supl::tl::type_list<bool, int, char, float, bool>>
  == 5);

static_assert(supl::tl::size_v<std::tuple<bool, int, char, float, bool>>
              == 5);

///////////////////////////////////////////// empty

static_assert(supl::tl::empty_v<supl::tl::type_list<>>);

static_assert(not supl::tl::empty_v<supl::tl::type_list<int>>);

static_assert(not supl::tl::empty_v<supl::tl::type_list<int, bool, char>>);

///////////////////////////////////////////// concat

static_assert(std::is_same_v<
              supl::tl::concat_t<supl::tl::type_list<int, bool, char>,
                                 supl::tl::type_list<float, std::size_t>>,
              supl::tl::type_list<int, bool, char, float, std::size_t>>);

static_assert(
  std::is_same_v<supl::tl::concat_t<supl::tl::type_list<int, bool, char>,
                                    supl::tl::type_list<>>,
                 supl::tl::type_list<int, bool, char>>);

static_assert(
  std::is_same_v<supl::tl::concat_t<supl::tl::type_list<>,
                                    supl::tl::type_list<int, bool, char>>,
                 supl::tl::type_list<int, bool, char>>);

static_assert(std::is_same_v<supl::tl::concat_t<supl::tl::type_list<>,
                                                supl::tl::type_list<>>,
                             supl::tl::type_list<>>);

static_assert(
  std::is_same_v<
    supl::tl::concat_t<supl::tl::type_list<int, char>,
                       supl::tl::type_list<bool, float>,
                       supl::tl::type_list<unsigned, void>>,
    supl::tl::type_list<int, char, bool, float, unsigned, void>>);

static_assert(
  std::is_same_v<supl::tl::concat_t<
                   supl::tl::type_list<int, char>,
                   supl::tl::type_list<bool, float>,
                   supl::tl::type_list<unsigned, void>,
                   supl::tl::type_list<const int, int&&>,
                   supl::tl::type_list<const volatile void*, const bool>,
                   supl::tl::type_list<void, void>,
                   supl::tl::type_list<double, std::nullptr_t>>,
                 supl::tl::type_list<int,
                                     char,
                                     bool,
                                     float,
                                     unsigned,
                                     void,
                                     const int,
                                     int&&,
                                     const volatile void*,
                                     const bool,
                                     void,
                                     void,
                                     double,
                                     std::nullptr_t>>);

static_assert(
  std::is_same_v<supl::tl::concat_t<
                   supl::tl::type_list<int, char>,
                   supl::tl::type_list<bool, float>,
                   supl::tl::type_list<>,
                   supl::tl::type_list<const int, int&&>,
                   supl::tl::type_list<const volatile void*, const bool>,
                   supl::tl::type_list<>,
                   supl::tl::type_list<double, std::nullptr_t>>,
                 supl::tl::type_list<int,
                                     char,
                                     bool,
                                     float,
                                     const int,
                                     int&&,
                                     const volatile void*,
                                     const bool,
                                     double,
                                     std::nullptr_t>>);

static_assert(
  std::is_same_v<supl::tl::concat_t<supl::tl::type_list<int, char, bool>>,
                 supl::tl::type_list<int, char, bool>>);

///////////////////////////////////////////// front

static_assert(
  std::is_same_v<supl::tl::front_t<supl::tl::type_list<int, bool, char>>,
                 int>);

static_assert(
  std::is_same_v<supl::tl::front_t<supl::tl::type_list<int>>, int>);

static_assert(
  std::is_same_v<supl::tl::front_t<std::tuple<int, bool, char>>, int>);

static_assert(std::is_same_v<supl::tl::front_t<std::tuple<int>>, int>);

///////////////////////////////////////////// back

static_assert(
  std::is_same_v<supl::tl::back_t<supl::tl::type_list<int, bool, char>>,
                 char>);

static_assert(
  std::is_same_v<supl::tl::back_t<supl::tl::type_list<int>>, int>);

static_assert(
  std::is_same_v<supl::tl::back_t<std::tuple<int, bool, char>>, char>);

static_assert(std::is_same_v<supl::tl::back_t<std::tuple<int>>, int>);

///////////////////////////////////////////// at_index

static_assert(
  std::is_same_v<
    supl::tl::at_index_t<0, supl::tl::type_list<int, char, bool>>,
    int>);

static_assert(
  std::is_same_v<
    supl::tl::at_index_t<2, supl::tl::type_list<int, char, bool>>,
    bool>);

static_assert(
  std::is_same_v<
    supl::tl::at_index_t<1, supl::tl::type_list<int, char, bool>>,
    char>);

static_assert(
  std::is_same_v<supl::tl::at_index_t<0, std::tuple<int, char, bool>>,
                 int>);

static_assert(
  std::is_same_v<supl::tl::at_index_t<2, std::tuple<int, char, bool>>,
                 bool>);

static_assert(
  std::is_same_v<supl::tl::at_index_t<1, std::tuple<int, char, bool>>,
                 char>);

///////////////////////////////////////////// push_back

static_assert(
  std::is_same_v<
    supl::tl::type_list<int, char, bool>,
    supl::tl::push_back_t<supl::tl::type_list<int, char>, bool>>);

static_assert(
  std::is_same_v<supl::tl::type_list<bool>,
                 supl::tl::push_back_t<supl::tl::type_list<>, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::type_list<int, char, bool, float, double>,
    supl::tl::
      push_back_t<supl::tl::type_list<int, char>, bool, float, double>>);

static_assert(
  std::is_same_v<
    supl::tl::type_list<bool, float, double>,
    supl::tl::push_back_t<supl::tl::type_list<>, bool, float, double>>);

static_assert(
  std::is_same_v<
    supl::tl::type_list<int, char, bool>,
    supl::tl::push_back_t<supl::tl::type_list<>, int, char, bool>>);

static_assert(
  std::is_same_v<std::tuple<int, char, bool>,
                 supl::tl::push_back_t<std::tuple<int, char>, bool>>);

static_assert(
  std::is_same_v<
    std::tuple<int, char, bool, float, double>,
    supl::tl::push_back_t<std::tuple<int, char>, bool, float, double>>);

static_assert(std::is_same_v<
              std::tuple<bool, float, double>,
              supl::tl::push_back_t<std::tuple<>, bool, float, double>>);

///////////////////////////////////////////// push_front

static_assert(
  std::is_same_v<
    supl::tl::type_list<bool, int, char>,
    supl::tl::push_front_t<supl::tl::type_list<int, char>, bool>>);

static_assert(
  std::is_same_v<supl::tl::type_list<bool>,
                 supl::tl::push_front_t<supl::tl::type_list<>, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::type_list<bool, float, double, int, char>,
    supl::tl::
      push_front_t<supl::tl::type_list<int, char>, bool, float, double>>);

static_assert(
  std::is_same_v<
    supl::tl::type_list<bool, float, double>,
    supl::tl::push_front_t<supl::tl::type_list<>, bool, float, double>>);

static_assert(
  std::is_same_v<std::tuple<bool, int, char>,
                 supl::tl::push_front_t<std::tuple<int, char>, bool>>);

static_assert(std::is_same_v<std::tuple<bool>,
                             supl::tl::push_front_t<std::tuple<>, bool>>);

static_assert(
  std::is_same_v<
    std::tuple<bool, float, double, int, char>,
    supl::tl::push_front_t<std::tuple<int, char>, bool, float, double>>);

static_assert(std::is_same_v<
              std::tuple<bool, float, double>,
              supl::tl::push_front_t<std::tuple<>, bool, float, double>>);

///////////////////////////////////////////// front_n

static_assert(std::is_same_v<
              supl::tl::front_n_t<supl::tl::type_list<bool, int, char>, 2>,
              supl::tl::type_list<bool, int>>);

static_assert(std::is_same_v<
              supl::tl::front_n_t<supl::tl::type_list<bool, int, char>, 1>,
              supl::tl::type_list<bool>>);

static_assert(std::is_same_v<
              supl::tl::front_n_t<supl::tl::type_list<bool, int, char>, 3>,
              supl::tl::type_list<bool, int, char>>);

static_assert(std::is_same_v<
              supl::tl::front_n_t<supl::tl::type_list<bool, int, char>, 0>,
              supl::tl::type_list<>>);

static_assert(
  std::is_same_v<supl::tl::front_n_t<std::tuple<bool, int, char>, 2>,
                 std::tuple<bool, int>>);

static_assert(
  std::is_same_v<supl::tl::front_n_t<std::tuple<bool, int, char>, 3>,
                 std::tuple<bool, int, char>>);

static_assert(
  std::is_same_v<supl::tl::front_n_t<std::tuple<bool, int, char>, 0>,
                 std::tuple<>>);

///////////////////////////////////////////// back_n

static_assert(std::is_same_v<
              supl::tl::back_n_t<supl::tl::type_list<bool, int, char>, 2>,
              supl::tl::type_list<int, char>>);

static_assert(std::is_same_v<
              supl::tl::back_n_t<supl::tl::type_list<bool, int, char>, 1>,
              supl::tl::type_list<char>>);

static_assert(std::is_same_v<
              supl::tl::back_n_t<supl::tl::type_list<bool, int, char>, 3>,
              supl::tl::type_list<bool, int, char>>);

static_assert(std::is_same_v<
              supl::tl::back_n_t<supl::tl::type_list<bool, int, char>, 0>,
              supl::tl::type_list<>>);

///////////////////////////////////////////// drop_front_n

static_assert(
  std::is_same_v<
    supl::tl::drop_front_n_t<supl::tl::type_list<bool, int, char>, 2>,
    supl::tl::type_list<char>>);

static_assert(
  std::is_same_v<
    supl::tl::drop_front_n_t<supl::tl::type_list<bool, int, char>, 1>,
    supl::tl::type_list<int, char>>);

static_assert(
  std::is_same_v<
    supl::tl::drop_front_n_t<supl::tl::type_list<bool, int, char>, 3>,
    supl::tl::type_list<>>);

static_assert(
  std::is_same_v<
    supl::tl::drop_front_n_t<supl::tl::type_list<bool, int, char>, 0>,
    supl::tl::type_list<bool, int, char>>);

///////////////////////////////////////////// drop_back_n

static_assert(
  std::is_same_v<
    supl::tl::drop_back_n_t<supl::tl::type_list<bool, int, char>, 2>,
    supl::tl::type_list<bool>>);

static_assert(
  std::is_same_v<
    supl::tl::drop_back_n_t<supl::tl::type_list<bool, int, char>, 1>,
    supl::tl::type_list<bool, int>>);

static_assert(
  std::is_same_v<
    supl::tl::drop_back_n_t<supl::tl::type_list<bool, int, char>, 3>,
    supl::tl::type_list<>>);

static_assert(
  std::is_same_v<
    supl::tl::drop_back_n_t<supl::tl::type_list<bool, int, char>, 0>,
    supl::tl::type_list<bool, int, char>>);

///////////////////////////////////////////// pop_back

static_assert(std::is_same_v<
              supl::tl::pop_back_t<supl::tl::type_list<int, char, bool>>,
              supl::tl::type_list<int, char>>);

static_assert(
  std::is_same_v<supl::tl::pop_back_t<supl::tl::type_list<bool>>,
                 supl::tl::type_list<>>);

static_assert(
  std::is_same_v<supl::tl::pop_back_t<std::tuple<int, char, bool>>,
                 std::tuple<int, char>>);

///////////////////////////////////////////// pop_front

static_assert(std::is_same_v<
              supl::tl::pop_front_t<supl::tl::type_list<bool, int, char>>,
              supl::tl::type_list<int, char>>);

static_assert(
  std::is_same_v<supl::tl::pop_front_t<supl::tl::type_list<bool>>,
                 supl::tl::type_list<>>);
static_assert(
  std::is_same_v<supl::tl::pop_front_t<std::tuple<bool, int, char>>,
                 std::tuple<int, char>>);

///////////////////////////////////////////// sublist

static_assert(
  std::is_same_v<
    supl::tl::sublist_t<
      supl::tl::type_list<int, char, bool, float, double, std::size_t>,
      0,
      3>,
    supl::tl::type_list<int, char, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::sublist_t<
      supl::tl::type_list<int, char, bool, float, double, std::size_t>,
      0,
      0>,
    supl::tl::type_list<>>);

static_assert(
  std::is_same_v<
    supl::tl::sublist_t<
      supl::tl::type_list<int, char, bool, float, double, std::size_t>,
      1,
      5>,
    supl::tl::type_list<char, bool, float, double>>);

static_assert(
  std::is_same_v<
    supl::tl::sublist_t<
      supl::tl::type_list<int, char, bool, float, double, std::size_t>,
      0,
      6>,
    supl::tl::type_list<int, char, bool, float, double, std::size_t>>);

static_assert(
  std::is_same_v<supl::tl::sublist_t<
                   std::tuple<int, char, bool, float, double, std::size_t>,
                   0,
                   3>,
                 std::tuple<int, char, bool>>);

static_assert(
  std::is_same_v<supl::tl::sublist_t<
                   std::tuple<int, char, bool, float, double, std::size_t>,
                   0,
                   0>,
                 std::tuple<>>);

static_assert(
  std::is_same_v<supl::tl::sublist_t<
                   std::tuple<int, char, bool, float, double, std::size_t>,
                   1,
                   5>,
                 std::tuple<char, bool, float, double>>);

static_assert(
  std::is_same_v<supl::tl::sublist_t<
                   std::tuple<int, char, bool, float, double, std::size_t>,
                   0,
                   6>,
                 std::tuple<int, char, bool, float, double, std::size_t>>);

///////////////////////////////////////////// insert

static_assert(
  std::is_same_v<
    supl::tl::insert_t<supl::tl::type_list<int, char, bool, float>,
                       2,
                       double,
                       std::size_t>,
    supl::tl::type_list<int, char, double, std::size_t, bool, float>>);

static_assert(
  std::is_same_v<
    supl::tl::insert_t<supl::tl::type_list<int, char, bool, float>,
                       0,
                       double,
                       std::size_t>,
    supl::tl::type_list<double, std::size_t, int, char, bool, float>>);

static_assert(
  std::is_same_v<
    supl::tl::insert_t<supl::tl::type_list<int, char, bool, float>,
                       4,
                       double,
                       std::size_t>,
    supl::tl::type_list<int, char, bool, float, double, std::size_t>>);

///////////////////////////////////////////// erase

static_assert(std::is_same_v<
              supl::tl::erase_t<supl::tl::type_list<int, char, bool>, 1>,
              supl::tl::type_list<int, bool>>);

static_assert(std::is_same_v<
              supl::tl::erase_t<supl::tl::type_list<int, char, bool>, 0>,
              supl::tl::type_list<char, bool>>);

static_assert(std::is_same_v<
              supl::tl::erase_t<supl::tl::type_list<int, char, bool>, 2>,
              supl::tl::type_list<int, char>>);

///////////////////////////////////////////// replace

static_assert(
  std::is_same_v<
    supl::tl::replace_t<supl::tl::type_list<int, char, bool>, 0, double>,
    supl::tl::type_list<double, char, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::replace_t<supl::tl::type_list<int, char, bool>, 1, double>,
    supl::tl::type_list<int, double, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::replace_t<supl::tl::type_list<int, char, bool>, 2, double>,
    supl::tl::type_list<int, char, double>>);

///////////////////////////////////////////// all_of

static_assert(supl::tl::all_of_v<supl::tl::type_list<int, char, bool>,
                                 std::is_integral>);

static_assert(not supl::tl::all_of_v<supl::tl::type_list<int, float, bool>,
                                     std::is_integral>);

static_assert(
  not supl::tl::all_of_v<supl::tl::type_list<double, float, bool>,
                         std::is_integral>);

static_assert(
  not supl::tl::all_of_v<supl::tl::type_list<int, float, double>,
                         std::is_integral>);

///////////////////////////////////////////// any_of

static_assert(supl::tl::any_of_v<supl::tl::type_list<int, char, bool>,
                                 std::is_integral>);

static_assert(supl::tl::any_of_v<supl::tl::type_list<int, float, bool>,
                                 std::is_integral>);

static_assert(supl::tl::any_of_v<supl::tl::type_list<double, float, bool>,
                                 std::is_integral>);

static_assert(supl::tl::any_of_v<supl::tl::type_list<int, float, double>,
                                 std::is_integral>);

static_assert(supl::tl::any_of_v<supl::tl::type_list<float, double, int>,
                                 std::is_integral>);

static_assert(
  not supl::tl::any_of_v<supl::tl::type_list<float, double, float>,
                         std::is_integral>);

///////////////////////////////////////////// none_of

static_assert(not supl::tl::none_of_v<supl::tl::type_list<int, char, bool>,
                                      std::is_integral>);

static_assert(
  not supl::tl::none_of_v<supl::tl::type_list<int, float, bool>,
                          std::is_integral>);

static_assert(
  not supl::tl::none_of_v<supl::tl::type_list<double, float, bool>,
                          std::is_integral>);

static_assert(
  not supl::tl::none_of_v<supl::tl::type_list<int, float, double>,
                          std::is_integral>);

static_assert(
  not supl::tl::none_of_v<supl::tl::type_list<float, double, int>,
                          std::is_integral>);

static_assert(
  supl::tl::none_of_v<supl::tl::type_list<float, double, float>,
                      std::is_integral>);

///////////////////////////////////////////// count_if

static_assert(supl::tl::count_if_v<
                supl::tl::type_list<int, char, bool, std::nullptr_t, void>,
                std::is_integral>
              == 3);

static_assert(supl::tl::count_if_v<
                supl::tl::type_list<int, char, bool, std::nullptr_t, void>,
                std::is_void>
              == 1);

static_assert(supl::tl::count_if_v<
                supl::tl::type_list<int, char, bool, std::nullptr_t, void>,
                std::is_fundamental>
              == 5);

static_assert(supl::tl::count_if_v<
                supl::tl::type_list<int, char, bool, std::nullptr_t, void>,
                std::is_const>
              == 0);

static_assert(supl::tl::count_if_v<supl::tl::type_list<>, std::is_const>
              == 0);

///////////////////////////////////////////// count

static_assert(supl::tl::count_v<
    supl::tl::type_list<int, char, bool>, int
    > == 1);

static_assert(supl::tl::count_v<
    supl::tl::type_list<int, char, bool>, char
    > == 1);

static_assert(supl::tl::count_v<
    supl::tl::type_list<int, char, bool>, bool
    > == 1);

static_assert(supl::tl::count_v<
    supl::tl::type_list<int, char, int, bool>, int
    > == 2);

static_assert(supl::tl::count_v<
    supl::tl::type_list<int, int, int, bool>, int
    > == 3);

static_assert(supl::tl::count_v<
    supl::tl::type_list<int, int, int, bool>, char
    > == 0);

static_assert(supl::tl::count_v<
    supl::tl::type_list<>, char
    > == 0);

///////////////////////////////////////////// transform

static_assert(
  std::is_same_v<
    supl::tl::transform_t<supl::tl::type_list<int>, std::add_const>,
    supl::tl::type_list<const int>>);

static_assert(std::is_same_v<
              supl::tl::transform_t<supl::tl::type_list<int, char, bool>,
                                    std::add_const>,
              supl::tl::type_list<const int, const char, const bool>>);

///////////////////////////////////////////// rotate_left

static_assert(
  std::is_same_v<
    supl::tl::rotate_left_t<supl::tl::type_list<int, char, bool>>,
    supl::tl::type_list<char, bool, int>>);

///////////////////////////////////////////// rotate_right

static_assert(
  std::is_same_v<
    supl::tl::rotate_right_t<supl::tl::type_list<int, char, bool>>,
    supl::tl::type_list<bool, int, char>>);

///////////////////////////////////////////// reorder

static_assert(
  std::is_same_v<
    supl::tl::
      reorder_t<supl::tl::type_list<int, char, bool, float>, 3, 1, 2, 1, 0>,
    supl::tl::type_list<float, char, bool, char, int>>);

static_assert(
  std::is_same_v<
    supl::tl::reorder_t<supl::tl::type_list<int, char, bool, float>>,
    supl::tl::type_list<>>);

static_assert(
  std::is_same_v<
    supl::tl::reorder_t<
      supl::tl::type_list<int, char, const int, void*, double, bool, char>,
      0,
      4,
      3,
      6,
      6,
      5,
      2,
      1,
      3,
      5,
      3,
      6,
      3>,
    supl::tl::type_list<int,
                        double,
                        void*,
                        char,
                        char,
                        bool,
                        const int,
                        char,
                        void*,
                        bool,
                        void*,
                        char,
                        void*>>);

///////////////////////////////////////////// reverse

static_assert(
  std::is_same_v<supl::tl::reverse_t<supl::tl::type_list<int, char, bool>>,
                 supl::tl::type_list<bool, char, int>>);

static_assert(
  std::is_same_v<
    supl::tl::reverse_t<supl::tl::type_list<int, void, char, bool>>,
    supl::tl::type_list<bool, char, void, int>>);

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::reverse_t<supl::tl::type_list< */
/*             int, int*, int**, int***, int****, int*****, int******, */
/*             int*******, int********, int*********, int**********, */
/*             int***********, int************, int*************, */
/*             int**************, int***************, int****************, */
/*             int*****************, int******************, */
/*             int*******************, int********************, */
/*             int*********************, int**********************, */
/*             int***********************, int************************, */
/*             int*************************, int**************************, */
/*             int***************************, */
/*             int****************************, */
/*             int*****************************, */
/*             int******************************, */
/*             int*******************************, */
/*             int********************************, */
/*             int*********************************, */
/*             int**********************************, */
/*             int***********************************, */
/*             int************************************, */
/*             int*************************************, */
/*             int**************************************>>, */
/*         supl::tl::type_list< */
/*             int**************************************, */
/*             int*************************************, */
/*             int************************************, */
/*             int***********************************, */
/*             int**********************************, */
/*             int*********************************, */
/*             int********************************, */
/*             int*******************************, */
/*             int******************************, */
/*             int*****************************, */
/*             int****************************, */
/*             int***************************, int**************************, */
/*             int*************************, int************************, */
/*             int***********************, int**********************, */
/*             int*********************, int********************, */
/*             int*******************, int******************, */
/*             int*****************, int****************, int***************, */
/*             int**************, int*************, int************, */
/*             int***********, int**********, int*********, int********, */
/*             int*******, int******, int*****, int****, int***, int**, int*, */
/*             int>>); */

// clang-format off
static_assert(std::is_same_v<
              supl::tl::reverse_t<
              supl::tl::type_list<
              int,
              int*,
              int**,
              int***,
              int****,
              int*****,
              int******,
              int*******,
              int********,
              int*********,
              int**********,
              int***********,
              int************,
              int*************,
              int**************,
              int***************,
              int****************,
              int*****************,
              int******************,
              int*******************,
              int********************,
              int*********************,
              int**********************,
              int***********************,
              int************************,
              int*************************,
              int**************************,
              int***************************,
              int****************************,
              int*****************************,
              int******************************,
              int*******************************,
              int********************************,
              int*********************************,
              int**********************************,
              int***********************************,
              int************************************,
              int*************************************,
              int**************************************
              >>,
              supl::tl::type_list<
              int**************************************,
              int*************************************,
              int************************************,
              int***********************************,
              int**********************************,
              int*********************************,
              int********************************,
              int*******************************,
              int******************************,
              int*****************************,
              int****************************,
              int***************************,
              int**************************,
              int*************************,
              int************************,
              int***********************,
              int**********************,
              int*********************,
              int********************,
              int*******************,
              int******************,
              int*****************,
              int****************,
              int***************,
              int**************,
              int*************,
              int************,
              int***********,
              int**********,
              int*********,
              int********,
              int*******,
              int******,
              int*****,
              int****,
              int***,
              int**,
              int*,
              int
              >>);
// clang-format on

static_assert(std::is_same_v<supl::tl::reverse_t<supl::tl::type_list<>>,
                             supl::tl::type_list<>>);

///////////////////////////////////////////// split

static_assert(
  std::is_same_v<
    supl::tl::split_t<
      supl::tl::type_list<int, char, bool, float, double, unsigned>,
      3>,
    supl::tl::type_pair<supl::tl::type_list<int, char, bool>,
                        supl::tl::type_list<float, double, unsigned>>>);

static_assert(
  std::is_same_v<
    supl::tl::split_t<supl::tl::type_list<int>, 1>,
    supl::tl::type_pair<supl::tl::type_list<int>, supl::tl::type_list<>>>);

static_assert(
  std::is_same_v<
    supl::tl::split_t<supl::tl::type_list<int>, 0>,
    supl::tl::type_pair<supl::tl::type_list<>, supl::tl::type_list<int>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool>, 0>,
              supl::tl::type_pair<supl::tl::type_list<>,
                                  supl::tl::type_list<int, char, bool>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool>, 3>,
              supl::tl::type_pair<supl::tl::type_list<int, char, bool>,
                                  supl::tl::type_list<>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool>, 1>,
              supl::tl::type_pair<supl::tl::type_list<int>,
                                  supl::tl::type_list<char, bool>>>);

static_assert(
  std::is_same_v<
    supl::tl::split_t<
      supl::tl::type_list<int, char, bool, float, double, unsigned>,
      5>,
    supl::tl::type_pair<
      supl::tl::type_list<int, char, bool, float, double>,
      supl::tl::type_list<unsigned>>>);

///////////////////////////////////////////// swap

static_assert(
  std::is_same_v<supl::tl::swap_t<supl::tl::type_list<int, bool>, 0, 1>,
                 supl::tl::type_list<bool, int>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 1, 3>,
    supl::tl::type_list<int, unsigned, void, char, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 0, 3>,
    supl::tl::type_list<unsigned, char, void, int, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 1, 4>,
    supl::tl::type_list<int, bool, void, unsigned, char>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 1, 2>,
    supl::tl::type_list<int, void, char, unsigned, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 2, 3>,
    supl::tl::type_list<int, char, unsigned, void, bool>>);

static_assert(
  std::is_same_v<supl::tl::swap_t<supl::tl::type_list<int, bool>, 1, 0>,
                 supl::tl::type_list<bool, int>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 3, 1>,
    supl::tl::type_list<int, unsigned, void, char, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 3, 0>,
    supl::tl::type_list<unsigned, char, void, int, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 4, 1>,
    supl::tl::type_list<int, bool, void, unsigned, char>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 2, 1>,
    supl::tl::type_list<int, void, char, unsigned, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 3, 2>,
    supl::tl::type_list<int, char, unsigned, void, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 3, 3>,
    supl::tl::type_list<int, char, void, unsigned, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 0, 0>,
    supl::tl::type_list<int, char, void, unsigned, bool>>);

static_assert(
  std::is_same_v<
    supl::tl::
      swap_t<supl::tl::type_list<int, char, void, unsigned, bool>, 4, 4>,
    supl::tl::type_list<int, char, void, unsigned, bool>>);

///////////////////////////////////////////// interleave

static_assert(
  std::is_same_v<
    supl::tl::interleave_t<
      supl::tl::type_list<int, char, bool, bool(int, char), void>,
      supl::tl::type_list<std::nullptr_t,
                          std::size_t,
                          volatile void*,
                          unsigned,
                          const char>>,
    supl::tl::type_list<int,
                        std::nullptr_t,
                        char,
                        std::size_t,
                        bool,
                        volatile void*,
                        bool(int, char),
                        unsigned,
                        void,

                        const char>>);

///////////////////////////////////////////// has_duplicates

static_assert(not supl::tl::has_duplicates_v<
              supl::tl::type_list<int,
                                  char,
                                  bool,
                                  const char,
                                  void,
                                  const int,
                                  int*,
                                  const int*,
                                  const volatile int*&>>);

static_assert(
  supl::tl::has_duplicates_v<supl::tl::type_list<int,
                                                 char,
                                                 bool,
                                                 const char,
                                                 void,
                                                 const int,
                                                 int,
                                                 const int*,
                                                 const volatile int*&>>);

///////////////////////////////////////////// find

static_assert(supl::tl::find_v<supl::tl::type_list<int, char, bool>, char>
              == 1);

static_assert(supl::tl::find_v<supl::tl::type_list<int, char, bool>, int>
              == 0);

static_assert(supl::tl::find_v<supl::tl::type_list<int, char, bool>, bool>
              == 2);

static_assert(supl::tl::find_v<supl::tl::type_list<int, char, bool>, void>
              == 3);

static_assert(
  supl::tl::find_v<supl::tl::type_list<int, char, bool, char>, char> == 1);

///////////////////////////////////////////// deduplicate

static_assert(
  std::is_same_v<supl::tl::deduplicate_t<
                   supl::tl::type_list<int, char, bool, int, char, int>>,
                 supl::tl::type_list<int, char, bool>>);

static_assert(
  std::is_same_v<supl::tl::deduplicate_t<supl::tl::type_list<int,
                                                             int,
                                                             int,
                                                             int,
                                                             double,
                                                             int,
                                                             int,
                                                             int,
                                                             int,
                                                             int,
                                                             char>>,
                 supl::tl::type_list<int, double, char>>);

static_assert(
  std::is_same_v<
    supl::tl::deduplicate_t<supl::tl::type_list<int, char, bool>>,
    supl::tl::type_list<int, char, bool>>);

static_assert(
  std::is_same_v<supl::tl::deduplicate_t<supl::tl::type_list<int>>,
                 supl::tl::type_list<int>>);

static_assert(
  std::is_same_v<supl::tl::deduplicate_t<supl::tl::type_list<int, int>>,
                 supl::tl::type_list<int>>);

static_assert(
  std::is_same_v<
    supl::tl::deduplicate_t<supl::tl::type_list<int,
                                                char,
                                                int&,
                                                int(),
                                                int&,
                                                int(),
                                                int(int),
                                                int,
                                                int&,
                                                char,
                                                char,
                                                char*,
                                                int()>>,
    supl::tl::type_list<int, char, int&, int(), int(int), char*>>);

static_assert(
  std::is_same_v<supl::tl::deduplicate_t<supl::tl::type_list<>>,
                 supl::tl::type_list<>>);

///////////////////////////////////////////// equal

static_assert(supl::tl::equal_v<supl::tl::type_list<int, char, bool>,
                                supl::tl::type_list<int, char, bool>>);

static_assert(
  not supl::tl::equal_v<supl::tl::type_list<int, char, bool, float>,
                        supl::tl::type_list<int, char, bool>>);

static_assert(
  not supl::tl::equal_v<supl::tl::type_list<int, char, bool>,
                        supl::tl::type_list<int, char, bool, float>>);

static_assert(not supl::tl::equal_v<supl::tl::type_list<int, float, bool>,
                                    supl::tl::type_list<int, char, bool>>);

static_assert(not supl::tl::equal_v<supl::tl::type_list<void, char, bool>,
                                    supl::tl::type_list<int, char, bool>>);

static_assert(
  not supl::tl::equal_v<supl::tl::type_list<int, char, bool>,
                        supl::tl::type_list<int, char, unsigned>>);

static_assert(supl::tl::equal_v<supl::tl::type_list<int, char, bool>,
                                std::tuple<int, char, bool>>);

///////////////////////////////////////////// multi_apply

static_assert(
  std::is_same_v<supl::tl::multi_apply_t<int,
                                         std::add_const,
                                         std::add_lvalue_reference,
                                         std::add_rvalue_reference,
                                         std::make_unsigned>,
                 supl::tl::type_list<const int, int&, int&&, unsigned>>);

///////////////////////////////////////////// enumerate

static_assert(
  std::is_same_v<
    supl::tl::enumerate_t<
      supl::tl::type_list<int, char, bool, void*, const unsigned short&>>,
    supl::tl::type_list<
      supl::tl::type_index_pair<int, 0>,
      supl::tl::type_index_pair<char, 1>,
      supl::tl::type_index_pair<bool, 2>,
      supl::tl::type_index_pair<void*, 3>,
      supl::tl::type_index_pair<const unsigned short&, 4>>>);

///////////////////////////////////////////// is_permutation

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<bool, char, float, int>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<bool, char, int, float>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<bool, float, char, int>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<bool, float, int, char>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<bool, int, char, float>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<bool, int, float, char>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<char, bool, float, int>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<char, bool, int, float>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<char, float, bool, int>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<char, float, int, bool>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<char, int, bool, float>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<char, int, float, bool>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<float, bool, char, int>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<float, bool, int, char>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<float, char, bool, int>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<float, char, int, bool>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<float, int, bool, char>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<float, int, char, bool>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<int, bool, char, float>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<int, bool, float, char>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<int, char, bool, float>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<int, char, float, bool>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<int, float, bool, char>>);

static_assert(
  supl::tl::is_permutation_v<supl::tl::type_list<int, char, bool, float>,
                             supl::tl::type_list<int, float, char, bool>>);

static_assert(supl::tl::is_permutation_v<supl::tl::type_list<>,
                                         supl::tl::type_list<>>);

static_assert(! supl::tl::is_permutation_v<supl::tl::type_list<int>,
                                           supl::tl::type_list<>>);

static_assert(! supl::tl::is_permutation_v<supl::tl::type_list<>,
                                           supl::tl::type_list<int>>);

static_assert(! supl::tl::is_permutation_v<
              supl::tl::type_list<int, char, bool, float>,
              supl::tl::type_list<char, float, bool, long>>);

static_assert(supl::tl::is_permutation_v<supl::tl::type_list<int, int, char, int>,
                                         supl::tl::type_list<int, char, int, int>>);

static_assert(! supl::tl::is_permutation_v<supl::tl::type_list<int, char, char, int>,
                                         supl::tl::type_list<int, char, int, int>>);

static_assert(supl::tl::is_permutation_v<supl::tl::type_list<char, int, int, int>,
                                         supl::tl::type_list<int, char, int, int>>);

static_assert(! supl::tl::is_permutation_v<supl::tl::type_list<char, int, int, int, void>,
                                         supl::tl::type_list<int, char, int, int>>);

static_assert(supl::tl::is_permutation_v<supl::tl::type_list<char, int, int, int, void>,
                                         supl::tl::type_list<int, void, char, int, int>>);

///////////////////////////////////////////// is_subset

static_assert(supl::tl::is_subset_v<
    supl::tl::type_list<>,
    supl::tl::type_list<>
    >);

static_assert(! supl::tl::is_subset_v<
    supl::tl::type_list<bool, int, char>,
    supl::tl::type_list<>
    >);

static_assert(supl::tl::is_subset_v<
    supl::tl::type_list<>,
    supl::tl::type_list<bool, int, char>
    >);

static_assert(supl::tl::is_subset_v<
    supl::tl::type_list<int, char, bool>,
    supl::tl::type_list<bool, int, char>
    >);

static_assert(supl::tl::is_subset_v<
    supl::tl::type_list<int, char, bool>,
    supl::tl::type_list<int, char, bool>
    >);

static_assert(supl::tl::is_subset_v<
    supl::tl::type_list<int, bool>,
    supl::tl::type_list<int, char, bool>
    >);

static_assert(supl::tl::is_subset_v<
    supl::tl::type_list<int>,
    supl::tl::type_list<int, char, bool>
    >);










auto main() -> int
{ }
