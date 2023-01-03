#include <array>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <supl/utility.hpp>

#include <supl/test_runner.hpp>

struct copy_counter {
  copy_counter() = default;
  ~copy_counter() = default;
  copy_counter(copy_counter&&) = default;
  auto operator=(const copy_counter&) -> copy_counter& = default;
  auto operator=(copy_counter&&) -> copy_counter& = default;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static inline int copy_count {0};

  copy_counter([[maybe_unused]] const copy_counter& src)
  {
    ++copy_count;
  }
};

static auto test_explicit_copy() -> supl::test_results
{
  supl::test_results results;
  const copy_counter test;

  results.enforce_exactly_equal(
    copy_counter::copy_count, 0, "Counter not properly initialized"
  );

  [[maybe_unused]] const copy_counter test2 = supl::explicit_copy(test);

  results.enforce_exactly_equal(
    copy_counter::copy_count, 1, "Incorrect number of copies"
  );

  [[maybe_unused]] const copy_counter test3 = supl::explicit_copy(test);

  results.enforce_exactly_equal(
    copy_counter::copy_count, 2, "Incorrect number of copies"
  );

  [[maybe_unused]] const copy_counter test4 = supl::explicit_copy(test);

  results.enforce_exactly_equal(
    copy_counter::copy_count, 3, "Incorrect number of copies"
  );

  [[maybe_unused]] const copy_counter test5 = supl::explicit_copy(test);

  results.enforce_exactly_equal(
    copy_counter::copy_count, 4, "Incorrect number of copies"
  );

  return results;
}

static auto test_to_stream() -> supl::test_results
{
  supl::test_results results;

  using namespace std::literals;

  std::stringstream str1;
  const std::tuple test1 {1, "hello", true};
  supl::to_stream(str1, test1);
  results.enforce_exactly_equal(
    str1.str(), "( 1, hello, true )"s, "tuple"
  );
  str1 << true;
  results.enforce_exactly_equal(
    str1.str(), "( 1, hello, true )1"s, "fmtflags not reset correctly"
  );

  std::stringstream str2;
  const std::pair test2 {42, "Neat"s};
  supl::to_stream(str2, test2);
  results.enforce_exactly_equal(str2.str(), "( 42, Neat )"s, "pair");

  std::stringstream str3;
  const std::vector test3 {1, 2, 42, 81};
  supl::to_stream(str3, test3);
  results.enforce_exactly_equal(str3.str(), "[ 1, 2, 42, 81 ]"s, "vector");

  std::stringstream str4;
  const std::list<std::pair<int, bool>> test4 {
    {1,  true},
    {2, false},
    {5,  true}
  };
  supl::to_stream(str4, test4);
  results.enforce_exactly_equal(
    str4.str(),
    "[ ( 1, true ), ( 2, false ), ( 5, true ) ]"s,
    "List of tuples"
  );

  std::stringstream str5;
  supl::to_stream(str5, 1);
  results.enforce_exactly_equal(str5.str(), "1"s, "int");

  std::stringstream str6;
  supl::to_stream(str6, std::vector<int> {});
  results.enforce_exactly_equal(str6.str(), "[ ]"s, "empty vector");

  std::stringstream str7;
  supl::to_stream(str7, std::tuple<> {});
  results.enforce_exactly_equal(str7.str(), "( )"s);

  supl::to_stream(str7, true);
  results.enforce_exactly_equal(str7.str(), "( )true"s);

  str7 << true;
  results.enforce_exactly_equal(str7.str(), "( )true1"s);

  std::stringstream str8;
  supl::to_stream(str8, std::tuple<int> {5});
  results.enforce_exactly_equal(str8.str(), "( 5 )"s);

  std::stringstream str9;
  const std::tuple test9 {test1, test3, false};
  supl::to_stream(str9, test9);

  results.enforce_exactly_equal(
    str9.str(), "( ( 1, hello, true ), [ 1, 2, 42, 81 ], false )"s
  );

  struct has_a_to_stream {
    int value {5};

    void to_stream(std::ostream& out) const noexcept
    {
      out << "works: " << value;
    }
  };

  std::stringstream str10;
  const has_a_to_stream test10 {};
  supl::to_stream(str10, test10);

  results.enforce_exactly_equal(str10.str(), "works: 5"s);

  std::stringstream str11;
  test10.to_stream(str11);
  results.enforce_exactly_equal(str11.str(), "works: 5"s);

  return results;
}

