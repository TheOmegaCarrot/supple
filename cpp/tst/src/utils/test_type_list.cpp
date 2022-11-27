#include "utils/test_type_list.h"
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

  ehanc::run_test("push_back", &pass);
}

///////////////////////////////////////////// contains

static_assert(
    ehanc::contains_type_v<int, ehanc::type_list<int, char, bool>>);

static_assert(
    ehanc::contains_type_v<int, ehanc::type_list<char, bool, int>>);

static_assert(
    ehanc::contains_type_v<int, ehanc::type_list<char, int, bool>>);

static_assert(
    not ehanc::contains_type_v<int, ehanc::type_list<char, bool, float>>);

static_assert(ehanc::contains_type_v<int, std::tuple<int, char, bool>>);

static_assert(ehanc::contains_type_v<int, std::tuple<char, bool, int>>);

static_assert(ehanc::contains_type_v<int, std::tuple<char, int, bool>>);

static_assert(
    not ehanc::contains_type_v<int, std::tuple<char, bool, float>>);

///////////////////////////////////////////// type_list_size

static_assert(ehanc::type_list_size_v<ehanc::type_list<>> == 0);

static_assert(ehanc::type_list_size_v<
                  ehanc::type_list<bool, int, char, float, bool>> == 5);

static_assert(ehanc::type_list_size_v<
                  std::tuple<bool, int, char, float, bool>> == 5);

///////////////////////////////////////////// push_back

static_assert(
    std::is_same_v<ehanc::type_list<int, char, bool>,
                   ehanc::push_back_t<ehanc::type_list<int, char>, bool>>);

static_assert(
    std::is_same_v<ehanc::type_list<bool>,
                   ehanc::push_back_t<ehanc::type_list<>, bool>>);

static_assert(
    std::is_same_v<std::tuple<int, char, bool>,
                   ehanc::push_back_t<std::tuple<int, char>, bool>>);

///////////////////////////////////////////// push_front

static_assert(std::is_same_v<
              ehanc::type_list<bool, int, char>,
              ehanc::push_front_t<ehanc::type_list<int, char>, bool>>);

static_assert(
    std::is_same_v<ehanc::type_list<bool>,
                   ehanc::push_front_t<ehanc::type_list<>, bool>>);

static_assert(
    std::is_same_v<std::tuple<bool, int, char>,
                   ehanc::push_front_t<std::tuple<int, char>, bool>>);

