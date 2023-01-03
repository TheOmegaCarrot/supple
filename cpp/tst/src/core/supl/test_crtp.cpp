#include <sstream>
#include <string>

#include <supl/crtp.hpp>

#include <supl/test_runner.hpp>

static auto test_rel_ops() -> supl::test_results
{
  supl::test_results results;

  class rel : public supl::rel_ops<rel>
  {
  private:

    int m_value;

  public:

    explicit rel(int value)
        : m_value {value}
    { }

    auto operator==(const rel& rhs) const noexcept -> bool
    {
      return m_value == rhs.m_value;
    }

    auto operator<(const rel& rhs) const noexcept -> bool
    {
      return m_value < rhs.m_value;
    }
  };

  const rel rel_A {5};
  const rel rel_B {5};
  const rel rel_C {8};

  results.enforce_exactly_equal(rel_A == rel_B, true);
  results.enforce_exactly_equal(rel_A != rel_B, false);
  results.enforce_exactly_equal(rel_A < rel_B, false);
  results.enforce_exactly_equal(rel_A <= rel_B, true);
  results.enforce_exactly_equal(rel_A > rel_B, false);
  results.enforce_exactly_equal(rel_A >= rel_B, true);

  results.enforce_exactly_equal(rel_C == rel_B, false);
  results.enforce_exactly_equal(rel_C != rel_B, true);
  results.enforce_exactly_equal(rel_C < rel_B, false);
  results.enforce_exactly_equal(rel_C <= rel_B, false);
  results.enforce_exactly_equal(rel_C > rel_B, true);
  results.enforce_exactly_equal(rel_C >= rel_B, true);

  // and at compile-time!
  class ct_rel : public supl::rel_ops<ct_rel>
  {
  private:

    int m_value;

  public:

    constexpr explicit ct_rel(int value)
        : m_value {value}
    { }

    constexpr auto operator==(const ct_rel& rhs) const noexcept -> bool
    {
      return m_value == rhs.m_value;
    }

    constexpr auto operator<(const ct_rel& rhs) const noexcept -> bool
    {
      return m_value < rhs.m_value;
    }
  };

  constexpr static ct_rel ct_rel_A {5};
  constexpr static ct_rel ct_rel_B {5};
  constexpr static ct_rel ct_rel_C {8};

  constexpr static bool A_eq_B {ct_rel_A == ct_rel_B};
  constexpr static bool A_neq_B {ct_rel_A != ct_rel_B};
  constexpr static bool A_lt_B {ct_rel_A < ct_rel_B};
  constexpr static bool A_leq_B {ct_rel_A <= ct_rel_B};
  constexpr static bool A_gt_B {ct_rel_A > ct_rel_B};
  constexpr static bool A_geq_B {ct_rel_A >= ct_rel_B};
  constexpr static bool C_eq_B {ct_rel_C == ct_rel_B};
  constexpr static bool C_neq_B {ct_rel_C != ct_rel_B};
  constexpr static bool C_lt_B {ct_rel_C < ct_rel_B};
  constexpr static bool C_leq_B {ct_rel_C <= ct_rel_B};
  constexpr static bool C_gt_B {ct_rel_C > ct_rel_B};
  constexpr static bool C_geq_B {ct_rel_C >= ct_rel_B};

  results.enforce_exactly_equal(A_eq_B, true);
  results.enforce_exactly_equal(A_neq_B, false);
  results.enforce_exactly_equal(A_lt_B, false);
  results.enforce_exactly_equal(A_leq_B, true);
  results.enforce_exactly_equal(A_gt_B, false);
  results.enforce_exactly_equal(A_geq_B, true);
  results.enforce_exactly_equal(C_eq_B, false);
  results.enforce_exactly_equal(C_neq_B, true);
  results.enforce_exactly_equal(C_lt_B, false);
  results.enforce_exactly_equal(C_leq_B, false);
  results.enforce_exactly_equal(C_gt_B, true);
  results.enforce_exactly_equal(C_geq_B, true);

  return results;
}

static auto test_add_to_string() -> supl::test_results
{
  using std::literals::operator""s;

  supl::test_results results;

  struct consumer : supl::add_to_string<consumer> {
    int value {42};

    // NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
      out << value;
    }
  };

  results.enforce_exactly_equal(consumer {}.to_string(), "42"s);

  return results;
}

static auto test_add_ostream() -> supl::test_results
{
  using std::literals::operator""s;

  supl::test_results results;

  struct consumer_to_stream : supl::add_ostream<consumer_to_stream> {
    int value {42};

    // NOLINTNEXTLINE(*unused*)
    void to_stream(std::ostream& out) const noexcept
    {
      out << value;
    }
  };

  const consumer_to_stream test1 {};

  std::stringstream str1;
  str1 << test1;
  results.enforce_exactly_equal(str1.str(), "42"s);

  struct consumer_iterators : supl::add_ostream<consumer_iterators> {
    std::vector<int> value {1, 2, 42, 18};

    // NOLINTNEXTLINE(*unused*)
    [[nodiscard]] auto begin() const noexcept
    {
      return value.begin();
    }

    // NOLINTNEXTLINE(*unused*)
    [[nodiscard]] auto end() const noexcept
    {
      return value.end();
    }
  };

  const consumer_iterators test2 {};

  std::stringstream str2;
  str2 << test2;
  results.enforce_exactly_equal(str2.str(), "[ 1, 2, 42, 18 ]"s);

  struct consumer_both : supl::add_ostream<consumer_both> {
    std::vector<int> value {1, 2, 42, 18};

    [[nodiscard]] auto begin() const noexcept
    {
      return value.begin();
    }

    [[nodiscard]] auto end() const noexcept
    {
      return value.end();
    }

    void to_stream(std::ostream& out) const noexcept
    {
      out << value.front();
    }
  };

  const consumer_both test3 {};

  std::stringstream discard;
  test3.to_stream(discard);
  [[maybe_unused]] auto discard_begin {test3.begin()};
  [[maybe_unused]] auto discard_end {test3.end()};

  std::stringstream str3;
  str3 << test3;
  results.enforce_exactly_equal(str3.str(), "1"s);

  return results;
}

auto test_crtp() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::rel_ops", &test_rel_ops);
  section.add_test("supl::add_to_string", &test_add_to_string);
  section.add_test("supl::add_ostream", &test_add_ostream);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(test_crtp());

  return runner.run();
}