static auto test_to_string() -> supl::test_results
{
  supl::test_results results;

  using namespace std::literals;

  const std::tuple test1 {1, "hello", true};
  results.enforce_exactly_equal(
    supl::to_string(test1), "( 1, hello, true )"s, "tuple"
  );

  const std::pair test2 {42, "Neat"s};
  results.enforce_exactly_equal(
    supl::to_string(test2), "( 42, Neat )"s, "pair"
  );

  const std::vector test3 {1, 2, 42, 81};
  results.enforce_exactly_equal(
    supl::to_string(test3), "[ 1, 2, 42, 81 ]"s, "vector"
  );

  const std::list<std::pair<int, bool>> test4 {
    {1,  true},
    {2, false},
    {5,  true}
  };
  results.enforce_exactly_equal(
    supl::to_string(test4),
    "[ ( 1, true ), ( 2, false ), ( 5, true ) ]"s,
    "List of tuples"
  );

  results.enforce_exactly_equal(supl::to_string(1), "1"s, "int");

  results.enforce_exactly_equal(
    supl::to_string(std::vector<int> {}), "[ ]"s, "empty vector"
  );

  results.enforce_exactly_equal(supl::to_string(std::tuple<> {}), "( )"s);

  results.enforce_exactly_equal(
    supl::to_string(std::tuple<int> {5}), "( 5 )"s
  );

  const std::tuple test5 {test1, test3, false};

  results.enforce_exactly_equal(
    supl::to_string(test5),
    "( ( 1, hello, true ), [ 1, 2, 42, 81 ], false )"s
  );

  const std::string empty_container {"[ ]"};
  results.enforce_exactly_equal(
    supl::to_string(std::list<int> {}), empty_container
  );
  results.enforce_exactly_equal(
    supl::to_string(std::vector<int> {}), empty_container
  );
  results.enforce_exactly_equal(
    supl::to_string(std::array<int, 0> {}), empty_container
  );
  /* results.enforce_exactly_equal(supl::to_string(std::deque<int> {}), empty_container); */
  /* results.enforce_exactly_equal(supl::to_string(std::forward_list<int> {}), */
  /*                  empty_container); */
  results.enforce_exactly_equal(
    supl::to_string(std::map<int, char> {}), empty_container
  );
  results.enforce_exactly_equal(
    supl::to_string(std::unordered_map<int, char> {}), empty_container
  );
  results.enforce_exactly_equal(
    supl::to_string(std::multimap<int, char> {}), empty_container
  );
  results.enforce_exactly_equal(
    supl::to_string(std::unordered_multimap<int, char> {}), empty_container
  );
  /* results.enforce_exactly_equal(supl::to_string(std::unordered_set<int> {}), */
  /*                  empty_container); */
  results.enforce_exactly_equal(
    supl::to_string(std::set<int> {}), empty_container
  );
  /* results.enforce_exactly_equal(supl::to_string(std::unordered_multiset<int> {}), */
  /*                  empty_container); */
  results.enforce_exactly_equal(
    supl::to_string(std::multiset<int> {}), empty_container
  );

  results.enforce_exactly_equal(
    supl::to_string(std::monostate {}), "<std::monostate>"s
  );

  // NOLINTNEXTLINE
  struct force_valueless_by_exception {

    // NOLINTNEXTLINE
    force_valueless_by_exception()
    { }

    // std::variant is only guaranteed to be valueless by exception
    // only if an exception is thrown during the move
    // initialization of the contained value during
    // move assignment
    // source: https://en.cppreference.com/w/cpp/utility/variant/valueless_by_exception
    // NOLINTNEXTLINE
    [[noreturn]] force_valueless_by_exception(force_valueless_by_exception&&)
    {
      throw 3.14;
    }

    // NOLINTNEXTLINE
    static void to_stream(std::ostream& out)
    {
      out << "This text should not appear.";
    }
  };

  std::variant<
    std::monostate,
    int,
    std::vector<int>,
    std::tuple<int, char, bool>,
    force_valueless_by_exception>
    test_variant {};

  results.enforce_exactly_equal(
    supl::to_string(test_variant), "<std::monostate>"s
  );

  test_variant = 5;
  results.enforce_exactly_equal(supl::to_string(test_variant), "5"s);

  test_variant = std::vector {1, 2, 3, 42};
  results.enforce_exactly_equal(
    supl::to_string(test_variant), "[ 1, 2, 3, 42 ]"s
  );

  test_variant = std::tuple {5, 'g', false};
  results.enforce_exactly_equal(
    supl::to_string(test_variant), "( 5, g, false )"s
  );

  try {
    force_valueless_by_exception forcer {};
    test_variant.emplace<force_valueless_by_exception>(std::move(forcer));
  } catch ( ... ) { }

  results.enforce_exactly_equal(
    supl::to_string(test_variant), "<valueless_by_exception>"s
  );

  return results;
}

