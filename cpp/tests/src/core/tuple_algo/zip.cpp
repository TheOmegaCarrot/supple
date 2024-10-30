#include "supl/tuple_algo.hpp"

#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

template <typename... Ls>
struct overload : Ls...
{
    using Ls::operator()...;
};

template <typename... Ls>
overload(Ls...) -> overload<Ls...>;

static auto test_simple_case() -> supl::test_results
{
    supl::test_results results;

    const std::tuple tup_idxs {0, 1, 2};
    const std::tuple tup1 {42, 'g', true};
    const std::tuple tup2 {18L, false, nullptr, "nope"};
    const std::tuple tup3 {'u', std::size_t {81}, 8U};

    const std::tuple expected1 {0, 42, 18L, 'u'};
    const std::tuple expected2 {1, 'g', false, std::size_t {81}};
    const std::tuple expected3 {2, true, nullptr, 8U};

    supl::tuple::zip(
      overload {
        // NOLINTNEXTLINE(*easily-swappable*)
        [&](int idx, int arg1, long arg2, char arg3)
        {
            results.enforce_equal(
              std::tie(idx, arg1, arg2, arg3), expected1
            );
        },
        [&](int idx, char arg1, bool arg2, std::size_t arg3)
        {
            results.enforce_equal(
              std::tie(idx, arg1, arg2, arg3), expected2
            );
        },
        [&](int idx, bool arg1, std::nullptr_t arg2, unsigned arg3)
        {
            results.enforce_equal(
              std::tie(idx, arg1, arg2, arg3), expected3
            );
        },
      },
      tup_idxs, tup1, tup2, tup3
    );

    return results;
}

static auto test_tuple_zip() -> supl::test_section
{
    supl::test_section section;

    section.add_test("supl::tuple::zip simple case", &test_simple_case);

    return section;
}

auto main() -> int
{
    supl::test_runner runner;

    runner.add_section(test_tuple_zip());

    return runner.run();
}
