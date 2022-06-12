#include <iostream>

#include "utils/algorithm.hpp"

int main( [[maybe_unused]] const int argc,
		[[maybe_unused]] const char *const *const argv )
{

	std::array one{1, 2, 3, 4};
	std::array two(one);
	std::array three{4, 3, 2, 1};

	ehanc::for_n_all([](const auto... args) {
				std::cout << (... + args) << '\n';
			}, 4, one.cbegin(), two.cbegin(), three.cbegin());

	std::cout << '\n' << '\n';

	ehanc::for_n_all([](const int a, const int b, const int c) {
				std::cout << a << ' ' << b << ' ' << c << ' ' << '\n';
			}, 4, one.cbegin(), two.cbegin(), three.cbegin());

	return 0;
}