struct uncopiable {
  int m_value;
  uncopiable() = delete;

  explicit uncopiable(int value)
      : m_value(value)
  { }

  uncopiable(const uncopiable&) = delete;
  uncopiable(uncopiable&&) noexcept = default;
  auto operator=(const uncopiable&) = delete;
  auto operator=(uncopiable&&) noexcept -> uncopiable& = default;
  ~uncopiable() = default;

  friend inline auto
  operator<<(std::ostream& out, const uncopiable& rhs) noexcept
    -> std::ostream&
  {
    out << rhs.m_value;
    return out;
  }
};

static auto test_stream_adapter() -> supl::test_results
{
  supl::test_results results;

  const std::vector test_input_vec {4, 8, 3};
  const std::map<char, int> test_input_map {
    {'a', 1},
    {'b', 2},
    {'c', 3},
    {'d', 4}
  };

  const std::string beans {"Beans"};

  uncopiable neat {7};

  const std::tuple test_input_tup {
    true,
    test_input_vec,
    std::move(neat),
    42,
    std::tuple {81, 3.14, false},
    test_input_map,
    'u',
    beans
  };

  const std::string expected1 {supl::to_string(test_input_tup)};

  std::stringstream test_stream;
  test_stream << supl::stream_adapter(test_input_tup);
  const std::string result1 {test_stream.str()};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_size_t_literals() -> supl::test_results
{
  supl::test_results results;

  using supl::size_t_literal::operator""_z;
  /* using namespace supl::literals::size_t_literal; */  // also works
  /* using namespace supl::literals; */  // also works
  const std::size_t i {500};
  const auto j {500_z};

  static_assert(std::is_same_v<decltype(j), decltype(i)>);

  results.enforce_exactly_equal(i, j, "Value is not as expected");

  return results;
}

static auto test_ptrdiff_t_literals() -> supl::test_results
{
  supl::test_results results;

  using supl::ptrdiff_t_literal::operator""_pd;
  /* using namespace supl::literals::ptrdiff_t_literal; */  // also works
  /* using namespace supl::literals; */  // also works
  const std::ptrdiff_t i {500};
  const auto j {500_pd};

  static_assert(std::is_same_v<decltype(j), decltype(i)>);

  results.enforce_exactly_equal(i, j, "Value is not as expected");

  const auto k {-500_pd};

  results.enforce_exactly_equal(-i, k, "Value is not as expected");
  results.enforce_exactly_equal(0 - i, k, "Value is not as expected");

  return results;
}

auto test_utility() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::explicit_copy", &test_explicit_copy);
  section.add_test("supl::to_stream", &test_to_stream);
  section.add_test("supl::to_string", &test_to_string);
  section.add_test("supl::stream_adapter", &test_stream_adapter);
  section.add_test(
    "supl::literals::size_t_literal::operator\"\"_z", &test_size_t_literals
  );
  section.add_test(
    "supl::literals::ptrdiff_t_literal::operator\"\"_pd",
    &test_ptrdiff_t_literals
  );

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_utility());

  return runner.run();
}
