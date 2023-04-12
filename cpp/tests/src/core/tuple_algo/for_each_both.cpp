#include "supl/test_runner.hpp"

#include "supl/tuple_algo.hpp"

template <typename... Ls>
struct overload : Ls... {
  using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

auto equal_size() -> supl::test_results
{
  supl::test_results results;

  std::tuple tup1 {1, 'g', true};
  std::tuple tup2 {8UL, 42U, nullptr};

  supl::tuple::for_each_both(
    tup1,
    tup2,
    overload {[&results](int a, unsigned long b) {
                results.enforce_equal(a, 1);
                results.enforce_equal(b, 8UL);
              },
              [&results](char a, unsigned b) {
                results.enforce_equal(a, 'g');
                results.enforce_equal(b, 42U);
              },
              [&results](bool a, std::nullptr_t b) {
                results.enforce_true(a);
                results.enforce_equal(b, nullptr);
              }});

  return results;
}

auto for_each_both() -> supl::test_section
{
  supl::test_section section;

  section.add_test("equal size", &equal_size);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(for_each_both());

  return runner.run();
}

