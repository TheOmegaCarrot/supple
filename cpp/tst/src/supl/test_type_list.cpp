#include "supl/type_list.hpp"

///////////////////////////////////////////// make_list

static_assert(std::is_same_v<
              supl::tl::make_list_t<supl::tl::type_list, int, char, bool,
                                    void*, const int, std::nullptr_t>,
              supl::tl::type_list<int, char, bool, void*, const int,
                                  std::nullptr_t>>);

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

static_assert(supl::tl::size_v<
                  supl::tl::type_list<bool, int, char, float, bool>> == 5);

static_assert(
    supl::tl::size_v<std::tuple<bool, int, char, float, bool>> == 5);

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

static_assert(std::is_same_v<
              supl::tl::concat_t<supl::tl::type_list<>,
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

static_assert(std::is_same_v<
              supl::tl::concat_t<
                  supl::tl::type_list<int, char>,
                  supl::tl::type_list<bool, float>,
                  supl::tl::type_list<unsigned, void>,
                  supl::tl::type_list<const int, int&&>,
                  supl::tl::type_list<const volatile void*, const bool>,
                  supl::tl::type_list<void, void>,
                  supl::tl::type_list<double, std::nullptr_t>>,
              supl::tl::type_list<int, char, bool, float, unsigned, void,
                                  const int, int&&, const volatile void*,
                                  const bool, void, void, double,
                                  std::nullptr_t>>);

static_assert(std::is_same_v<
              supl::tl::concat_t<
                  supl::tl::type_list<int, char>,
                  supl::tl::type_list<bool, float>, supl::tl::type_list<>,
                  supl::tl::type_list<const int, int&&>,
                  supl::tl::type_list<const volatile void*, const bool>,
                  supl::tl::type_list<>,
                  supl::tl::type_list<double, std::nullptr_t>>,
              supl::tl::type_list<int, char, bool, float, const int, int&&,
                                  const volatile void*, const bool, double,
                                  std::nullptr_t>>);

static_assert(std::is_same_v<
              supl::tl::concat_t<supl::tl::type_list<int, char, bool>>,
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

static_assert(std::is_same_v<
              supl::tl::at_index_t<0, std::tuple<int, char, bool>>, int>);

static_assert(std::is_same_v<
              supl::tl::at_index_t<2, std::tuple<int, char, bool>>, bool>);

static_assert(std::is_same_v<
              supl::tl::at_index_t<1, std::tuple<int, char, bool>>, char>);

///////////////////////////////////////////// push_back

static_assert(
    std::is_same_v<
        supl::tl::type_list<int, char, bool>,
        supl::tl::push_back_t<supl::tl::type_list<int, char>, bool>>);

static_assert(
    std::is_same_v<supl::tl::type_list<bool>,
                   supl::tl::push_back_t<supl::tl::type_list<>, bool>>);

static_assert(
    std::is_same_v<supl::tl::type_list<int, char, bool, float, double>,
                   supl::tl::push_back_t<supl::tl::type_list<int, char>,
                                         bool, float, double>>);

static_assert(std::is_same_v<supl::tl::type_list<bool, float, double>,
                             supl::tl::push_back_t<supl::tl::type_list<>,
                                                   bool, float, double>>);

static_assert(std::is_same_v<supl::tl::type_list<int, char, bool>,
                             supl::tl::push_back_t<supl::tl::type_list<>,
                                                   int, char, bool>>);

static_assert(
    std::is_same_v<std::tuple<int, char, bool>,
                   supl::tl::push_back_t<std::tuple<int, char>, bool>>);

static_assert(std::is_same_v<std::tuple<int, char, bool, float, double>,
                             supl::tl::push_back_t<std::tuple<int, char>,
                                                   bool, float, double>>);

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
    std::is_same_v<supl::tl::type_list<bool, float, double, int, char>,
                   supl::tl::push_front_t<supl::tl::type_list<int, char>,
                                          bool, float, double>>);

static_assert(std::is_same_v<supl::tl::type_list<bool, float, double>,
                             supl::tl::push_front_t<supl::tl::type_list<>,
                                                    bool, float, double>>);

static_assert(
    std::is_same_v<std::tuple<bool, int, char>,
                   supl::tl::push_front_t<std::tuple<int, char>, bool>>);

static_assert(std::is_same_v<std::tuple<bool>,
                             supl::tl::push_front_t<std::tuple<>, bool>>);

static_assert(std::is_same_v<std::tuple<bool, float, double, int, char>,
                             supl::tl::push_front_t<std::tuple<int, char>,
                                                    bool, float, double>>);

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
        supl::tl::sublist_t<supl::tl::type_list<int, char, bool, float,
                                                double, std::size_t>,
                            0, 3>,
        supl::tl::type_list<int, char, bool>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<supl::tl::type_list<int, char, bool, float,
                                                double, std::size_t>,
                            0, 0>,
        supl::tl::type_list<>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<supl::tl::type_list<int, char, bool, float,
                                                double, std::size_t>,
                            1, 5>,
        supl::tl::type_list<char, bool, float, double>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<supl::tl::type_list<int, char, bool, float,
                                                double, std::size_t>,
                            0, 6>,
        supl::tl::type_list<int, char, bool, float, double, std::size_t>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<
            std::tuple<int, char, bool, float, double, std::size_t>, 0, 3>,
        std::tuple<int, char, bool>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<
            std::tuple<int, char, bool, float, double, std::size_t>, 0, 0>,
        std::tuple<>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<
            std::tuple<int, char, bool, float, double, std::size_t>, 1, 5>,
        std::tuple<char, bool, float, double>>);

