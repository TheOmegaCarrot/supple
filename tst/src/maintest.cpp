#include <iostream>

#include "test_utils.h"
#include "utils/term_colors.h"
#include "test_algorithm.h"
#include "test_lazy.h"
#include "test_etc.h"

constexpr static const std::string_view HEADER_COLOR = ehanc::FG_RED;

int main()
{

	std::cout << HEADER_COLOR << "Algorithm:" << ehanc::RESET << '\n';
	test_algorithm();

	std::cout << '\n' << '\n';

	std::cout << HEADER_COLOR << "Lazy:" << ehanc::RESET << '\n';
	test_lazy();

	std::cout << '\n' << '\n';

	std::cout << HEADER_COLOR << "Etc:" << ehanc::RESET << '\n';
	test_etc();

	return 0;
}
