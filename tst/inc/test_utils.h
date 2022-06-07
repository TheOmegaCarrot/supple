#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <string_view>
#include <functional>

void result_message(const std::string_view function_name,
					const std::string_view result,
					const std::string_view color);
bool run_test(const std::string_view function_name,
				std::function<bool()> to_test);

#endif