static_assert(
    std::is_same_v<
        supl::tl::sublist_t<
            std::tuple<int, char, bool, float, double, std::size_t>, 0, 6>,
        std::tuple<int, char, bool, float, double, std::size_t>>);

///////////////////////////////////////////// insert

static_assert(
    std::is_same_v<
        supl::tl::insert_t<supl::tl::type_list<int, char, bool, float>, 2,
                           double, std::size_t>,
        supl::tl::type_list<int, char, double, std::size_t, bool, float>>);

static_assert(
    std::is_same_v<
        supl::tl::insert_t<supl::tl::type_list<int, char, bool, float>, 0,
                           double, std::size_t>,
        supl::tl::type_list<double, std::size_t, int, char, bool, float>>);

static_assert(
    std::is_same_v<
        supl::tl::insert_t<supl::tl::type_list<int, char, bool, float>, 4,
                           double, std::size_t>,
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

///////////////////////////////////////////// all_of

static_assert(supl::tl::all_of_v<supl::tl::type_list<int, char, bool>,
                                 std::is_integral>);

static_assert(not supl::tl::all_of_v<supl::tl::type_list<int, float, bool>,
                                     std::is_integral>);

static_assert(not supl::tl::all_of_v<
              supl::tl::type_list<double, float, bool>, std::is_integral>);

static_assert(not supl::tl::all_of_v<
              supl::tl::type_list<int, float, double>, std::is_integral>);

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

static_assert(not supl::tl::none_of_v<
              supl::tl::type_list<int, float, bool>, std::is_integral>);

static_assert(not supl::tl::none_of_v<
              supl::tl::type_list<double, float, bool>, std::is_integral>);

static_assert(not supl::tl::none_of_v<
              supl::tl::type_list<int, float, double>, std::is_integral>);

static_assert(not supl::tl::none_of_v<
              supl::tl::type_list<float, double, int>, std::is_integral>);

static_assert(
    supl::tl::none_of_v<supl::tl::type_list<float, double, float>,
                        std::is_integral>);

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
        supl::tl::reorder_t<supl::tl::type_list<int, char, bool, float>, 3,
                            1, 2, 1, 0>,
        supl::tl::type_list<float, char, bool, char, int>>);

