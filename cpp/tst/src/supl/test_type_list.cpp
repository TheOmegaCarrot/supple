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

  ehanc::run_test("supl::tl::contains", &pass);
  ehanc::run_test("supl::tl::size", &pass);
  ehanc::run_test("supl::tl::concat", &pass);
  ehanc::run_test("supl::tl::at_index", &pass);
  ehanc::run_test("supl::tl::push_back", &pass);
  ehanc::run_test("supl::tl::push_front", &pass);
  ehanc::run_test("supl::tl::front_n", &pass);
  ehanc::run_test("supl::tl::pop_back", &pass);
  ehanc::run_test("supl::tl::pop_front", &pass);
}

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
    std::is_same_v<supl::tl::concat_t<std::tuple<int, bool, char>,
                                      std::tuple<float, std::size_t>>,
                   std::tuple<int, bool, char, float, std::size_t>>);

static_assert(
    std::is_same_v<
        supl::tl::concat_t<std::tuple<int, bool, char>, std::tuple<>>,
        std::tuple<int, bool, char>>);

static_assert(
    std::is_same_v<
        supl::tl::concat_t<std::tuple<>, std::tuple<int, bool, char>>,
        std::tuple<int, bool, char>>);

static_assert(
    std::is_same_v<supl::tl::concat_t<std::tuple<>, std::tuple<>>,
                   std::tuple<>>);

///////////////////////////////////////////// front

static_assert(
    std::is_same_v<supl::tl::front_t<supl::tl::type_list<int, bool, char>>,
                   int>);

static_assert(
    std::is_same_v<supl::tl::front_t<supl::tl::type_list<int>>, int>);

static_assert(
    std::is_same_v<supl::tl::front_t<std::tuple<int, bool, char>>, int>);

static_assert(std::is_same_v<supl::tl::front_t<std::tuple<int>>, int>);

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
    std::is_same_v<std::tuple<int, char, bool>,
                   supl::tl::push_back_t<std::tuple<int, char>, bool>>);

///////////////////////////////////////////// push_front

static_assert(
    std::is_same_v<
        supl::tl::type_list<bool, int, char>,
        supl::tl::push_front_t<supl::tl::type_list<int, char>, bool>>);

static_assert(
    std::is_same_v<supl::tl::type_list<bool>,
                   supl::tl::push_front_t<supl::tl::type_list<>, bool>>);

static_assert(
    std::is_same_v<std::tuple<bool, int, char>,
                   supl::tl::push_front_t<std::tuple<int, char>, bool>>);

///////////////////////////////////////////// front_n

static_assert(std::is_same_v<
              supl::tl::front_n_t<supl::tl::type_list<bool, int, char>, 2>,
              supl::tl::type_list<bool, int>>);

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
