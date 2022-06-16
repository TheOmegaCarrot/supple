#include <iostream>
#include <utility>

#include "utils/all.h"

/// @cond

struct loud
{

	int m_val;

	loud() = delete;

	loud(int val)
		: m_val{val}
	{  }

	loud(const loud& src)
		: m_val{src.m_val}
	{
		std::cout << "COPY" << '\n';
	}

	loud(loud&& src)
		: m_val{src.m_val}
	{
		std::cout << "MOVE" << '\n';
	}

	int operator()() const
	{
		std::cout << "EVAL" << '\n';
		return m_val;
	}
};

template<typename T>
auto printer(const T& print)
{
	std::cout << print << '\n';
	std::cout << print << '\n';
}

int main( [[maybe_unused]] const int argc,
		[[maybe_unused]] const char *const *const argv )
{

	ehanc::lazy<int> test([i{loud(7)}](){return i();});

	std::cout << test << '\n';
	std::cout << test << '\n';

	ehanc::lazy<int> test2(loud(9));

	std::cout << test2 << '\n';
	std::cout << test2 << '\n';

	auto test3{ehanc::make_lazy([i{loud(3)}](){return i();})};

	std::cout << test3 << '\n';
	std::cout << test3 << '\n';

	auto test4{ehanc::make_lazy(loud(6))};

	std::cout << test4 << '\n';
	std::cout << test4 << '\n';

	auto test5{ehanc::make_lazy([i{loud(14)}](){return i();})};

	printer(test5);
	printer(test5);

	std::cout << std::boolalpha <<
		ehanc::is_lazy_v<decltype(test5)> << '\n' <<
		ehanc::is_lazy_v<int> << '\n';

	std::cout << std::boolalpha <<
		ehanc::is_lazy<decltype(test5)>::value << '\n' <<
		ehanc::is_lazy<int>::value << '\n';

	ehanc::lazy deduced([i{loud(2)}](){return i();});

	std::cout << deduced << '\n';
	std::cout << deduced << '\n';

	std::cout << '\n' << '\n';

	std::cout << std::boolalpha
		<< ehanc::is_lazy_of<int, decltype(test5)>::value << '\n'
		<< ehanc::is_lazy_of<double, decltype(test5)>::value << '\n'
		<< ehanc::is_lazy_of_v<int, decltype(test5)> << '\n'
		<< ehanc::is_lazy_of_v<double, decltype(test5)> << '\n';

	return 0;
}
