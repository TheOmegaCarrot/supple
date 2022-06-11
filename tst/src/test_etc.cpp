#include "test_etc.h"

#include "test_utils.h"

void test_etc()
{
	run_test("ehanc::lazy", &test_lazy);
}

bool test_lazy()
{

	std::vector<int> check;

	ehanc::lazy<int> test([&check]() -> int {
				check.push_back(2);
				return 5;
			});

	check.push_back(1);

	return	(
				test == 5
			&&	check[0] == 1
			&&	check[1] == 2
			&&	test == 5
			);
}
