#ifndef ETC_HPP
#define ETC_HPP

#include <algorithm>

namespace ehanc {

	template<typename Container, typename BoolTFunc>
	bool all_pass(const Container& vec, BoolTFunc func) {
		bool check{true};
		std::for_each(vec.cbegin(), vec.cend(),
				[&func, &check](const auto& i) {
					check = check && func(i);
				});
		return check;
	}


} // namespace ehanc

#endif