static_assert(
    std::is_same_v<
        supl::tl::reorder_t<supl::tl::type_list<int, char, bool, float>>,
        supl::tl::type_list<>>);

static_assert(
    std::is_same_v<
        supl::tl::reorder_t<supl::tl::type_list<int, char, const int,
                                                void*, double, bool, char>,
                            0, 4, 3, 6, 6, 5, 2, 1, 3, 5, 3, 6, 3>,
        supl::tl::type_list<int, double, void*, char, char, bool,
                            const int, char, void*, bool, void*, char,
                            void*>>);

///////////////////////////////////////////// split

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool, float,
                                                    double, unsigned>,
                                3>,
              std::pair<supl::tl::type_list<int, char, bool>,
                        supl::tl::type_list<float, double, unsigned>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int>, 1>,
              std::pair<supl::tl::type_list<int>, supl::tl::type_list<>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int>, 0>,
              std::pair<supl::tl::type_list<>, supl::tl::type_list<int>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool>, 0>,
              std::pair<supl::tl::type_list<>,
                        supl::tl::type_list<int, char, bool>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool>, 3>,
              std::pair<supl::tl::type_list<int, char, bool>,
                        supl::tl::type_list<>>>);

static_assert(std::is_same_v<
              supl::tl::split_t<supl::tl::type_list<int, char, bool>, 1>,
              std::pair<supl::tl::type_list<int>,
                        supl::tl::type_list<char, bool>>>);

static_assert(
    std::is_same_v<
        supl::tl::split_t<
            supl::tl::type_list<int, char, bool, float, double, unsigned>,
            5>,
        std::pair<supl::tl::type_list<int, char, bool, float, double>,
                  supl::tl::type_list<unsigned>>>);

///////////////////////////////////////////// swap

/* static_assert( */
/*     std::is_same_v<supl::tl::swap_t<supl::tl::type_list<int, bool>, 0, 1>, */
/*                    supl::tl::type_list<bool, int>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 1, 3>, */
/*         supl::tl::type_list<int, unsigned, void, char, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 0, 3>, */
/*         supl::tl::type_list<unsigned, char, void, int, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 1, 4>, */
/*         supl::tl::type_list<int, bool, void, unsigned, char>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 1, 2>, */
/*         supl::tl::type_list<int, void, char, unsigned, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 2, 3>, */
/*         supl::tl::type_list<int, char, unsigned, void, bool>>); */

/* static_assert( */
/*     std::is_same_v<supl::tl::swap_t<supl::tl::type_list<int, bool>, 1, 0>, */
/*                    supl::tl::type_list<bool, int>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 3, 1>, */
/*         supl::tl::type_list<int, unsigned, void, char, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 3, 0>, */
/*         supl::tl::type_list<unsigned, char, void, int, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 4, 1>, */
/*         supl::tl::type_list<int, bool, void, unsigned, char>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 2, 1>, */
/*         supl::tl::type_list<int, void, char, unsigned, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 3, 2>, */
/*         supl::tl::type_list<int, char, unsigned, void, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 3, 3>, */
/*         supl::tl::type_list<int, char, void, unsigned, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 0, 0>, */
/*         supl::tl::type_list<int, char, void, unsigned, bool>>); */

/* static_assert( */
/*     std::is_same_v< */
/*         supl::tl::swap_t< */
/*             supl::tl::type_list<int, char, void, unsigned, bool>, 4, 4>, */
/*         supl::tl::type_list<int, char, void, unsigned, bool>>); */

///////////////////////////////////////////// interleave

static_assert(std::is_same_v<
              supl::tl::interleave_t<
                  supl::tl::type_list<int, char, bool, void>,
                  supl::tl::type_list<std::nullptr_t, std::size_t,
                                      unsigned, const char>>,
              supl::tl::type_list<int, std::nullptr_t, char, std::size_t,
                                  bool, unsigned, void, const char>>);
