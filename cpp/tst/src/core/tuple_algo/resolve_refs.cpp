#include <supl/test_results.hpp>

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

auto main() -> int
{
  supl::test_results results;

  const std::tuple test_input {42, 3.14, '&', copy_counter {}};

  const std::tuple intermediate_1 {
    supl::tuple::type_transform<supl::make_const_ref>(test_input)};

  const int i1 {42};
  const std::tuple intermediate_2 {
    supl::tuple::push_back_as<const int&>(intermediate_1, i1)};

  const double d1 {3.14};
  const std::tuple intermediate_3 {
    supl::tuple::push_back_as<const double&>(intermediate_2, d1)};

  const char c1 {'|'};
  const std::tuple intermediate_4 {
    supl::tuple::push_back_as<const char&>(intermediate_3, c1)};

  const bool b1 {false};
  const std::tuple intermediate_5 {
    supl::tuple::push_back_as<const bool&>(intermediate_4, b1)};

  const std::tuple result {supl::tuple::resolve_refs(intermediate_5)};

  const std::
    tuple<int, double, char, copy_counter, int, double, char, bool>
      expected {42, 3.14, '&', copy_counter {1}, 42, 3.14, '|', false};

  results.enforce_exactly_equal(result, expected);

  return results.print_and_return();
}

