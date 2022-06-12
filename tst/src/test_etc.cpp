#include "test_etc.h"

#include "test_utils.h"

bool test_all_pass()
{
	std::vector<int> yes{1, 2, 3, 4, 2, 3};
	std::array no{1, 2, 4, 9, 2, 3};
	auto checker = [](const int i) -> bool{ return i < 6; };

	return	(
				ehanc::all_pass(yes, checker)
			&&	! ehanc::all_pass(no, checker)
			);
}


void test_etc()
{
	run_test("ehanc::all_pass", &test_all_pass);
}
