#include <iostream>
#include <iomanip>

#include "utils/term_colors.h"

#include "test_utils.h"

static const int TEST_OUTPUT_WIDTH = 60;

void result_message(const std::string_view function_name,
					const std::string_view result,
					const std::string_view color)
{
	std::cout << std::left << std::setw(TEST_OUTPUT_WIDTH) << std::setfill('.')
		<< function_name << color << result << ehanc::RESET << '\n';
}

bool run_test(const std::string_view function_name,
				std::function<bool()> to_test)
{

	bool result = to_test();

	if ( result )
	{
		result_message(function_name, "PASS", ehanc::FG_GREEN);
	} else
	{
		result_message(function_name, "FAIL", ehanc::FG_RED);
	}

	return result;
}

