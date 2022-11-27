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
