#include "test_lazy.h"

#include <iostream>

ehanc::test test_lazy_class()
{
	ehanc::test result;

	std::vector<int> check;

	ehanc::lazy<int> test([&check, i{5}]() mutable -> int {
				check.push_back(2);
				return i++;
			});

	check.push_back(1);

	result.add_case(test.get(), 5);
	result.add_case(check[0], 1);
	result.add_case(check[1], 2);
	result.add_case(test.func()(), 6);
	result.add_case(test.get(), 5);

	return result;
}

ehanc::test test_lazy_has_value()
{
	ehanc::test result;

	auto test{ehanc::make_lazy([](){return 5;})};

	bool before = test.has_value();

	[[maybe_unused]] auto discarded{test.get()};

	bool after = test.has_value();

	result.add_case(before, false);
	result.add_case(after, true);

	return result;
}

ehanc::test test_make_lazy()
{
	ehanc::test result;

	auto test{ehanc::make_lazy([](){return 5;})};

	result.add_case(test.get(), 5);
	result.add_case(test.get(), 5);

	return result;
}

ehanc::test test_is_lazy()
{
	ehanc::test result;

	result.add_case(ehanc::is_lazy_v<ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_v<const ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_v<volatile ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_v<const volatile ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_v<ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_v<const ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_v<volatile ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_v<const volatile ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_v<ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_v<const ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_v<volatile ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_v<const volatile ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_v<int>, false);

	return result;
}

ehanc::test test_is_lazy_of()
{
	ehanc::test result;

	result.add_case(ehanc::is_lazy_of_v<int,
			ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			const ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			volatile ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			const volatile ehanc::lazy<int>>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			const ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			volatile ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			const volatile ehanc::lazy<int>&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			const ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			volatile ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_of_v<int,
			const volatile ehanc::lazy<int>&&>, true);
	result.add_case(ehanc::is_lazy_of_v<char,
			ehanc::lazy<int>>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			const ehanc::lazy<int>>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			volatile ehanc::lazy<int>>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			const volatile ehanc::lazy<int>>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			const ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			volatile ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			const volatile ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			const ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			volatile ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<char,
			const volatile ehanc::lazy<int>&&>, false);
	result.add_case(ehanc::is_lazy_of_v<int,
			int>, false);
	result.add_case(ehanc::is_lazy_of_v<int,
			char>, false);

	return result;
}

ehanc::test test_lazy_inner_type()
{
	ehanc::test result;

	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>, true);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&&>>, true);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<const ehanc::lazy<int>>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<const ehanc::lazy<int>&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<ehanc::lazy<int>&&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<const ehanc::lazy<int>&&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<volatile ehanc::lazy<int>&&>>, false);
	result.add_case(std::is_same_v<char,
			ehanc::lazy_inner_type_t<const volatile ehanc::lazy<int>&&>>, false);
	result.add_case(std::is_same_v<int,
			ehanc::lazy_inner_type_t<int>>, true);

	return result;
}

void test_lazy()
{
	ehanc::run_test("ehanc::lazy", &test_lazy_class);
	ehanc::run_test("ehanc::lazy::has_value", &test_lazy_has_value);
	ehanc::run_test("ehanc::make_lazy", &test_make_lazy);
	ehanc::run_test("ehanc::is_lazy", &test_is_lazy);
	ehanc::run_test("ehanc::is_lazy_of", &test_is_lazy_of);
	ehanc::run_test("ehanc::lazy_inner_type", &test_lazy_inner_type);
}
