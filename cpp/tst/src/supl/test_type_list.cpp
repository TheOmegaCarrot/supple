#include "supl/test_type_list.h"
#include "test_utils.hpp"

static auto pass() -> ehanc::test
{
  return ehanc::test {};
}

void test_type_list()
{
  // All the testing is done at compile time
  // If this TU compiles, all tests pass
  // Test results here just for seratonin

  ehanc::run_test("supl::contains", &pass);
  ehanc::run_test("supl::type_list_size", &pass);
  ehanc::run_test("supl::type_list_concat", &pass);
  ehanc::run_test("supl::type_at_index", &pass);
  ehanc::run_test("supl::push_back", &pass);
  ehanc::run_test("supl::push_front", &pass);
  ehanc::run_test("supl::front_n", &pass);
  ehanc::run_test("supl::pop_back", &pass);
  ehanc::run_test("supl::pop_front", &pass);
}

///////////////////////////////////////////// contains

static_assert(
    supl::contains_type_v<int, supl::type_list<int, char, bool>>);

static_assert(
    supl::contains_type_v<int, supl::type_list<char, bool, int>>);

static_assert(
    supl::contains_type_v<int, supl::type_list<char, int, bool>>);

static_assert(
    not supl::contains_type_v<int, supl::type_list<char, bool, float>>);

static_assert(supl::contains_type_v<int, std::tuple<int, char, bool>>);

static_assert(supl::contains_type_v<int, std::tuple<char, bool, int>>);

static_assert(supl::contains_type_v<int, std::tuple<char, int, bool>>);

static_assert(
    not supl::contains_type_v<int, std::tuple<char, bool, float>>);

///////////////////////////////////////////// type_list_size

static_assert(supl::type_list_size_v<supl::type_list<>> == 0);

static_assert(supl::type_list_size_v<
                  supl::type_list<bool, int, char, float, bool>> == 5);

static_assert(
    supl::type_list_size_v<std::tuple<bool, int, char, float, bool>> == 5);

///////////////////////////////////////////// type_list_concat

static_assert(
    std::is_same_v<
        supl::type_list_concat_t<supl::type_list<int, bool, char>,
                                 supl::type_list<float, std::size_t>>,
        supl::type_list<int, bool, char, float, std::size_t>>);

static_assert(std::is_same_v<
              supl::type_list_concat_t<supl::type_list<int, bool, char>,
                                       supl::type_list<>>,
              supl::type_list<int, bool, char>>);

static_assert(std::is_same_v<
              supl::type_list_concat_t<supl::type_list<>,
                                       supl::type_list<int, bool, char>>,
              supl::type_list<int, bool, char>>);

static_assert(std::is_same_v<supl::type_list_concat_t<supl::type_list<>,
                                                      supl::type_list<>>,
                             supl::type_list<>>);

static_assert(std::is_same_v<
              supl::type_list_concat_t<std::tuple<int, bool, char>,
                                       std::tuple<float, std::size_t>>,
              std::tuple<int, bool, char, float, std::size_t>>);

static_assert(
    std::is_same_v<supl::type_list_concat_t<std::tuple<int, bool, char>,
                                            std::tuple<>>,
                   std::tuple<int, bool, char>>);

static_assert(
    std::is_same_v<supl::type_list_concat_t<std::tuple<>,
                                            std::tuple<int, bool, char>>,
                   std::tuple<int, bool, char>>);

static_assert(
    std::is_same_v<supl::type_list_concat_t<std::tuple<>, std::tuple<>>,
                   std::tuple<>>);

///////////////////////////////////////////// type_at_index

static_assert(
    std::is_same_v<
        supl::type_at_index_t<0, supl::type_list<int, char, bool>>, int>);

static_assert(
    std::is_same_v<
        supl::type_at_index_t<2, supl::type_list<int, char, bool>>, bool>);

static_assert(
    std::is_same_v<
        supl::type_at_index_t<1, supl::type_list<int, char, bool>>, char>);

static_assert(std::is_same_v<
              supl::type_at_index_t<0, std::tuple<int, char, bool>>, int>);

static_assert(
    std::is_same_v<supl::type_at_index_t<2, std::tuple<int, char, bool>>,
                   bool>);

static_assert(
    std::is_same_v<supl::type_at_index_t<1, std::tuple<int, char, bool>>,
                   char>);

///////////////////////////////////////////// push_back

static_assert(
    std::is_same_v<supl::type_list<int, char, bool>,
                   supl::push_back_t<supl::type_list<int, char>, bool>>);

static_assert(std::is_same_v<supl::type_list<bool>,
                             supl::push_back_t<supl::type_list<>, bool>>);

static_assert(
    std::is_same_v<std::tuple<int, char, bool>,
                   supl::push_back_t<std::tuple<int, char>, bool>>);

///////////////////////////////////////////// push_front

static_assert(
    std::is_same_v<supl::type_list<bool, int, char>,
                   supl::push_front_t<supl::type_list<int, char>, bool>>);

static_assert(std::is_same_v<supl::type_list<bool>,
                             supl::push_front_t<supl::type_list<>, bool>>);

static_assert(
    std::is_same_v<std::tuple<bool, int, char>,
                   supl::push_front_t<std::tuple<int, char>, bool>>);

///////////////////////////////////////////// front_n

static_assert(
    std::is_same_v<supl::front_n_t<supl::type_list<bool, int, char>, 2>,
                   supl::type_list<bool, int>>);

static_assert(
    std::is_same_v<supl::front_n_t<supl::type_list<bool, int, char>, 3>,
                   supl::type_list<bool, int, char>>);

static_assert(
    std::is_same_v<supl::front_n_t<supl::type_list<bool, int, char>, 0>,
                   supl::type_list<>>);

static_assert(
    std::is_same_v<supl::front_n_t<std::tuple<bool, int, char>, 2>,
                   std::tuple<bool, int>>);

static_assert(
    std::is_same_v<supl::front_n_t<std::tuple<bool, int, char>, 3>,
                   std::tuple<bool, int, char>>);

static_assert(
    std::is_same_v<supl::front_n_t<std::tuple<bool, int, char>, 0>,
                   std::tuple<>>);

///////////////////////////////////////////// pop_back

static_assert(
    std::is_same_v<supl::pop_back_t<supl::type_list<int, char, bool>>,
                   supl::type_list<int, char>>);

static_assert(std::is_same_v<supl::pop_back_t<supl::type_list<bool>>,
                             supl::type_list<>>);

static_assert(std::is_same_v<supl::pop_back_t<std::tuple<int, char, bool>>,
                             std::tuple<int, char>>);

///////////////////////////////////////////// pop_front

static_assert(
    std::is_same_v<supl::pop_front_t<supl::type_list<bool, int, char>>,
                   supl::type_list<int, char>>);

static_assert(std::is_same_v<supl::pop_front_t<supl::type_list<bool>>,
                             supl::type_list<>>);
static_assert(
    std::is_same_v<supl::pop_front_t<std::tuple<bool, int, char>>,
                   std::tuple<int, char>>);
