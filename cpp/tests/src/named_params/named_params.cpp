#include <cstddef>
#include <iostream>

#include "supl/named_params.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"

///////////// BEGIN EXAMPLE

// named parameters must be a simple struct with a public `value` member...
struct foo_size {
  std::size_t value;
};

// ...or an enum
enum struct foo_type : char {
  foo,
  bar,
  third
};

// this `to_stream` function is just for internal usage in this test file
// and is not necessary to add for named parameters
template <>
void supl::to_stream<foo_type>(std::ostream& out,
                               const foo_type& value) noexcept
{
  switch ( value ) {
    case foo_type::foo:
      out << "foo_type::foo";
      break;
    case foo_type::bar:
      out << "foo_type::bar";
      break;
    case foo_type::third:
      out << "foo_type::third";
      break;
  }
}

// again, a simple "value struct" is all that is needed
struct foo_count {
  std::size_t value;
};

// just a return type for the example below
struct foo_config {
  foo_type type;
  std::size_t size;
  std::size_t count;

  constexpr friend auto operator==(const foo_config& lhs,
                                   const foo_config& rhs) noexcept
  {
    return (lhs.type == rhs.type) && (lhs.size == rhs.size)
        && (lhs.count == rhs.count);
  }

  void to_stream(std::ostream& out) const noexcept
  {
    out << supl::stream_adapter {std::tie(type, size, count)};
  }
};

// functions with named parameters must have a template type pack
template <typename... Named_Params>
static auto configure_foo(Named_Params&&... raw_named_params)
{
  // provide set of legal named parameter types
  //
  // the order of the types is irrelevant
  // -
  // I could put these types in any order
  // without changing the behavior
  supl::named_params named_parameters {
    supl::tl::type_list<foo_type, foo_size, foo_count> {},
    // just forward the raw named parameters in
    std::forward<Named_Params>(raw_named_params)...};

  return foo_config {
    // get_or will get the `foo_type` parameter that was passed,
    // or the fallback value if none was provided
    // here, if no `foo_type` was specified, `foo_type::bar` will be used
    named_parameters.template get_or<foo_type>(foo_type::bar),
    // pass in the underlying value type as the fallback
    named_parameters.template get_or<foo_size>(5),
    named_parameters.template get_or<foo_count>(42)};
}

// This test also demonstrates the behavior of named parameters at the call site
// usage of the supple test framework should be easy to understand
// (if this example is not clear, let me know!)
static auto test_example() -> supl::test_results
{
  // just a test results object for the test framework
  supl::test_results results;

  results.enforce_equal(
    // returned struct from this...
    configure_foo(foo_type::foo, foo_size {10}, foo_count {12}),
    // ...must equal this (type, size, count)
    foo_config {foo_type::foo, 10, 12});

  results.enforce_equal(
    // named parameters may be in any order...
    configure_foo(foo_size {10}, foo_type::foo, foo_count {12}),
    // ...and the result will be the same
    foo_config {foo_type::foo, 10, 12});

  results.enforce_equal(
    // named parameters may be omitted, if the function permits it...
    configure_foo(),
    // ...in this case, the function has default values for omitted parameters
    foo_config {foo_type::bar, 5, 42});

  results.enforce_equal(
    // you can also only specify some, and again, order does not matter!
    configure_foo(foo_size {100}),
    foo_config {foo_type::bar, 100, 42});

  return results;
}

///////////// END EXAMPLE

// legal set
using ls = supl::tl::type_list<foo_type, foo_count, foo_config>;

static auto test_construction() -> supl::test_results
{
  supl::test_results results;

  // just testing the "happy path" of construction in this function
  //
  // constexpr construction of the object without a compilation error
  // is the purpose of this test function

  [[maybe_unused]] constexpr static supl::named_params test1 {
    ls {}, foo_type::bar, foo_size {5}, foo_count {16}};

  [[maybe_unused]] constexpr static supl::named_params test2 {
    ls {}, foo_size {8}, foo_count {500}, foo_type::foo};

  [[maybe_unused]] constexpr static supl::named_params test3 {
    ls {}, foo_count {508}, foo_type::third, foo_size {12}};

  [[maybe_unused]] constexpr static supl::named_params<foo_type> test4 {
    ls {}, foo_type::foo};

  [[maybe_unused]] constexpr static supl::named_params<foo_size> test5 {
    ls {}, foo_size {12}};

  [[maybe_unused]] constexpr static supl::named_params<foo_count> test6 {
    ls {}, foo_count {12}};

  [[maybe_unused]] constexpr static supl::named_params<> test7 {
    ls {},
  };

  [[maybe_unused]] constexpr static supl::named_params test8 {
    ls {},
  };

  // now without constexpr

  [[maybe_unused]] supl::named_params test10 {
    ls {}, foo_type::bar, foo_size {5}, foo_count {16}};

  [[maybe_unused]] supl::named_params test20 {
    ls {}, foo_size {8}, foo_count {500}, foo_type::foo};

  [[maybe_unused]] supl::named_params test30 {
    ls {}, foo_count {508}, foo_type::third, foo_size {12}};

  [[maybe_unused]] supl::named_params<foo_type> test40 {ls {},
                                                        foo_type::foo};

  [[maybe_unused]] supl::named_params<foo_size> test50 {ls {},
                                                        foo_size {12}};

  [[maybe_unused]] supl::named_params<foo_count> test60 {ls {},
                                                         foo_count {12}};

  [[maybe_unused]] supl::named_params<> test70 {ls {}};

  [[maybe_unused]] supl::named_params test80 {ls {}};

  return results;
}

static auto test_was_passed() -> supl::test_results
{
  supl::test_results results;

  constexpr static supl::named_params test {
    supl::tl::type_list<foo_type, foo_size> {}, foo_type::bar};

  results.enforce_true(test.was_passed<foo_type>());

  results.enforce_false(test.was_passed<foo_size>());

  return results;
}

static auto test_get() -> supl::test_results
{
  supl::test_results results;

  constexpr static supl::named_params test {
    supl::tl::type_list<foo_type, foo_size> {}, foo_type::bar};

  results.enforce_equal(test.get<foo_type>(), foo_type::bar);

  try {
    [[maybe_unused]] const auto& should_fail {test.get<foo_size>()};
    results.fail("Unchecked get did not throw as expected");
  } catch ( const supl::missing_named_parameter_exception& ) {
    // no-op to silence warning about empty catch block
    []() {}();
  }

  return results;
}

static auto test_get_or() -> supl::test_results
{
  supl::test_results results;

  constexpr static supl::named_params test {
    supl::tl::type_list<foo_type, foo_size> {}, foo_type::bar};

  results.enforce_equal(test.get_or<foo_type>(foo_type::third),
                        foo_type::bar);
  results.enforce_equal(test.get_or<foo_size>(3), std::size_t {3});

  return results;
}

auto test_named_params() -> supl::test_section
{
  supl::test_section section;

  section.add_test("example function", &test_example);
  section.add_test("named_params construction", &test_construction);
  section.add_test("named_params::was_passed", &test_was_passed);
  section.add_test("named_params::get", &test_get);
  section.add_test("named_params::get_or", &test_get_or);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_named_params());

  return runner.run();
}
