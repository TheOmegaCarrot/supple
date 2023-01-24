#include <type_traits>

#include <supl/metaprogramming.hpp>
#include <supl/tuple_algo.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>

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
