#include <array>
#include <deque>
#include <forward_list>
#include <iterator>
#include <numeric>
#include <vector>

#include <supl/iterators.hpp>
#include <supl/utility.hpp>

#include <supl/test_runner.hpp>

static auto test_last() -> supl::test_results
{
  supl::test_results results;

  const std::array<int, 5> test1 {1, 2, 3, 4, 5};
  const std::array<int, 2> test2 {2, 3};
  const std::vector<int> test3;
  const std::forward_list<int> test4 {1, 2, 3, 4};

  results.enforce_exactly_equal(*supl::last(test1), 5);
  results.enforce_exactly_equal(*supl::last(test2), 3);
  results.enforce_exactly_equal(
    supl::last(test3) == std::begin(test3), true
  );
  results.enforce_exactly_equal(*supl::last(test4), 4);

  return results;
}

static auto test_clast() -> supl::test_results
{
  supl::test_results results;

  const std::array<int, 5> test1 {1, 2, 3, 4, 5};
  const std::array<int, 2> test2 {2, 3};
  const std::vector<int> test3;
  const std::forward_list<int> test4 {1, 2, 3, 4};

  results.enforce_exactly_equal(*supl::clast(test1), 5);
  results.enforce_exactly_equal(*supl::clast(test2), 3);
  results.enforce_exactly_equal(
    supl::clast(test3) == std::cbegin(test3), true
  );
  results.enforce_exactly_equal(*supl::clast(test4), 4);

  return results;
}

static auto test_iterator() -> supl::test_results
{
  supl::test_results results;

  const std::vector test_vector {1, 2, 3, 4, 5};
  const std::deque test_deque {6, 7, 8, 9, 10};

  // increment and decrement
  supl::iterator test_1 {test_vector.begin()};
  results.enforce_exactly_equal(*test_1, 1);
  ++test_1;
  results.enforce_exactly_equal(*test_1, 2);
  --test_1;
  results.enforce_exactly_equal(*test_1, 1);

  static_assert(std::is_same_v<
                typename std::iterator_traits<decltype(test_1)>::reference,
                const int&>);

  supl::iterator test_2 {test_1++};
  results.enforce_exactly_equal(*test_1, 2);
  results.enforce_exactly_equal(*test_2, 1);

  ++test_2;
  const supl::iterator test_3 {test_2--};
  results.enforce_exactly_equal(*test_2, 1);
  results.enforce_exactly_equal(*test_3, 2);

  test_1 = test_deque.begin();
  results.enforce_exactly_equal(*test_1, 6);
  ++test_1;

  const supl::iterator test_4 {test_1};
  results.enforce_exactly_equal(*test_1, 7);
  results.enforce_exactly_equal(*test_4, 7);

  // equality
  const supl::iterator test_a {test_vector.begin()};
  supl::iterator test_b {test_vector.begin()};

  // same type - good cast
  results.enforce_exactly_equal(test_a == test_b, true);
  results.enforce_exactly_equal(test_a != test_b, false);
  ++test_b;

  // different type - bad cast
  test_b = test_deque.begin();
  results.enforce_exactly_equal(test_a == test_b, false);
  results.enforce_exactly_equal(test_a != test_b, true);

  // arrow operator

  struct foo {
    std::string m_foo {"foo"};

    [[nodiscard]] auto the_thing() const noexcept -> const std::string&
    {
      return m_foo;
    }
  };

  std::vector test_foo {foo {}};

  const supl::iterator test_foo_itr {test_foo.begin()};
  results.enforce_exactly_equal(
    test_foo_itr->the_thing(), std::string {"foo"}
  );

  // const?

  supl::iterator const_itr {test_vector.cbegin()};

  results.enforce_exactly_equal(
    const_itr.is_null(), false, "Should not be null"
  );

  static_assert(std::is_same_v<
                typename std::iterator_traits<decltype(const_itr
                )>::reference,
                const int&>);

  const_itr = test_vector.begin();
  results.enforce_exactly_equal(*const_itr, 1);

  // useful error message here - bad reassignment due to const incorrectness
  /* test_1 = test_iterable_1.cbegin(); */

  const std::vector test_iterable_3 {1.0, 2.0, 3.0, 4.0, 5.0};

  // useful error message here - bad reassignment due to type mismatch
  /* test_1 = test_iterable_3.begin(); */

  const supl::iterator constify {test_vector.begin()};
  results.enforce_exactly_equal(*constify, 1);

  // Null case

  const supl::iterator<int> nulled {};

  results.enforce_exactly_equal(nulled.is_null(), true, "Should be null");

  try {
    [[maybe_unused]] auto illegal {*nulled};
    results.enforce_exactly_equal(true, false, "Should not be reached");
    throw "This should be unreachable - supl::iterator null cast "
          "test";
  } catch ( supl::bad_iterator_access& e ) {
    results.enforce_exactly_equal<const char*>(
      e.what(),
      "Illegal access to null supl::iterator",
      "Incorrect error message"
    );
  }

  return results;
}

auto test_iterators() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::last", &test_last);
  section.add_test("supl::clast", &test_clast);
  section.add_test("supl::iterator", &test_iterator);

  return section;
}

static_assert(supl::is_iterator_tag_v<std::forward_iterator_tag>);
static_assert(supl::is_iterator_tag_v<std::bidirectional_iterator_tag>);
static_assert(supl::is_iterator_tag_v<std::random_access_iterator_tag>);
static_assert(not supl::is_iterator_tag_v<int>);
static_assert(not supl::is_iterator_tag_v<
              supl::stream_adapter<std::vector<int>>>);

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_iterators());

  return runner.run();
}
