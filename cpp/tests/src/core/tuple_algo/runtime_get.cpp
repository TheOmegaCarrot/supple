#include "supl/tuple_algo.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

using supl::tuple::impl::tuple_element_variant_t;

static_assert(
  std::is_same_v<std::variant<int, char, bool>,
                 tuple_element_variant_t<std::tuple<int, char, bool>>>);

static_assert(std::is_same_v<
              std::variant<int, char, bool>,
              tuple_element_variant_t<
                std::tuple<int, char, char, char, int, bool, int, bool>>>);

static_assert(
  std::is_same_v<std::variant<int, char, std::reference_wrapper<bool>>,
                 tuple_element_variant_t<std::tuple<int, char, bool&>>>);

static_assert(
  std::is_same_v<std::variant<int, char, std::reference_wrapper<bool>>,
                 tuple_element_variant_t<std::tuple<int, char, bool&&>>>);

static_assert(
  std::is_same_v<
    std::variant<const int, char, std::reference_wrapper<const bool>>,
    tuple_element_variant_t<std::tuple<const int, char, const bool&>>>);

auto basic_test() -> supl::test_results
{
  supl::test_results results;

  std::tuple tup {42, '&', true};
  auto test_value {supl::tuple::runtime_get(tup, 0)};

  std::visit(
    [&results](auto should_be_int) {
      if constexpr ( std::is_same_v<decltype(should_be_int), int> ) {
        results.enforce_equal(should_be_int, 42, "Wrong value");
      } else {
        results.fail("Wrong type");
      }
    },
    test_value);

  return results;
}

auto test_runtime_get() -> supl::test_section
{
  supl::test_section section;

  section.add_test("basic_test", &basic_test);

  return section;
}

int main()
{
  supl::test_runner runner;

  runner.add_section(test_runtime_get());

  return runner.run();
}
