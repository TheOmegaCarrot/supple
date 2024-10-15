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

/* #pragma message("DEBUG CODE PRESENT") */
/* #include <cxxabi.h> */

/* auto demangle(const std::string& input) -> std::string */
/* { */
/*   int status {0}; */
/*   const char* output { */
/*     abi::__cxa_demangle(input.c_str(), nullptr, nullptr, &status)}; */

/*   if ( status != 0 ) { */
/*     throw std::runtime_error {"Error demangling: " + input */
/*                               + " with code: " + std::to_string(status)}; */
/*   } */

/*   std::string retval {output}; */

/*   std::free((void*) output); */

/*   return retval; */
/* } */

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
        /* results.fail( */
        /*   "Wrong type, got: " */
        /*   + demangle(typeid(supl::remove_cvref_t<decltype(held)>).name()) */
        /*   + " but expected: " + demangle(typeid(Value).name())); */
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

auto references() -> supl::test_results
{
  supl::test_results results;

  int i {81};
  char c {'*'};
  bool b {false};
  std::tuple<int&, char&, bool&> tup {i, c, b};

  std::reference_wrapper good_i_ref {i};
  std::reference_wrapper good_c_ref {c};
  std::reference_wrapper good_b_ref {b};

  test_tuple_index(results, tup, good_i_ref, 0);
  test_tuple_index(results, tup, good_c_ref, 1);
  test_tuple_index(results, tup, good_b_ref, 2);

  auto i_ref_variant {supl::tuple::runtime_get(tup, 0)};
  auto i_ref {std::get<1>(i_ref_variant)};
  i_ref.get() = 42;

  auto c_ref_variant {supl::tuple::runtime_get(tup, 1)};
  auto c_ref {std::get<2>(c_ref_variant)};
  c_ref.get() = '$';

  auto b_ref_variant {supl::tuple::runtime_get(tup, 2)};
  auto b_ref {std::get<3>(b_ref_variant)};
  b_ref.get() = true;

  results.enforce_equal(i, 42);
  results.enforce_equal(c, '$');
  results.enforce_equal(b, true);

  test_tuple_index(results, tup, good_i_ref, 0);
  test_tuple_index(results, tup, good_c_ref, 1);
  test_tuple_index(results, tup, good_b_ref, 2);

  return results;
}

auto test_runtime_get() -> supl::test_section
{
  supl::test_section section;

  section.add_test("basic_test", &basic_test);
  section.add_test("test_references", &references);

  return section;
}

int main()
{
  supl::test_runner runner;

  runner.add_section(test_runtime_get());

  return runner.run();
}
