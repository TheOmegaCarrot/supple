#include <array>
#include <vector>
#include <numeric>

#include "test_algorithm.h"

ehanc::test test_for_each_pair()
{
	ehanc::test result;
	std::vector<int> test_input(6);
	std::iota(test_input.begin(), test_input.end(), 1);
	std::vector<int> test_output;
	std::vector<int> reference_output{3, 5, 7, 9, 11};
	
	ehanc::for_each_pair(test_input.cbegin(), test_input.cend(),
			[&test_output](const int i, const int j) {
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref == reference_output.cend(), true);

	return result;
}

ehanc::test test_for_each_pair_n()
{
	ehanc::test result;
	std::vector<int> test_input{1, 2, 3, 4, 5, 6};
	std::vector<int> test_output;
	std::vector<int> reference_output{3, 5, 7};
	
	ehanc::for_each_pair_n(test_input.cbegin(), test_input.cend(), 3,
			[&test_output](const int i, const int j) {
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref  == reference_output.cend(), true);

	return result;
}

ehanc::test test_for_each_all()
{
	ehanc::test result;
	std::array test1{4, 9, 16, 25};
	std::array test2{2, 3, 4, 5};
	std::vector<int> test_output;
	std::vector reference_output{2, 3, 4, 5};

	ehanc::for_each_all([&test_output](const int a, const int b) {
				test_output.push_back(a / b);
			}, test1, test2);

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref  == reference_output.cend(), true);

	return result;
}

ehanc::test test_for_each_all_c()
{
	ehanc::test result;
	const std::array test1{4, 9, 16, 25};
	const std::array test2{2, 3, 4, 5};
	std::vector<int> test_output;
	std::vector reference_output{2, 3, 4, 5};

	ehanc::for_each_all_c([&test_output](const int a, const int b) {
				test_output.push_back(a / b);
			}, test1, test2);

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref  == reference_output.cend(), true);

	return result;
}

ehanc::test test_for_each_all_n()
{
	ehanc::test result;
	std::array test1{4, 9, 16, 25};
	std::array test2{2, 3, 4, 5};
	std::vector<int> test_output;
	std::vector reference_output{2, 3, 4};

	ehanc::for_each_all_n([&test_output](const int a, const int b) {
				test_output.push_back(a / b);
			}, 3, test1.cbegin(), test2.cbegin());

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref  == reference_output.cend(), true);

	return result;
}

ehanc::test test_for_each_both()
{
	ehanc::test result;
	std::array<int, 5> test_input_1;
	std::iota(test_input_1.begin(), test_input_1.end(), 1);
	std::array<int, 4> test_input_2{10, 20, 30, 40};
	std::vector<int> test_output;
	std::array<int, 4> reference_output{11, 22, 33, 44};

	ehanc::for_each_both(test_input_1.cbegin(), test_input_1.cend(),
			test_input_2.cbegin(), test_input_2.cend(),
			[&test_output](const auto& i, const auto& j){
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref  == reference_output.cend(), true);

	return result;
}

ehanc::test test_for_each_both_n()
{
	ehanc::test result;
	std::array<int, 5> test_input_1;
	std::iota(test_input_1.begin(), test_input_1.end(), 1);
	std::array<int, 4> test_input_2{10, 20, 30, 40};
	std::vector<int> test_output;
	std::array<int, 2> reference_output{11, 22};

	ehanc::for_each_both_n(test_input_1.cbegin(), test_input_1.cend(),
			test_input_2.cbegin(), test_input_2.cend(), 2,
			[&test_output](const auto& i, const auto& j){
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	result.add_case(test == test_output.cend(), true);
	result.add_case(ref  == reference_output.cend(), true);

	return result;
}

ehanc::test test_forward_distance()
{
	ehanc::test result;
	std::array<int, 5> test{1, 2, 3, 4, 5};

	result.add_case(ehanc::forward_distance(test.cbegin(), test.cend()),
			size_t{5});

	return result;
}

ehanc::test test_last()
{
	ehanc::test result;
	std::array<int, 5> test1;
	std::iota(test1.begin(), test1.end(), 1);

	std::array<int, 2> test2{2, 3};
	std::vector<int> test3;

	result.add_case(*ehanc::last(test1), 5);
	result.add_case(*ehanc::last(test2), 3);
	result.add_case(ehanc::last(test3) == std::begin(test3), true);

	return result;
}

ehanc::test test_clast()
{
	ehanc::test result;
	std::array<int, 5> test1;
	std::iota(test1.begin(), test1.end(), 1);

	std::array<int, 2> test2{2, 3};
	std::vector<int> test3;

	result.add_case(*ehanc::clast(test1), 5);
	result.add_case(*ehanc::clast(test2), 3);
	result.add_case(ehanc::clast(test3) == std::cbegin(test3), true);

	return result;
}

void test_algorithm()
{
	ehanc::run_test("ehanc::for_each_pair", &test_for_each_pair);
	ehanc::run_test("ehanc::for_each_pair_n", &test_for_each_pair_n);
	ehanc::run_test("ehanc::for_each_all_n", &test_for_each_all_n);
	ehanc::run_test("ehanc::for_each_all", &test_for_each_all);
	ehanc::run_test("ehanc::for_each_all_c", &test_for_each_all_c);
	ehanc::run_test("ehanc::for_each_both", &test_for_each_both);
	ehanc::run_test("ehanc::for_each_both_n", &test_for_each_both_n);
	ehanc::run_test("ehanc::forward_distance", &test_forward_distance);
	ehanc::run_test("ehanc::last", & test_last);
	ehanc::run_test("ehanc::clast", & test_clast);
}
