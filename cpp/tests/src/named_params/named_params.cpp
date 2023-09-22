#include <cstddef>
#include <iostream>

#include "supl/named_params.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"

struct foo_size {
  std::size_t value;
};

enum struct foo_type : char {
  foo,
  bar,
  third
};

template<>
void supl::to_stream<foo_type>(std::ostream& out, const foo_type& value) noexcept
{
  switch(value) {
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

struct foo_count {
  std::size_t value;
};

static auto test_construction() -> supl::test_results
{
  supl::test_results results;

  // just testing the "happy path" of construction in this function
  //
  // constexpr construction of the object without a compilation error
  // is the purpose of this test function
  using np = supl::named_params<foo_size, foo_type, foo_count>;

  [[maybe_unused]] constexpr static np test1 {
    foo_type::bar, foo_size {5}, foo_count {16}};

  [[maybe_unused]] constexpr static np test2 {
    foo_size {8}, foo_count {500}, foo_type::foo};

  [[maybe_unused]] constexpr static np test3 {
    foo_count {508}, foo_type::third, foo_size {12}};

  [[maybe_unused]] constexpr static supl::named_params<foo_type> test4 {
    foo_type::foo};

  [[maybe_unused]] constexpr static supl::named_params<foo_size> test5 {
    foo_size {12}};

  [[maybe_unused]] constexpr static supl::named_params<foo_count> test6 {
    foo_count {12}};

  [[maybe_unused]] constexpr static supl::named_params<> test7 {};

  [[maybe_unused]] constexpr static np test8 {};

  // now without constexpr

  [[maybe_unused]] np test10 {foo_type::bar, foo_size {5}, foo_count {16}};

  [[maybe_unused]] np test20 {
    foo_size {8}, foo_count {500}, foo_type::foo};

  [[maybe_unused]] np test30 {
    foo_count {508}, foo_type::third, foo_size {12}};

  [[maybe_unused]] supl::named_params<foo_type> test40 {foo_type::foo};

  [[maybe_unused]] supl::named_params<foo_size> test50 {foo_size {12}};

  [[maybe_unused]] supl::named_params<foo_count> test60 {foo_count {12}};

  [[maybe_unused]] supl::named_params<> test70 {};

  [[maybe_unused]] np test80 {};

  return results;
}

static auto test_was_passed() -> supl::test_results
{
  supl::test_results results;

  constexpr static supl::named_params<foo_type, foo_size> test {
    foo_type::bar};

  results.enforce_true(test.was_passed<foo_type>());

  results.enforce_false(test.was_passed<foo_size>());

  return results;
}

static auto test_get() -> supl::test_results
{
  supl::test_results results;

  constexpr static supl::named_params<foo_type, foo_size> test {
    foo_type::bar};

  results.enforce_equal(test.get<foo_type>(), foo_type::bar);

  try {
    [[maybe_unused]] const auto& should_fail {test.get<foo_size>()};
    results.fail("Unchecked get did not throw as expected");
  } catch(const supl::missing_named_parameter_exception&) {
    // no-op to silence warning about empty catch block
    [](){}();
  }

  return results;
}

static auto test_get_or() -> supl::test_results
{
  supl::test_results results;

  constexpr static supl::named_params<foo_type, foo_size> test {
    foo_type::bar};

  results.enforce_equal(test.get_or<foo_type>(foo_type::third), foo_type::bar);
  results.enforce_equal(test.get_or<foo_size>(foo_size{3}), foo_size{3});

  return results;
}

auto test_named_params() -> supl::test_section
{
  supl::test_section section;

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
