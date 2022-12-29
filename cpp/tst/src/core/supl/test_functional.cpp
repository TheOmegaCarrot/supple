#include <functional>
#include <memory>

#include "supl/functional.hpp"
#include "supl/test_functional.h"
#include "test_utils.hpp"

static auto test_equal_to() -> ehanc::test
{
  ehanc::test results;

  auto equal_to_5 {supl::equal_to(5)};

  results.add_case(equal_to_5(3), false, "(3 == 5) == false");
  results.add_case(equal_to_5(7), false, "(7 == 5) == false");
  results.add_case(equal_to_5(5), true, "(5 == 5) == true");

  return results;
}

static auto test_not_equal_to() -> ehanc::test
{
  ehanc::test results;

  auto not_equal_to_5 {supl::not_equal_to(5)};

  results.add_case(not_equal_to_5(3), true, "(3 != 5) == true");
  results.add_case(not_equal_to_5(7), true, "(7 != 5) == true");
  results.add_case(not_equal_to_5(5), false, "(5 != 5) == false");

  return results;
}

static auto test_greater_than() -> ehanc::test
{
  ehanc::test results;

  auto greater_than_5 {supl::greater_than(5)};

  results.add_case(greater_than_5(3), false, "3 > 5 == false");
  results.add_case(greater_than_5(7), true, "7 > 5 == true");
  results.add_case(greater_than_5(5), false, "5 > 5 == false");

  return results;
}

static auto test_greater_eq_than() -> ehanc::test
{
  ehanc::test results;

  auto greater_eq_than_5 {supl::greater_eq_than(5)};

  results.add_case(greater_eq_than_5(3), false, "3 >= 5 == false");
  results.add_case(greater_eq_than_5(7), true, "7 >= 5 == true");
  results.add_case(greater_eq_than_5(5), true, "5 >= 5 == true");

  return results;
}

static auto test_less_than() -> ehanc::test
{
  ehanc::test results;

  auto less_than_5 {supl::less_than(5)};

  results.add_case(less_than_5(3), true, "3 < 5 == true");
  results.add_case(less_than_5(7), false, "7 < 5 == false");
  results.add_case(less_than_5(5), false, "5 < 5 == false");

  return results;
}

static auto test_less_eq_than() -> ehanc::test
{
  ehanc::test results;

  auto less_eq_than_5 {supl::less_eq_than(5)};

  results.add_case(less_eq_than_5(3), true, "3 <= 5 == true");
  results.add_case(less_eq_than_5(7), false, "7 <= 5 == false");
  results.add_case(less_eq_than_5(5), true, "5 <= 5 == true");

  return results;
}

static auto test_invoke() -> ehanc::test
{
  ehanc::test results;

  // Regular functions

  constexpr static int result1 {supl::invoke([]() { return 5; })};
  results.add_case(result1, 5);

  results.add_case(result1, 5);

  constexpr static int result2 {supl::invoke([](int x) { return x; }, 5)};

  results.add_case(result2, 5);

  // Pointer to member

  struct Foo {
    int m_value;

    [[nodiscard]] constexpr auto value() const noexcept -> int
    {
      return m_value;
    }

    [[nodiscard]] constexpr auto value_plus(int arg) const noexcept -> int
    {
      return m_value + arg;
    }
  };

  constexpr static Foo foo {5};

  constexpr static int result3 {supl::invoke(&Foo::value, &foo)};

  results.add_case(result3, 5);

  constexpr static int result4 {supl::invoke(&Foo::value_plus, &foo, 2)};

  results.add_case(result4, 7);

  constexpr static int result5 {supl::invoke(&Foo::value, foo)};

  results.add_case(result5, 5);

  constexpr static int result6 {supl::invoke(&Foo::value_plus, foo, 2)};

  results.add_case(result6, 7);

  constexpr static int result12 {supl::invoke(&Foo::m_value, foo)};

  results.add_case(result12, 5);

  constexpr static int result13 {supl::invoke(&Foo::m_value, &foo)};

  results.add_case(result13, 5);

  // ref-qualification

  struct ref_qual {
    int m_value;

    [[nodiscard]] constexpr auto funcl() const& noexcept -> int
    {
      return m_value;
    }

    [[nodiscard]] constexpr auto funcr() const&& noexcept -> int
    {
      return m_value + 5;
    }

    [[nodiscard]] constexpr auto funcl2(int arg) const& noexcept -> int
    {
      return m_value + arg;
    }

    [[nodiscard]] constexpr auto funcr2(int arg) const&& noexcept -> int
    {
      return m_value + arg + 5;
    }
  };

  constexpr static ref_qual qual {5};

  constexpr static int result7 {supl::invoke(&ref_qual::funcl, qual)};

  results.add_case(result7, 5);

  constexpr static int result8 {
      supl::invoke(&ref_qual::funcr, ref_qual {5})};

  results.add_case(result8, 10);

  constexpr static int result9 {supl::invoke(&ref_qual::funcl2, qual, 3)};

  results.add_case(result9, 8);

  constexpr static int result10 {
      supl::invoke(&ref_qual::funcr2, ref_qual {5}, 3)};

  results.add_case(result10, 13);

  // virtual calls

  // NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
  class virt_base
  {
  protected:

    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
    int m_value;

  public:

    virt_base() = delete;

    explicit virt_base(int value)
        : m_value {value}
    {}

    virtual ~virt_base() = default;

    virtual auto func(int arg) -> int
    {
      return m_value + arg;
    }
  };

  // NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
  class virt_derv : public virt_base
  {
  public:

    virt_derv() = delete;

    explicit virt_derv(int value)
        : virt_base {value}
    {}

    ~virt_derv() override = default;

    auto func(int arg) -> int override
    {
      return m_value * arg;
    }
  };

  std::unique_ptr<virt_base> vptr {std::make_unique<virt_derv>(5)};

  const int result11 {supl::invoke(&virt_base::func, vptr.get(), 3)};

  results.add_case(result11, 15);

  return results;
}

void test_functional()
{
  ehanc::run_test("supl::equal_to", &test_equal_to);
  ehanc::run_test("supl::not_equal_to", &test_not_equal_to);
  ehanc::run_test("supl::greater_than", &test_greater_than);
  ehanc::run_test("supl::greater_eq_than", &test_greater_eq_than);
  ehanc::run_test("supl::less_than", &test_less_than);
  ehanc::run_test("supl::less_eq_than", &test_less_eq_than);
  ehanc::run_test("supl::invoke", &test_invoke);
}
