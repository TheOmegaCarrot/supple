#include <array>
#include <vector>
#include <numeric>

#include "test_algorithm.h"

bool test_for_each_pair()
{
	std::vector<int> test_input(6);
	std::vector<int> test_output;
	std::vector<int> reference_output = {3, 5, 7, 9, 11};
	std::iota(test_input.begin(), test_input.end(), 1);
	
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
