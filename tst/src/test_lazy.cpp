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

	return	test == 5
		&&	check[0] == 1
		&&	check[1] == 2
		&&	test.func()() == 6
		&&	test == 5;
}

bool test_lazy_has_value()
{
	auto test{ehanc::make_lazy([](){return 5;})};

	bool before = test.has_value();

	[[maybe_unused]] auto discarded{test.get()};

	bool after = test.has_value();

	return	!before
		&&	after;

}

bool test_is_lazy()
{
	return	ehanc::is_lazy_v<ehanc::lazy<int>>
		&& ehanc::is_lazy_v<const ehanc::lazy<int>>
		&& ehanc::is_lazy_v<volatile ehanc::lazy<int>>
		&& ehanc::is_lazy_v<const volatile ehanc::lazy<int>>
		&& ehanc::is_lazy_v<ehanc::lazy<int>&>
		&& ehanc::is_lazy_v<const ehanc::lazy<int>&>
		&& ehanc::is_lazy_v<volatile ehanc::lazy<int>&>
		&& ehanc::is_lazy_v<const volatile ehanc::lazy<int>&>
		&& ehanc::is_lazy_v<ehanc::lazy<int>&&>
		&& ehanc::is_lazy_v<const ehanc::lazy<int>&&>
		&& ehanc::is_lazy_v<volatile ehanc::lazy<int>&&>
		&& ehanc::is_lazy_v<const volatile ehanc::lazy<int>&&>;
}

bool test_is_lazy_of()
{
	return	ehanc::is_lazy_of_v<int, ehanc::lazy<int>>
		&&	ehanc::is_lazy_of_v<int, const ehanc::lazy<int>>
		&&	ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>>
		&&	ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>>
		&&	ehanc::is_lazy_of_v<int, ehanc::lazy<int>&>
		&&	ehanc::is_lazy_of_v<int, const ehanc::lazy<int>&>
		&&	ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>&>
		&&	ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>&>
		&&	ehanc::is_lazy_of_v<int, ehanc::lazy<int>&&>
		&&	ehanc::is_lazy_of_v<int, const ehanc::lazy<int>&&>
		&&	ehanc::is_lazy_of_v<int, volatile ehanc::lazy<int>&&>
		&&	ehanc::is_lazy_of_v<int, const volatile ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, ehanc::lazy<int>>
		&&	not ehanc::is_lazy_of_v<char, const ehanc::lazy<int>>
		&&	not ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>>
		&&	not ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>>
		&&	not ehanc::is_lazy_of_v<char, ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, const ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, const ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, volatile ehanc::lazy<int>&&>
		&&	not ehanc::is_lazy_of_v<char, const volatile ehanc::lazy<int>&&>;
}

bool test_lazy_inner_type()
{
	return	std::is_same_v<int,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>
		&&	std::is_same_v<int,
		ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<ehanc::lazy<int>>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>
		&&	not std::is_same_v<char,
		ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&&>>
		&&	std::is_same_v<int, ehanc::lazy_inner_type_t<int>>;
}

bool test_make_lazy()
{
	auto test{ehanc::make_lazy([](){return 5;})};

	return	test == 5
		&&	test == 5;
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
