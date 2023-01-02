#include <functional>
#include <memory>

#include <supl/functional.hpp>

#include "supl/test_functional.h"

static auto test_equal_to() -> supl::test_results
{
  supl::test_results results;

  auto equal_to_5 {supl::equal_to(5)};

  results.enforce_exactly_equal(equal_to_5(3), false, "(3 == 5) == false");
  results.enforce_exactly_equal(equal_to_5(7), false, "(7 == 5) == false");
  results.enforce_exactly_equal(equal_to_5(5), true, "(5 == 5) == true");

  return results;
}

static auto test_not_equal_to() -> supl::test_results
{
  supl::test_results results;

  auto not_equal_to_5 {supl::not_equal_to(5)};

  results.enforce_exactly_equal(
    not_equal_to_5(3), true, "(3 != 5) == true"
  );
  results.enforce_exactly_equal(
    not_equal_to_5(7), true, "(7 != 5) == true"
  );
  results.enforce_exactly_equal(
    not_equal_to_5(5), false, "(5 != 5) == false"
  );

  return results;
}

static auto test_greater_than() -> supl::test_results
{
  supl::test_results results;

  auto greater_than_5 {supl::greater_than(5)};

  results.enforce_exactly_equal(
    greater_than_5(3), false, "3 > 5 == false"
  );
  results.enforce_exactly_equal(greater_than_5(7), true, "7 > 5 == true");
  results.enforce_exactly_equal(
    greater_than_5(5), false, "5 > 5 == false"
  );

  return results;
}

static auto test_greater_eq_than() -> supl::test_results
{
  supl::test_results results;

  auto greater_eq_than_5 {supl::greater_eq_than(5)};

  results.enforce_exactly_equal(
    greater_eq_than_5(3), false, "3 >= 5 == false"
  );
  results.enforce_exactly_equal(
    greater_eq_than_5(7), true, "7 >= 5 == true"
  );
  results.enforce_exactly_equal(
    greater_eq_than_5(5), true, "5 >= 5 == true"
  );

  return results;
}

static auto test_less_than() -> supl::test_results
{
  supl::test_results results;

  auto less_than_5 {supl::less_than(5)};

  results.enforce_exactly_equal(less_than_5(3), true, "3 < 5 == true");
  results.enforce_exactly_equal(less_than_5(7), false, "7 < 5 == false");
  results.enforce_exactly_equal(less_than_5(5), false, "5 < 5 == false");

  return results;
}

static auto test_less_eq_than() -> supl::test_results
{
  supl::test_results results;

  auto less_eq_than_5 {supl::less_eq_than(5)};

  results.enforce_exactly_equal(less_eq_than_5(3), true, "3 <= 5 == true");
  results.enforce_exactly_equal(
    less_eq_than_5(7), false, "7 <= 5 == false"
  );
  results.enforce_exactly_equal(less_eq_than_5(5), true, "5 <= 5 == true");

  return results;
}

static auto test_invoke() -> supl::test_results
{
  supl::test_results results;

  // Regular functions

  constexpr static int result1 {supl::invoke([]() { return 5; })};
  results.enforce_exactly_equal(result1, 5);

  results.enforce_exactly_equal(result1, 5);

  constexpr static int result2 {supl::invoke([](int x) { return x; }, 5)};

  results.enforce_exactly_equal(result2, 5);

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

  results.enforce_exactly_equal(result3, 5);

  constexpr static int result4 {supl::invoke(&Foo::value_plus, &foo, 2)};

  results.enforce_exactly_equal(result4, 7);

  constexpr static int result5 {supl::invoke(&Foo::value, foo)};

  results.enforce_exactly_equal(result5, 5);

  constexpr static int result6 {supl::invoke(&Foo::value_plus, foo, 2)};

  results.enforce_exactly_equal(result6, 7);

  constexpr static int result12 {supl::invoke(&Foo::m_value, foo)};

  results.enforce_exactly_equal(result12, 5);

  constexpr static int result13 {supl::invoke(&Foo::m_value, &foo)};

  results.enforce_exactly_equal(result13, 5);

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

  results.enforce_exactly_equal(result7, 5);

  constexpr static int result8 {
    supl::invoke(&ref_qual::funcr, ref_qual {5})};

  results.enforce_exactly_equal(result8, 10);

  constexpr static int result9 {supl::invoke(&ref_qual::funcl2, qual, 3)};

  results.enforce_exactly_equal(result9, 8);

  constexpr static int result10 {
    supl::invoke(&ref_qual::funcr2, ref_qual {5}, 3)};

  results.enforce_exactly_equal(result10, 13);

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
    { }

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
    { }

    ~virt_derv() override = default;

    auto func(int arg) -> int override
    {
      return m_value * arg;
    }
  };

  const std::unique_ptr<virt_base> vptr {std::make_unique<virt_derv>(5)};

  const int result11 {supl::invoke(&virt_base::func, vptr.get(), 3)};

  results.enforce_exactly_equal(result11, 15);

  return results;
}

auto test_functional() -> supl::test_section
{
  supl::test_section section;

  section.add_test("supl::equal_to", &test_equal_to);
  section.add_test("supl::not_equal_to", &test_not_equal_to);
  section.add_test("supl::greater_than", &test_greater_than);
  section.add_test("supl::greater_eq_than", &test_greater_eq_than);
  section.add_test("supl::less_than", &test_less_than);
  section.add_test("supl::less_eq_than", &test_less_eq_than);
  section.add_test("supl::invoke", &test_invoke);

  return section;
}
