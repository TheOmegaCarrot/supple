#include "supl/tuple_algo.hpp"

using supl::tuple::impl::tuple_element_variant_t;

static_assert(
  std::is_same_v<std::variant<int, char, bool>,
                 tuple_element_variant_t<std::tuple<int, char, bool>>>);

static_assert(std::is_same_v<
              std::variant<int, char, bool>,
              tuple_element_variant_t<
                std::tuple<int, char, char, char, int, bool, int, bool>>>);

static_assert(
  std::is_same_v<std::variant<int, char, std::reference_wrapper<bool>>,
                 tuple_element_variant_t<std::tuple<int, char, bool&>>>);

static_assert(
  std::is_same_v<std::variant<int, char, std::reference_wrapper<bool>>,
                 tuple_element_variant_t<std::tuple<int, char, bool&&>>>);

static_assert(
  std::is_same_v<
    std::variant<const int, char, std::reference_wrapper<const bool>>,
    tuple_element_variant_t<std::tuple<const int, char, const bool&>>>);

int main()
{ }
