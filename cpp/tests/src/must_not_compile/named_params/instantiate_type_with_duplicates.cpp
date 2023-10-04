#include <supl/named_params.hpp>

struct foo_size {
  std::size_t value;
};

enum struct foo_type : char {
  foo,
  bar,
  third
};

struct foo_count {
  std::size_t value;
};

auto main() -> int
{
  // this should not compile, because `foo_type` is specified twice
  [[maybe_unused]] supl::
    named_params<foo_count, foo_type, foo_size, foo_type>
      params {};
}
