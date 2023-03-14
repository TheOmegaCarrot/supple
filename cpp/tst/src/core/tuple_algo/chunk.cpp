#include <supl/test_runner.hpp>
#include <supl/tuple_algo.hpp>

auto standard1() -> supl::test_results
{
  supl::test_results results;

  constexpr static std::tuple input {42, 3.14, 'g', 't', 1024UL, 2.7F};

  constexpr static std::tuple<std::tuple<int, double, char>,
                              std::tuple<char, unsigned long, float>>
    expected {
      { 42,   3.14,  'g'},
      {'t', 1024UL, 2.7F}
  };

  constexpr static auto result {supl::tuple::chunk<3>(input)};

  static_assert(result == expected);

  results.enforce_exactly_equal(result, expected);

  return results;
}

auto standard2() -> supl::test_results
{
  supl::test_results results;

  constexpr static std::tuple input {
    42,
    false,
    'r',
    std::pair {12, '&'},
    'E',
    std::tuple {81, short {42}, true},
    18L,
    std::size_t {8196}
  };

  constexpr static std::tuple<
    std::tuple<int, bool>,
    std::tuple<char, std::pair<int, char>>,
    std::tuple<char, std::tuple<int, short, bool>>,
    std::tuple<long, std::size_t>>
    expected {
      { 42,                  false},
      {'r',              {12, '&'}},
      {'E', {81, short {42}, true}},
      {18L,     std::size_t {8196}}
  };

  constexpr static auto result {supl::tuple::chunk<2>(input)};

  static_assert(result == expected);

  results.enforce_exactly_equal(result, expected);

  return results;
}

auto standard3() -> supl::test_results
{
  supl::test_results results;

  enum class foobar {
    foo,
    bar
  };

  // clang-format off
  constexpr static std::tuple input {
    42, true, 'K', 3.1415, 8196UL, '~', false, 18,
    short{12}, '^', 17L, 8.1F, '#', nullptr, unsigned {12}, 9001ULL
  };
  // clang-format on

  constexpr static std::tuple<
    std::tuple<int, bool, char, double, unsigned long, char, bool, int>,
    std::tuple<short,
               char,
               long,
               float,
               char,
               std::nullptr_t,
               unsigned,
               unsigned long long>>
    expected
    // clang-format off
  {
    {42, true, 'K', 3.1415, 8196UL, '~', false, 18},
    {short{12}, '^', 17L, 8.1F, '#', nullptr, unsigned {12} , 9001ULL}
  };
  // clang-format on

  constexpr static auto result {supl::tuple::chunk<8>(input)};

  static_assert(result == expected);

  results.enforce_exactly_equal(result, expected);

  return results;
}

auto chunk_by_1() -> supl::test_results
{
  supl::test_results results;

  constexpr static std::tuple input {31, 72.34, true, 18UL};

  constexpr static std::tuple<std::tuple<int>,
                              std::tuple<double>,
                              std::tuple<bool>,
                              std::tuple<unsigned long>>
    expected {{31}, {72.34}, {true}, {18UL}};

  constexpr static auto result {supl::tuple::chunk<1>(input)};

  static_assert(result == expected);

  results.enforce_exactly_equal(result, expected);

  return results;
}

auto input_size_0() -> supl::test_results
{
  supl::test_results results;

  constexpr static std::tuple<> input {};

  constexpr static std::tuple<> expected {};

  constexpr static auto result {supl::tuple::chunk<5>(input)};

  static_assert(result == expected);

  results.enforce_exactly_equal(result, expected);

  return results;
}

auto chunk() -> supl::test_section
{
  supl::test_section section;

  section.add_test("Standard 1", &standard1);
  section.add_test("Standard 2", &standard2);
  section.add_test("Standard 3", &standard3);
  section.add_test("Chunked by 1", &chunk_by_1);
  section.add_test("Input size 0", &input_size_0);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(chunk());

  return runner.run();
}

