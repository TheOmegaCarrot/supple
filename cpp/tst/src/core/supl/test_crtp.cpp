#include <sstream>
#include <string>

#include <supl/crtp.hpp>

#include <supl/test_results.hpp>
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

  results.enforce_true(rel_A == rel_B);
  results.enforce_false(rel_A != rel_B);
  results.enforce_false(rel_A < rel_B);
  results.enforce_true(rel_A <= rel_B);
  results.enforce_false(rel_A > rel_B);
  results.enforce_true(rel_A >= rel_B);

  results.enforce_false(rel_C == rel_B);
  results.enforce_true(rel_C != rel_B);
  results.enforce_false(rel_C < rel_B);
  results.enforce_false(rel_C <= rel_B);
  results.enforce_true(rel_C > rel_B);
  results.enforce_true(rel_C >= rel_B);

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

  results.enforce_true(A_eq_B);
  results.enforce_false(A_neq_B);
  results.enforce_false(A_lt_B);
  results.enforce_true(A_leq_B);
  results.enforce_false(A_gt_B);
  results.enforce_true(A_geq_B);
  results.enforce_false(C_eq_B);
  results.enforce_true(C_neq_B);
  results.enforce_false(C_lt_B);
  results.enforce_false(C_leq_B);
  results.enforce_true(C_gt_B);
  results.enforce_true(C_geq_B);

  return results;
}

// friend functions cannot be defined in a local class

class sym_rel : public supl::sym_rel_ops<sym_rel>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  sym_rel(int value)
      : m_value {value}
  { }

  friend auto operator==(const sym_rel& lhs, const sym_rel& rhs) noexcept
    -> bool
  {
    return lhs.m_value == rhs.m_value;
  }

  friend auto operator<(const sym_rel& lhs, const sym_rel& rhs) noexcept
    -> bool
  {
    return lhs.m_value < rhs.m_value;
  }
};

class ct_sym_rel : public supl::sym_rel_ops<ct_sym_rel>
{
private:

  int m_value;

public:

  // NOLINTNEXTLINE(*explicit*)
  constexpr ct_sym_rel(int value)
      : m_value {value}
  { }

  friend constexpr auto operator==(const ct_sym_rel& lhs,
                                   const ct_sym_rel& rhs) noexcept -> bool
  {
    return lhs.m_value == rhs.m_value;
  }

  friend constexpr auto operator<(const ct_sym_rel& lhs,
                                  const ct_sym_rel& rhs) noexcept -> bool
  {
    return lhs.m_value < rhs.m_value;
  }
};

static auto test_sym_rel_ops() -> supl::test_results
{
  supl::test_results results;

  const sym_rel rel_A {5};
  const sym_rel rel_B {5};
  const sym_rel rel_C {8};

  results.enforce_true(rel_A == rel_B);
  results.enforce_false(rel_A != rel_B);
  results.enforce_false(rel_A < rel_B);
  results.enforce_true(rel_A <= rel_B);
  results.enforce_false(rel_A > rel_B);
  results.enforce_true(rel_A >= rel_B);

  results.enforce_false(rel_C == rel_B);
  results.enforce_true(rel_C != rel_B);
  results.enforce_false(rel_C < rel_B);
  results.enforce_false(rel_C <= rel_B);
  results.enforce_true(rel_C > rel_B);
  results.enforce_true(rel_C >= rel_B);

  // and at compile-time!

  constexpr static ct_sym_rel ct_rel_A {5};
  constexpr static ct_sym_rel ct_rel_B {5};
  constexpr static ct_sym_rel ct_rel_C {8};

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

  results.enforce_true(A_eq_B);
  results.enforce_false(A_neq_B);
  results.enforce_false(A_lt_B);
  results.enforce_true(A_leq_B);
  results.enforce_false(A_gt_B);
  results.enforce_true(A_geq_B);
  results.enforce_false(C_eq_B);
  results.enforce_true(C_neq_B);
  results.enforce_false(C_lt_B);
  results.enforce_false(C_leq_B);
  results.enforce_true(C_gt_B);
  results.enforce_true(C_geq_B);

  // symmetric

  constexpr static bool sym_5_eq_B {5 == ct_rel_B};
  constexpr static bool sym_5_neq_B {5 != ct_rel_B};
  constexpr static bool sym_5_lt_B {5 < ct_rel_B};
  constexpr static bool sym_5_leq_B {5 <= ct_rel_B};
  constexpr static bool sym_5_gt_B {5 > ct_rel_B};
  constexpr static bool sym_5_geq_B {5 >= ct_rel_B};
  constexpr static bool sym_8_eq_B {8 == ct_rel_B};
  constexpr static bool sym_8_neq_B {8 != ct_rel_B};
  constexpr static bool sym_8_lt_B {8 < ct_rel_B};
  constexpr static bool sym_8_leq_B {8 <= ct_rel_B};
  constexpr static bool sym_8_gt_B {8 > ct_rel_B};
  constexpr static bool sym_8_geq_B {8 >= ct_rel_B};

  results.enforce_true(sym_5_eq_B);
  results.enforce_false(sym_5_neq_B);
  results.enforce_false(sym_5_lt_B);
  results.enforce_true(sym_5_leq_B);
  results.enforce_false(sym_5_gt_B);
  results.enforce_true(sym_5_geq_B);
  results.enforce_false(sym_8_eq_B);
  results.enforce_true(sym_8_neq_B);
  results.enforce_false(sym_8_lt_B);
  results.enforce_false(sym_8_leq_B);
  results.enforce_true(sym_8_gt_B);
  results.enforce_true(sym_8_geq_B);

  return results;
}

