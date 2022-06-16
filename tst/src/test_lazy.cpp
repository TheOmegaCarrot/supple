#include "test_lazy.h"

#include "test_utils.h"

#include <iostream>

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

bool test_is_lazy()
{
	ehanc::lazy<int> test{[](){return 5;}};
	bool t1{ehanc::is_lazy_v<decltype(test)>};
	bool f1{ehanc::is_lazy_v<int>};
	bool t2{ehanc::is_lazy<decltype(test)>::value};
	bool f2{ehanc::is_lazy<int>::value};

	return	(
				t1
			&&	t2
			&&	!f1
			&&	!f2
			);
}

bool test_is_lazy_of()
{
	ehanc::lazy<int> test{[](){return 5;}};

	return	(
				ehanc::is_lazy_of<int, decltype(test)>::value
			&&	!ehanc::is_lazy_of<double, decltype(test)>::value
			&&	!ehanc::is_lazy_of<double, int>::value
			&&	ehanc::is_lazy_of_v<int, decltype(test)>
			&&	!ehanc::is_lazy_of_v<double, decltype(test)>
			&&	!ehanc::is_lazy_of_v<double, int>
		   );
}

bool test_lazy_inner_type()
{
	ehanc::lazy<int> test{[](){return 5;}};

	return	(
				std::is_same_v<
				typename ehanc::lazy_inner_type<decltype(test)>::type, int>
			&&	not std::is_same_v<
				typename ehanc::lazy_inner_type<decltype(test)>::type, double>
			&&	std::is_same_v<
				typename ehanc::lazy_inner_type<int>::type, int>
			&&	std::is_same_v<
				ehanc::lazy_inner_type_t<decltype(test)>, int>
			&&	not std::is_same_v<
				ehanc::lazy_inner_type_t<decltype(test)>, double>
			&&	std::is_same_v<
				ehanc::lazy_inner_type_t<int>, int>
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
	run_test("ehanc::is_lazy", &test_is_lazy);
	run_test("ehanc::is_lazy_of", &test_is_lazy_of);
	run_test("ehanc::lazy_inner_type", &test_lazy_inner_type);
	run_test("ehanc::make_lazy", &test_make_lazy);
}
