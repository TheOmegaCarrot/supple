#include <type_traits>

#include <supl/metaprogramming.hpp>
#include <supl/tuple_algo.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>

static auto test_split() -> supl::test_results
{
  supl::test_results results;

  // halves
  const std::string split {"split"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple test_input {3, 3.14, vec, true, 'f', split};

  const std::tuple expected1_first {3, 3.14, vec};
  const std::tuple expected1_second {true, 'f', split};
  const auto result1 {supl::tuple::split<3>(test_input)};

  results.enforce_exactly_equal(result1.first, expected1_first);
  results.enforce_exactly_equal(result1.second, expected1_second);

  // near_end
  const std::tuple expected2_first {3, 3.14, vec, true, 'f'};
  const std::tuple expected2_second {split};
  const auto result2 {supl::tuple::split<5>(test_input)};

  results.enforce_exactly_equal(result2.first, expected2_first);
  results.enforce_exactly_equal(result2.second, expected2_second);

  // references
  int ref_test {42};
  const std::tuple<int&, char, bool, double, int&> test_ref_input {
    ref_test, 'y', false, 3.14, ref_test};
  const std::tuple<int&, char, bool> expected3_first {
    ref_test, 'y', false};
  const std::tuple<double, int&> expected3_second {3.14, ref_test};

  const auto results3 {supl::tuple::split<3>(test_ref_input)};

  results.enforce_exactly_equal(results3.first, expected3_first);
  results.enforce_exactly_equal(results3.second, expected3_second);

  // at_beginning
  const std::tuple<> expected4_first {};
  const auto& expected4_second {test_input};
  const auto result4 {supl::tuple::split<0>(test_input)};

  results.enforce_exactly_equal(result4.first, expected4_first);
  results.enforce_exactly_equal(result4.second, expected4_second);

  // at_end
  const auto& expected5_first {test_input};
  const std::tuple<> expected5_second {};
  const auto result5 {
    supl::tuple::split<std::tuple_size_v<decltype(test_input)>>(
      test_input)};

  results.enforce_exactly_equal(result5.first, expected5_first);
  results.enforce_exactly_equal(result5.second, expected5_second);

  return results;
}

static auto test_reorder() -> supl::test_results
{
  supl::test_results results;

  // simple
  const std::string reorder {"reorder"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple test_input {3, 3.14, true, reorder, vec};
  const std::tuple expected1 {3.14, reorder, 3, 3.14, vec, true};

  const auto result1 {supl::tuple::reorder<1, 3, 0, 1, 4, 2>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  // references
  const std::
    tuple<int, const std::vector<int>&, const std::string&, char, bool>
      test_ref_input {42, vec, reorder, 'j', true};

  const std::tuple<const std::vector<int>&,
                   bool,
                   char,
                   const std::string&,
                   const std::vector<int>&,
                   int>
    expected2 {vec, true, 'j', reorder, vec, 42};

  const auto result2 {
    supl::tuple::reorder<1, 4, 3, 2, 1, 0>(test_ref_input)};

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_reverse() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, 'g', 3.14, true, 'D'};
  const std::tuple expected1 {'D', true, 3.14, 'g', 42};
  const auto result1 {supl::tuple::reverse(test_input)};
  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_subtuple() -> supl::test_results
{
  supl::test_results results;

  const std::string str {"subtuple"};
  const std::vector vec {1, 2, 3, 4};
  const std::tuple test_input {3, true, str, 3.14, vec};

  const std::tuple expected1 {str, 3.14, vec};
  const auto result1 {supl::tuple::subtuple<2, 5>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {test_input};
  const auto result2 {
    supl::tuple::subtuple<0, std::tuple_size_v<decltype(test_input)>>(
      test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {3, true, str};
  const auto result3 {supl::tuple::subtuple<0, 3>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  const std::tuple expected4 {3, true, str};
  const auto result4 {supl::tuple::subtuple<0, 3>(test_input)};

  results.enforce_exactly_equal(result4, expected4);

  const std::tuple expected5 {str, 3.14, vec};
  const auto result5 {supl::tuple::subtuple<2, 5>(test_input)};

  results.enforce_exactly_equal(result5, expected5);

  const std::tuple expected6 {str, 3.14, vec};
  const auto result6 {supl::tuple::subtuple<2, 5>(test_input)};

  results.enforce_exactly_equal(result6, expected6);

  int ref_test {42};
  const std::tuple<char, bool, int&, double, char> test_ref_input {
    'j', true, ref_test, 3.14, 'l'};
  const std::tuple<bool, int&, double> expected7 {true, ref_test, 3.14};
  const auto result7 {supl::tuple::subtuple<1, 4>(test_ref_input)};

  results.enforce_exactly_equal(result7, expected7);

  const std::tuple expected8 {3, true, str};
  const auto result8 {supl::tuple::subtuple<0, 3>(test_input)};

  results.enforce_exactly_equal(result8, expected8);

  return results;
}

static auto test_count_if() -> supl::test_results
{
  using supl::literals::size_t_literal::operator""_z;

  supl::test_results results;

  const std::tuple test {7, 2, 42.53, 3.14F, 9344285UL, -83LL};
  results.enforce_exactly_equal(
    supl::tuple::count_if(test,
                          [](const auto& i) -> std::size_t {
                            return i > 5;
                          }),
    3_z);

  return results;
}

static auto test_interleave() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test1_1 {42, 'g', 3.14};
  const std::tuple test1_2 {2.6F, true, 8L};

  const std::tuple expected1 {42, 2.6F, 'g', true, 3.14, 8L};
  const auto result1 {supl::tuple::interleave(test1_1, test1_2)};

  results.enforce_exactly_equal(result1, expected1);

  using std::string_literals::operator""s;

  const std::tuple test2_1 {42, 'g', 3.14, 81, 5.31F, 4UL, false, '7'};
  const std::tuple test2_2 {
    2.6F, true, 8L, 8.3F, 2U, 'u', "yes"s, std::vector {3, 5, 7}
  };

  const std::tuple expected2 {
    42,
    2.6F,
    'g',
    true,
    3.14,
    8L,
    81,
    8.3F,
    5.31F,
    2U,
    4UL,
    'u',
    false,
    "yes"s,
    '7',
    std::vector {3, 5, 7}
  };
  const auto result2 {supl::tuple::interleave(test2_1, test2_2)};

  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_tuple_alternating_split() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input_1 {1, 'J', true, 3.14, 'c', 42};
  const std::tuple expected1_first {1, true, 'c'};
  const std::tuple expected1_second {'J', 3.14, 42};

  const auto [result1_first, result1_second] {
    supl::tuple::alternating_split(test_input_1)};

  results.enforce_exactly_equal(result1_first, expected1_first);
  results.enforce_exactly_equal(result1_second, expected1_second);

  const std::tuple test_input_2 {1, true, 'c', 3.14, 42};
  const std::tuple expected2_first {1, 'c'};
  const std::tuple expected2_second {true, 3.14};
  const auto [result2_first, result2_second] {
    supl::tuple::alternating_split(test_input_2)};

  results.enforce_exactly_equal(result2_first, expected2_first);
  results.enforce_exactly_equal(result2_second, expected2_second);

  return results;
}

static auto test_front_n() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, '&', 3.14};

  const std::tuple<> expected0 {};
  const auto result0 {supl::tuple::front_n<0>(test_input)};

  results.enforce_exactly_equal(result0, expected0);

  const std::tuple expected1 {42};
  const auto result1 {supl::tuple::front_n<1>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {42, '&'};
  const auto result2 {supl::tuple::front_n<2>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {42, '&', 3.14};
  const auto result3 {supl::tuple::front_n<3>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_front() -> supl::test_results
{
  supl::test_results results;

  // NOLINTNEXTLINE(*const*)
  std::tuple not_const {42, 'g', 3.14};

  static_assert(
    std::is_same_v<int&, decltype(supl::tuple::front(not_const))>);
  results.enforce_equal(supl::tuple::front(not_const), 42);

  // NOLINTNEXTLINE(*const*)
  std::tuple<const int, char, double> const_element {42, 'g', 3.14};

  static_assert(
    std::is_same_v<const int&,
                   decltype(supl::tuple::front(const_element))>);
  results.enforce_equal(supl::tuple::front(const_element), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple const_tuple {42, 'g', 3.14};

  static_assert(
    std::is_same_v<const int&, decltype(supl::tuple::front(const_tuple))>);
  results.enforce_equal(supl::tuple::front(const_tuple), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple<const int, char, double> const_tuple_const_element {
    42, 'g', 3.14};

  static_assert(std::is_same_v<const int&,
                               decltype(supl::tuple::front(
                                 const_tuple_const_element))>);
  results.enforce_equal(supl::tuple::front(const_tuple_const_element), 42);

  return results;
}

static auto test_back_n() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, '&', 3.14};

  const std::tuple<> expected0 {};
  const auto result0 {supl::tuple::back_n<0>(test_input)};

  results.enforce_exactly_equal(result0, expected0);

  const std::tuple expected1 {3.14};
  const auto result1 {supl::tuple::back_n<1>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {'&', 3.14};
  const auto result2 {supl::tuple::back_n<2>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {42, '&', 3.14};
  const auto result3 {supl::tuple::back_n<3>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  return results;
}

static auto test_back() -> supl::test_results
{
  supl::test_results results;

  // NOLINTNEXTLINE(*const*)
  std::tuple not_const {3.14, 'g', 42};

  static_assert(
    std::is_same_v<int&, decltype(supl::tuple::back(not_const))>);
  results.enforce_equal(supl::tuple::back(not_const), 42);

  // NOLINTNEXTLINE(*const*)
  std::tuple<double, char, const int> const_element {3.14, 'g', 42};

  static_assert(
    std::is_same_v<const int&,
                   decltype(supl::tuple::back(const_element))>);
  results.enforce_equal(supl::tuple::back(const_element), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple const_tuple {3.14, 'g', 42};

  static_assert(
    std::is_same_v<const int&, decltype(supl::tuple::back(const_tuple))>);
  results.enforce_equal(supl::tuple::back(const_tuple), 42);

  // NOLINTNEXTLINE(*const*)
  const std::tuple<double, char, const int> const_tuple_const_element {
    3.14, 'g', 42};

  static_assert(std::is_same_v<const int&,
                               decltype(supl::tuple::back(
                                 const_tuple_const_element))>);
  results.enforce_equal(supl::tuple::back(const_tuple_const_element), 42);

  return results;
}

static auto test_elem_swap() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, 'g', 3.14, true, 18UL, 2.7F};

  const std::tuple expected1 {42, 18UL, 3.14, true, 'g', 2.7F};
  const auto result1 {supl::tuple::elem_swap<1, 4>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  const std::tuple expected2 {42, 'g', 3.14, true, 18UL, 2.7F};
  const auto result2 {supl::tuple::elem_swap<3, 3>(test_input)};

  results.enforce_exactly_equal(result2, expected2);

  const std::tuple expected3 {2.7F, 'g', 3.14, true, 18UL, 42};
  const auto result3 {supl::tuple::elem_swap<0, 5>(test_input)};

  results.enforce_exactly_equal(result3, expected3);

  const std::tuple expected4 {42, 18UL, 3.14, true, 'g', 2.7F};
  const auto result4 {supl::tuple::elem_swap<4, 1>(test_input)};

  results.enforce_exactly_equal(result4, expected4);

  return results;
}

static auto test_type_transform() -> supl::test_results
{
  supl::test_results results;

  const std::tuple test_input {42, 3.14, true};

  const std::tuple<const int&, const double&, const bool&> expected1 {
    std::get<0>(test_input),
    std::get<1>(test_input),
    std::get<2>(test_input)};

  const auto result1 {
    supl::tuple::type_transform<supl::make_const_ref>(test_input)};

  results.enforce_exactly_equal(result1, expected1);

  results.enforce_exactly_equal<const int*>(&std::get<0>(test_input),
                                            &std::get<0>(result1));

  results.enforce_exactly_equal<const double*>(&std::get<1>(test_input),
                                               &std::get<1>(result1));

  results.enforce_exactly_equal<const bool*>(&std::get<2>(test_input),
                                             &std::get<2>(result1));

  const auto& test_input_2 {expected1};
  const auto& expected2 {test_input};
  const auto result2 {
    supl::tuple::type_transform<std::decay>(test_input_2)};
  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_convert() -> supl::test_results
{
  struct just_a_bool {
    // NOLINTNEXTLINE(google-explicit-constructor)
    just_a_bool(bool arg)
        : value {arg}
    { }

    bool value;

    // NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
      out << value;
    }

    // NOLINTNEXTLINE(*unused*)
    [[nodiscard]] constexpr auto operator==(const just_a_bool& rhs) const
      -> bool
    {
      return this->value == rhs.value;
    }
  };

  using supl::literals::size_t_literal::operator""_z;
  supl::test_results results;

  constexpr static std::tuple test_input {42, 3.14, true};

  constexpr static std::tuple expected1 {42.0, 3_z, 1};
  constexpr static auto result1 {
    supl::tuple::convert<double, std::size_t, int>(test_input)};
  results.enforce_exactly_equal(result1, expected1);
  const std::tuple<char, double, just_a_bool> expected2 {
    '*', 3.14, just_a_bool {true}};
  const auto result2 {
    supl::tuple::convert<char, double, just_a_bool>(test_input)};
  results.enforce_exactly_equal(result2, expected2);

  return results;
}

static auto test_resolve_refs() -> supl::test_results
{
  supl::test_results results;

  // NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
  class copy_counter
  {
  private:

    std::size_t m_count {0};

  public:

    copy_counter() = default;

    explicit copy_counter(std::size_t count)
        : m_count {count}
    { }

    copy_counter(const copy_counter& src) noexcept
        : m_count {src.m_count + 1}
    { }

    copy_counter(copy_counter&& src) = default;

    auto operator=(const copy_counter& rhs) noexcept -> copy_counter&
    {
      if ( &rhs != this ) {
        this->m_count = rhs.m_count + 1;
      }
      return *this;
    }

    /* ~copy_counter()                                 = default; */

    //NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
      out << "Copies: " << m_count;
    }

    //NOLINTNEXTLINE(*unused*)
    auto operator==(const copy_counter& rhs) const noexcept -> bool
    {
      return m_count == rhs.m_count;
    }
  };

  const std::tuple test_input {42, 3.14, '&', copy_counter {}};
  const std::tuple intermediate1_1 {
    supl::tuple::type_transform<supl::make_const_ref>(test_input)};
  const int i1 {42};
  const std::tuple intermediate1_2 {
    supl::tuple::push_back_as<const int&>(intermediate1_1, i1)};
  const double d1 {3.14};
  const std::tuple intermediate1_3 {
    supl::tuple::push_back_as<const double&>(intermediate1_2, d1)};
  const char c1 {'|'};
  const std::tuple intermediate1_4 {
    supl::tuple::push_back_as<const char&>(intermediate1_3, c1)};
  const bool b1 {false};
  const std::tuple intermediate1_5 {
    supl::tuple::push_back_as<const bool&>(intermediate1_4, b1)};
  const std::tuple result1 {supl::tuple::resolve_refs(intermediate1_5)};

  const std::
    tuple<int, double, char, copy_counter, int, double, char, bool>
      expected1 {42, 3.14, '&', copy_counter {1}, 42, 3.14, '|', false};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

static auto test_make_cref_tuple() -> supl::test_results
{
  supl::test_results results;

  const int a {5};
  const char b {'b'};
  const bool c {true};
  const std::tuple<const int&, const char&, const bool&> expected1 {
    a, b, c};
  const auto result1 {supl::tuple::make_cref_tuple(a, b, c)};

  results.enforce_exactly_equal(result1, expected1);

  return results;
}

auto test_tuple_algo() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::tuple::reorder", &test_reorder);
  section.add_test("supl::tuple::reverse", &test_reverse);
  section.add_test("supl::tuple::split", &test_split);
  section.add_test("supl::tuple::subtuple", &test_subtuple);
  section.add_test("supl::tuple::count_if", &test_count_if);
  section.add_test("supl::tuple::interleave", &test_interleave);
  section.add_test("supl::tuple::alternating_split",
                   &test_tuple_alternating_split);
  section.add_test("supl::tuple::front_n", &test_front_n);
  section.add_test("supl::tuple::front", &test_front);
  section.add_test("supl::tuple::back_n", &test_back_n);
  section.add_test("supl::tuple::back", &test_back);
  section.add_test("supl::tuple::elem_swap", &test_elem_swap);
  section.add_test("supl::tuple::type_transform", &test_type_transform);
  section.add_test("supl::tuple::convert", &test_convert);
  section.add_test("supl::tuple::resolve_refs", &test_resolve_refs);
  section.add_test("supl::tuple::make_cref_tuple", &test_make_cref_tuple);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_tuple_algo());

  return runner.run();
}
