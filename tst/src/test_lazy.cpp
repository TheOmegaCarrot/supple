#include "test_lazy.h"

#include "test_utils.h"

bool test_lazy_class()
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

bool test_make_lazy()
{
	auto test = ehanc::make_lazy([](){return 5;});

	return	(
				test == 5
			&&	test == 5
			);
}

void test_lazy()
{
	run_test("ehanc::lazy", &test_lazy_class);
	run_test("ehanc::make_lazy", &test_make_lazy);
}
