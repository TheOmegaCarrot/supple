#include <iostream>
#include <utility>

#include "utils/all.h"

/// @cond

void lazy_demo();

template <typename... Ts>
auto peeler(Ts...)
{
  return ehanc::peel_first_t<Ts...>();
}

template <typename... Ts>
bool uniform(Ts...)
{
  return ehanc::is_pack_uniform_v<Ts...>;
}

template <typename... Ts>
std::size_t sizer(Ts...)
{
  return ehanc::pack_size_v<Ts...>;
}

int main([[maybe_unused]] const int argc,
         [[maybe_unused]] const char* const* const argv)
{

  std::cout << std::boolalpha
            << std::is_same_v<decltype(peeler()), void> << '\n'
            << uniform() << '\n'
            << sizer() << '\n'
            << '\n';

  return 0;
}

struct loud {

  int m_val;

  loud() = delete;

  ~loud() = default;

  loud(int val)
      : m_val{val}
  {}

  loud(const loud& src)
      : m_val{src.m_val}
  {
    std::cout << "COPY" << '\n';
  }

  loud& operator=(const loud&) = default;

  loud(loud&& src) noexcept
      : m_val{src.m_val}
  {
    std::cout << "MOVE" << '\n';
  }

  loud& operator=(loud&&) noexcept = default;

  int operator()() const
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
  ehanc::lazy<int> test([i{loud(7)}]() { return i(); });

  std::cout << test << '\n';
  std::cout << test << '\n';

  ehanc::lazy<int> test2(loud(9));

  std::cout << test2 << '\n';
  std::cout << test2 << '\n';

  auto test3{ehanc::make_lazy([i{loud(3)}]() { return i(); })};

  std::cout << test3 << '\n';
  std::cout << test3 << '\n';

  auto test4{ehanc::make_lazy(loud(6))};

  std::cout << test4 << '\n';
  std::cout << test4 << '\n';

  auto test5{ehanc::make_lazy([i{loud(14)}]() { return i(); })};

  printer(test5);
  printer(test5);

  ehanc::lazy deduced([i{loud(2)}]() { return i(); });

  std::cout << deduced << '\n';
  std::cout << deduced << '\n';
}
