#include <vector>

#include <supl/fake_ranges.hpp>

#include <supl/test_runner.hpp>

/* static auto test_all_of() -> supl::test_results */
/* { */
/*   supl::test_results results; */

/*   const std::vector cvec {2, 3, 4, 5, 2}; */
/*   std::vector vec {2, 3, 4, 5, 2}; */

/*   results.enforce_exactly_equal(supl::fr::all_of(vec, [](auto&& i) { return i > 1; }), */
/*                    true); */
/*   results.enforce_exactly_equal(supl::fr::all_of(cvec, [](auto&& i) { return i > 1; }), */
/*                    true); */
/*   results.enforce_exactly_equal(supl::fr::all_of(vec, [](auto&& i) { return i > 3; }), */
/*                    false); */
/*   results.enforce_exactly_equal(supl::fr::all_of(cvec, [](auto&& i) { return i > 3; }), */
/*                    false); */

/*   return results; */
/* } */

/* static auto test_any_of() -> supl::test_results */
/* { */
/*   supl::test_results results; */

/*   const std::vector cvec {2, 3, 4, 5, 2}; */
/*   std::vector vec {2, 3, 4, 5, 2}; */

/*   results.enforce_exactly_equal(supl::fr::any_of(vec, [](auto&& i) { return i > 1; }), */
/*                    true); */
/*   results.enforce_exactly_equal(supl::fr::any_of(cvec, [](auto&& i) { return i > 1; }), */
/*                    true); */
/*   results.enforce_exactly_equal(supl::fr::any_of(vec, [](auto&& i) { return i > 9; }), */
/*                    false); */
/*   results.enforce_exactly_equal(supl::fr::any_of(cvec, [](auto&& i) { return i > 9; }), */
/*                    false); */

/*   return results; */
/* } */

/* static auto test_none_of() -> supl::test_results */
/* { */
/*   supl::test_results results; */

/*   const std::vector cvec {2, 3, 4, 5, 2}; */
/*   std::vector vec {2, 3, 4, 5, 2}; */

/*   results.enforce_exactly_equal(supl::fr::none_of(vec, [](auto&& i) { return i < 1; }), */
/*                    true); */
/*   results.enforce_exactly_equal(supl::fr::none_of(cvec, [](auto&& i) { return i < 1; }), */
/*                    true); */
/*   results.enforce_exactly_equal(supl::fr::none_of(vec, [](auto&& i) { return i < 9; }), */
/*                    false); */
/*   results.enforce_exactly_equal(supl::fr::none_of(cvec, [](auto&& i) { return i < 9; }), */
/*                    false); */

/*   return results; */
/* } */

/* static auto test_count() -> supl::test_results */
/* { */
/*   supl::test_results results; */

/*   const std::vector cvec {2, 3, 4, 5, 5}; */

/*   auto count {supl::fr::count(cvec, 5)}; */
/*   auto correct {static_cast<decltype(count)>(2)}; */

/*   results.enforce_exactly_equal(count, correct); */

/*   return results; */
/* } */

static auto test_iota() -> supl::test_results
{
  supl::test_results results;

  constexpr static std::array<std::size_t, 10> expected1 {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  constexpr static auto result1 {[]() {
    std::array<std::size_t, 10> retval {};
    for ( const std::size_t i : supl::fr::iota<std::size_t> {1, 11} ) {
      retval.at(i - 1) = i;
    }
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result1, expected1);

  const std::array<std::size_t, 10> expected2 {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result2 {[]() {
    std::array<std::size_t, 10> retval {};
    std::copy(supl::fr::iota<std::size_t>::iterator {1},
              supl::fr::iota<std::size_t>::iterator {11},
              retval.begin());
    return retval;
  }()};  // IILE

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

auto test_fake_ranges() -> supl::test_section
{
  supl::test_section section;

  // Do the algorithms *really* need rigorous testing?
  // Literally just forwarding arguments
  /* section.add_test("supl::fr::all_of", &test_all_of); */
  /* section.add_test("supl::fr::any_of", &test_any_of); */
  /* section.add_test("supl::fr::none_of", &test_none_of); */
  /* section.add_test("supl::fr::for_each", &test_for_each); */
  /* section.add_test("supl::fr::for_each_n", &test_for_each_n); */
  /* section.add_test("supl::fr::count", &test_count); */
  /* section.add_test("supl::fr::count_if", &test_count_if); */
  section.add_test("supl::fr::iota", &test_iota);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_fake_ranges());

  return runner.run();
}
