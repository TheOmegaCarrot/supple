#include "test_etc.h"

#include "test_utils.h"

bool test_all_pass_cont()
{
	std::vector<int> yes{1, 2, 3, 4, 2, 3};
	std::array no{1, 2, 4, 9, 2, 3};
	auto checker = [](const int i) -> bool{ return i < 6; };

	return	(
				ehanc::all_pass(yes, checker)
			&&	! ehanc::all_pass(no, checker)
			);
}

bool test_all_pass_itr()
{
	std::vector<int> yes{1, 2, 3, 4, 2, 3};
	std::array no{1, 2, 4, 9, 2, 3};
	auto checker = [](const int i) -> bool{ return i < 6; };

	return	(
				ehanc::all_pass(yes.cbegin(), yes.cend(), checker)
			&&	! ehanc::all_pass(no.cbegin(), no.cend(), checker)
			);
}

bool test_min_size()
{
	std::array<int,5> test1;
	std::array<int,8> test2;
	std::array<int,3> test3;
	std::array<int,6> test4;

	return ehanc::min_size(test1, test2, test3, test4) == 3;

}

bool test_size_t_literals()
{
	using namespace ehanc::literals;
	std::size_t i{500};
	auto j{500_z};
	return (
				std::is_same_v<decltype(j), std::size_t>
			&&	i == j
		   );
}

void test_etc()
{
	run_test("ehanc::all_pass (container)", &test_all_pass_cont);
	run_test("ehanc::all_pass (iterators)", &test_all_pass_itr);
	run_test("ehanc::min_size", &test_min_size);
	run_test("ehanc::operator\"\"_z", &test_size_t_literals);
}
