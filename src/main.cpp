#include <iostream>
#include <utility>

#include "utils/all.h"

/// @cond

// NOLINTBEGIN

void lazy_demo();

template <typename... Ts>
auto peeler(Ts...)
{
  return ehanc::peel_first_t<Ts...>();
}

template <typename... Ts>
auto uniform(Ts...) -> bool
{
  return ehanc::is_pack_uniform_v<Ts...>;
}

auto main([[maybe_unused]] const int argc,
          [[maybe_unused]] const char* const* const argv) -> int
{

  /* std::cout << std::boolalpha */
  /*           << std::is_same_v<decltype(peeler()), void> << '\n' */
  /*           << uniform() << '\n' */
  /*           << '\n'; */

  /* lazy_demo(); */

  return 0;
}

struct loud {

  int m_val;

  loud() = delete;

  ~loud() = default;

  explicit loud(int val)
      : m_val {val}
  {}

  loud(const loud& src)
      : m_val {src.m_val}
  {
    std::cout << "COPY" << '\n';
  }

  auto operator=(const loud&) -> loud& = default;

  loud(loud&& src) noexcept
      : m_val {src.m_val}
  {
    std::cout << "MOVE" << '\n';
  }

  auto operator=(loud&&) noexcept -> loud& = default;

  auto operator()() const -> int
  {
    std::cout << "EVAL" << '\n';
    return m_val;
  }
};

template <typename T>
auto printer(const T& print)
{
  std::cout << print << '\n';
  std::cout << print << '\n';
}

void lazy_demo()
{
  ehanc::lazy<int> test([i {loud(7)}]() { return i(); });

  std::cout << test << '\n';
  std::cout << test << '\n';

  ehanc::lazy<int> test2(loud(9));

  std::cout << test2 << '\n';
  std::cout << test2 << '\n';

  auto test3 {ehanc::make_lazy([i {loud(3)}]() { return i(); })};

  std::cout << test3 << '\n';
  std::cout << test3 << '\n';

  auto test4 {ehanc::make_lazy(loud(6))};

  std::cout << test4 << '\n';
  std::cout << test4 << '\n';

  auto test5 {ehanc::make_lazy([i {loud(14)}]() { return i(); })};

  printer(test5);
  printer(test5);

  ehanc::lazy deduced([i {loud(2)}]() { return i(); });

  std::cout << deduced << '\n';
  std::cout << deduced << '\n';
}

// NOLINTEND
