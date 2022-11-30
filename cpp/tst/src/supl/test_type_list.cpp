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
  ehanc::run_test("supl::type_at_index", &pass);
  ehanc::run_test("supl::push_back", &pass);
  ehanc::run_test("supl::push_front", &pass);
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
