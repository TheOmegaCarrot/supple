#include <cstddef>

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

  constexpr static supl::named_params<foo_type, foo_size> test1 {
    foo_type::bar};

  results.enforce_true(test1.was_passed<foo_type>());

  results.enforce_false(test1.was_passed<foo_size>());

  return results;
}

auto test_named_params() -> supl::test_section
{
  supl::test_section section;

  section.add_test("named_params construction", &test_construction);
  section.add_test("named_params::was_passed", &test_was_passed);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_named_params());

  return runner.run();
}
