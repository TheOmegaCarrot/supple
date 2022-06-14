#include "test_lazy.h"

#include "test_utils.h"

bool test_lazy_class()
{

	std::vector<int> check;

	ehanc::lazy<int> test([&check, i{5}]() mutable -> int {
				check.push_back(2);
				return i++;
			});

	check.push_back(1);

	return	(
				test == 5
			&&	check[0] == 1
			&&	check[1] == 2
			&&	test.func()() == 6
			&&	test == 5
			);
}

bool test_lazy_has_value()
{
	auto test{ehanc::make_lazy([](){return 5;})};

	bool before = test.has_value();

	[[maybe_unused]] auto discarded{test.get()};

	bool after = test.has_value();

	return	(
				!before
			&&	after
			);

}

bool test_make_lazy()
{
	auto test{ehanc::make_lazy([](){return 5;})};

	return	(
				test == 5
			&&	test == 5
			);
}

void test_lazy()
{
	run_test("ehanc::lazy", &test_lazy_class);
	run_test("ehanc::lazy::has_value", &test_lazy_has_value);
	run_test("ehanc::make_lazy", &test_make_lazy);
}
