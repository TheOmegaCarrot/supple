#include <supl/named_params.hpp>

#include "foo_params.h"

auto main() -> int
{
  // this should not compile, because `foo_type` is specified twice
  [[maybe_unused]] supl::
    named_params<foo_count, foo_type, foo_size, foo_type>
      params {};
}
