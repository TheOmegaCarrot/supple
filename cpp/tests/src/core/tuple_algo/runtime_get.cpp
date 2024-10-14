#include "supl/tuple_algo.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

using supl::tuple::impl::tuple_element_variant_t;
using my_monostate = supl::tuple::impl::monostate;

static_assert(
  std::is_same_v<std::variant<my_monostate, int, char, bool>,
                 tuple_element_variant_t<std::tuple<int, char, bool>>>);

static_assert(std::is_same_v<
              std::variant<my_monostate, int, char, bool>,
              tuple_element_variant_t<
                std::tuple<int, char, char, char, int, bool, int, bool>>>);

static_assert(
  std::is_same_v<
    std::variant<my_monostate, int, char, std::reference_wrapper<bool>>,
    tuple_element_variant_t<std::tuple<int, char, bool&>>>);

static_assert(
  std::is_same_v<
    std::variant<my_monostate, int, char, std::reference_wrapper<bool>>,
    tuple_element_variant_t<std::tuple<int, char, bool&&>>>);

static_assert(
  std::is_same_v<
    std::variant<my_monostate,
                 const int,
                 char,
                 std::reference_wrapper<const bool>>,
    tuple_element_variant_t<std::tuple<const int, char, const bool&>>>);

template <typename Variant, typename Value>
void test_variant(supl::test_results& results,
                  const Variant& variant,
                  const Value& value)
{
  std::visit(
    [&results, &value](const auto& held) {
      if constexpr ( std::is_same_v<supl::remove_cvref_t<decltype(held)>,
                                    Value> ) {
        results.enforce_equal(held, value);
      } else {
        results.fail("Wrong type");
      }
    },
    variant);
}

template <typename Tuple, typename Expected>
void test_tuple_index(supl::test_results& results,
                      Tuple&& tuple,
                      const Expected& expected,
                      std::size_t idx)
{
  auto test_value {
    supl::tuple::runtime_get(std::forward<Tuple>(tuple), idx)};
  test_variant(results, test_value, expected);
}

auto basic_test() -> supl::test_results
{
  supl::test_results results;

  std::tuple tup {42, '&', true};
  test_tuple_index(results, tup, 42, 0);
  test_tuple_index(results, tup, '&', 1);
  test_tuple_index(results, tup, true, 2);

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
