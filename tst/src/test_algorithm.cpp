#include <array>
#include <vector>
#include <numeric>

#include "test_algorithm.h"

#include "test_utils.h"

void test_algorithm()
{
	run_test("ehanc::for_each_pair", &test_for_each_pair);
	run_test("ehanc::for_each_pair_n", &test_for_each_pair_n);
	run_test("ehanc::for_each_both", &test_for_each_both);
	run_test("ehanc::last", & test_last);
	run_test("ehanc::clast", & test_clast);
}

bool test_for_each_pair()
{
	std::vector<int> test_input(6);
	std::iota(test_input.begin(), test_input.end(), 1);
	std::vector<int> test_output;
	std::vector<int> reference_output = {3, 5, 7, 9, 11};
	
	ehanc::for_each_pair(test_input.cbegin(), test_input.cend(),
			[&test_output](const int i, const int j) {
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	return	(
				test == test_output.cend()
			&&	ref  == reference_output.cend()
			);
}

bool test_for_each_pair_n()
{
	std::vector<int> test_input(6);
	std::iota(test_input.begin(), test_input.end(), 1);
	std::vector<int> test_output;
	std::vector<int> reference_output = {3, 5, 7, 9, 11};
	
	ehanc::for_each_pair_n(test_input.cbegin(), test_input.cend(), 3,
			[&test_output](const int i, const int j) {
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	return	(
				test == test_output.cend()
			&&	ref  == reference_output.cend()
			);
}

bool test_for_each_both()
{
	std::array<int, 5> test_input_1;
	std::iota(test_input_1.begin(), test_input_1.end(), 1);
	std::array<int, 4> test_input_2 = {10, 20, 30, 40};
	std::vector<int> test_output;
	std::array<int, 4> reference_output = {11, 22, 33, 44};

	ehanc::for_each_both(test_input_1.cbegin(), test_input_1.cend(),
			test_input_2.cbegin(), test_input_2.cend(),
			[&test_output](const auto& i, const auto& j){
				test_output.push_back(i + j);
			});

	auto[test, ref] = std::mismatch(
			test_output.cbegin(), test_output.cend(),
			reference_output.cbegin(), reference_output.cend() );

	return	(
				test == test_output.cend()
			&&	ref  == reference_output.cend()
			);
}

bool test_last()
{
	std::array<int, 5> test1;
	std::iota(test1.begin(), test1.end(), 1);

	std::array<int, 1> test2 = {3};
	std::vector<int> test3;

	return	(
				*ehanc::last(test1) == 5
			&&	*ehanc::last(test2) == 3
			&&	ehanc::last(test3) == std::begin(test3)
			);
}

bool test_clast()
{
	std::array<int, 5> test1;
	std::iota(test1.begin(), test1.end(), 1);

	std::array<int, 1> test2 = {3};
	std::vector<int> test3;

	return	(
				*ehanc::clast(test1) == 5
			&&	*ehanc::clast(test2) == 3
			&&	ehanc::clast(test3) == std::cbegin(test3)
			);
}
