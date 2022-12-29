#include <sstream>
#include <string>

#include "supl/test_crtp.h"
#include "test_utils.hpp"

static auto test_rel_ops() -> ehanc::test
{
  ehanc::test results;

  class rel : public supl::rel_ops<rel>
  {
  private:

    int m_value;

  public:

    explicit rel(int value)
        : m_value {value}
    {}

    auto operator==(const rel& rhs) const noexcept -> bool
    {
      return m_value == rhs.m_value;
    }

    auto operator<(const rel& rhs) const noexcept -> bool
    {
      return m_value < rhs.m_value;
    }
  };

  rel rel_A {5};
  rel rel_B {5};
  rel rel_C {8};

  results.add_case(rel_A == rel_B, true);
  results.add_case(rel_A != rel_B, false);
  results.add_case(rel_A < rel_B, false);
  results.add_case(rel_A <= rel_B, true);
  results.add_case(rel_A > rel_B, false);
  results.add_case(rel_A >= rel_B, true);

  results.add_case(rel_C == rel_B, false);
  results.add_case(rel_C != rel_B, true);
  results.add_case(rel_C < rel_B, false);
  results.add_case(rel_C <= rel_B, false);
  results.add_case(rel_C > rel_B, true);
  results.add_case(rel_C >= rel_B, true);

  // and at compile-time!
  class ct_rel : public supl::rel_ops<ct_rel>
  {
  private:

    int m_value;

  public:

    constexpr explicit ct_rel(int value)
        : m_value {value}
    {}

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

  results.add_case(A_eq_B, true);
  results.add_case(A_neq_B, false);
  results.add_case(A_lt_B, false);
  results.add_case(A_leq_B, true);
  results.add_case(A_gt_B, false);
  results.add_case(A_geq_B, true);
  results.add_case(C_eq_B, false);
  results.add_case(C_neq_B, true);
  results.add_case(C_lt_B, false);
  results.add_case(C_leq_B, false);
  results.add_case(C_gt_B, true);
  results.add_case(C_geq_B, true);

  return results;
}

static auto test_add_to_string() -> ehanc::test
{
  using std::literals::operator""s;

  ehanc::test results;

  struct consumer : supl::add_to_string<consumer> {
    int value {42};

    void to_stream(std::ostream& out) const noexcept
    {
      out << value;
    }
  };

  results.add_case(consumer {}.to_string(), "42"s);

  return results;
}

static auto test_add_ostream() -> ehanc::test
{
  using std::literals::operator""s;

  ehanc::test results;

  struct consumer_to_stream : supl::add_ostream<consumer_to_stream> {
    int value {42};

    void to_stream(std::ostream& out) const noexcept
    {
      out << value;
    }
  };

  consumer_to_stream test1 {};

  std::stringstream str1;
  str1 << test1;
  results.add_case(str1.str(), "42"s);

  struct consumer_iterators : supl::add_ostream<consumer_iterators> {
    std::vector<int> value {1, 2, 42, 18};

    [[nodiscard]] auto begin() const noexcept
    {
      return value.begin();
    }

    [[nodiscard]] auto end() const noexcept
    {
      return value.end();
    }
  };

  consumer_iterators test2 {};

  std::stringstream str2;
  str2 << test2;
  results.add_case(str2.str(), "[ 1, 2, 42, 18 ]"s);

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

  consumer_both test3 {};

  std::stringstream discard;
  test3.to_stream(discard);
  [[maybe_unused]] auto discard_begin {test3.begin()};
  [[maybe_unused]] auto discard_end {test3.end()};

  std::stringstream str3;
  str3 << test3;
  results.add_case(str3.str(), "1"s);

  return results;
}

void test_crtp()
{
  ehanc::run_test("supl::rel_ops", &test_rel_ops);
  ehanc::run_test("supl::add_to_string", &test_add_to_string);
  ehanc::run_test("supl::add_ostream", &test_add_ostream);
}
