#include "test_utils.h"
#include "test_algorithm.h"

int main( [[maybe_unused]] const int argc,
		[[maybe_unused]] const char *const *const argv )
{

	run_test("ehanc::for_each_pair", &test_for_each_pair);


	return 0;
}
