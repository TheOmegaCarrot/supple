#ifndef SUPPLE_INTERNAL_FOO_PARAMS_H
#define SUPPLE_INTERNAL_FOO_PARAMS_H

#include <cstddef>

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

#endif