static auto test_addition() -> supl::test_results
{
  supl::test_results results;

  class addable : public supl::addition<addable>
  {
  private:

    int m_value;

  public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr addable(int value)
        : m_value {value}
    { }

    constexpr auto operator+=(const addable& rhs) noexcept -> addable&
    {
      m_value += rhs.m_value;
      return *this;
    }

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }
  };

  const addable A {2};
  const addable B {A + 5};
  const addable C {5 + A};
  results.enforce_equal(B.value(), 7);
  results.enforce_equal(C.value(), 7);

  return results;
}

static auto test_subtraction() -> supl::test_results
{
  supl::test_results results;

  class subtractable : public supl::subtraction<subtractable>
  {
  private:

    int m_value;

  public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr subtractable(int value)
        : m_value {value}
    { }

    constexpr auto operator-=(const subtractable& rhs) noexcept
      -> subtractable&
    {
      m_value -= rhs.m_value;
      return *this;
    }

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }
  };

  const subtractable A {2};
  const subtractable B {A - 5};
  const subtractable C {5 - A};
  results.enforce_equal(B.value(), -3);
  results.enforce_equal(C.value(), 3);

  return results;
}

static auto test_multiplication() -> supl::test_results
{
  supl::test_results results;

  class multiplicable : public supl::multiplication<multiplicable>
  {
  private:

    int m_value;

  public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr multiplicable(int value)
        : m_value {value}
    { }

    constexpr auto operator*=(const multiplicable& rhs) noexcept
      -> multiplicable&
    {
      m_value *= rhs.m_value;
      return *this;
    }

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }
  };

  const multiplicable A {2};
  const multiplicable B {A * 5};
  const multiplicable C {5 * A};
  results.enforce_equal(B.value(), 10);
  results.enforce_equal(C.value(), 10);

  return results;
}

static auto test_division() -> supl::test_results
{
  supl::test_results results;

  class divisible : public supl::division<divisible>
  {
  private:

    int m_value;

  public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr divisible(int value)
        : m_value {value}
    { }

    constexpr auto operator/=(const divisible& rhs) noexcept -> divisible&
    {
      m_value /= rhs.m_value;
      return *this;
    }

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }
  };

  const divisible A {4};
  const divisible B {A / 2};
  const divisible C {12 / A};
  results.enforce_equal(B.value(), 2);
  results.enforce_equal(C.value(), 3);

  return results;
}

static auto test_arithmetic() -> supl::test_results
{
  supl::test_results results;

  class mathematical : public supl::arithmetic<mathematical>
  {
  private:

    int m_value;

  public:

    // NOLINTNEXTLINE(*explicit*)
    constexpr mathematical(int value)
        : m_value {value}
    { }

    constexpr auto operator+=(const mathematical& rhs) noexcept
      -> mathematical&
    {
      m_value += rhs.m_value;
      return *this;
    }

    constexpr auto operator-=(const mathematical& rhs) noexcept
      -> mathematical&
    {
      m_value -= rhs.m_value;
      return *this;
    }

    constexpr auto operator*=(const mathematical& rhs) noexcept
      -> mathematical&
    {
      m_value *= rhs.m_value;
      return *this;
    }

    constexpr auto operator/=(const mathematical& rhs) noexcept
      -> mathematical&
    {
      m_value /= rhs.m_value;
      return *this;
    }

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }
  };

  const mathematical A {4};

  const mathematical add_l {A + 2};
  const mathematical add_r {2 + A};
  results.enforce_equal(add_l.value(), 6);
  results.enforce_equal(add_r.value(), 6);

  const mathematical sub_l {A - 2};
  const mathematical sub_r {2 - A};
  results.enforce_equal(sub_l.value(), 2);
  results.enforce_equal(sub_r.value(), -2);

  const mathematical mult_l {A * 2};
  const mathematical mult_r {2 * A};
  results.enforce_equal(mult_l.value(), 8);
  results.enforce_equal(mult_r.value(), 8);

  const mathematical div_l {A / 2};
  const mathematical div_r {16 / A};
  results.enforce_equal(div_l.value(), 2);
  results.enforce_equal(div_r.value(), 4);

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
  section.add_test("supl::sym_rel_ops", &test_sym_rel_ops);
  section.add_test("supl::addition", &test_addition);
  section.add_test("supl::subtraction", &test_subtraction);
  section.add_test("supl::multiplication", &test_multiplication);
  section.add_test("supl::division", &test_division);
  section.add_test("supl::arithmetic", &test_arithmetic);
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
