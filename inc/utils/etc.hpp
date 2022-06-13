#ifndef EHANC_UTILS_ETC_HPP
#define EHANC_UTILS_ETC_HPP

#include <algorithm>

namespace ehanc {

	/* {{{ doc */
	/**
	 * @brief Checks if all values in `cont` pass some predicate `func`.
	 *
	 * @param cont Container providing values to check.
	 *
	 * @param func Condition to check all values in cont against.
	 */
	/* }}} */
	template<typename Container, typename Predicate>
	bool all_pass(const Container& cont, Predicate func) {
		bool check{true};
		std::for_each(cont.cbegin(), cont.cend(),
				[&func, &check](const auto& i) {
					check = check && func(i);
				});
		return check;
	}

	/* {{{ doc */
	/**
	 * @brief Checks if all values in iterator range
	 * pass some predicate `func`.
	 *
	 * @param begin Begin iterator of range to check.
	 *
	 * @param end End iterator of range to check.
	 *
	 * @param func Condition to check all values in cont against.
	 */
	/* }}} */
	template<typename Itr, typename Predicate>
	bool all_pass(Itr begin, const Itr end, Predicate func)
	{
		bool check{true};
		std::for_each(begin, end, [&func, &check](const auto& i) {
					check = check && func(i);
				});
		return check;
	}

	/* {{{ doc */
	/**
	 * @brief Base case of recursive overload.
	 *
	 * @param cont Container whose size to query.
	 *
	 * @return Size of container `cont`.
	 */
	/* }}} */
	template<typename Container>
	constexpr size_t min_size(const Container& cont) noexcept
	{
		return cont.size();
	}

	/* {{{ doc */
	/**
	 * @brief Returns size of smallest container provided.
	 * Calls size method internally.
	 *
	 * @param cont First container.
	 *
	 * @param conts Pack of remaining containers.
	 *
	 * @return Size of smallest container.
	 */
	/* }}} */
	template<typename Container, typename... Containers>
	constexpr size_t min_size(const Container& cont,
			const Containers&... conts) noexcept
	{
		return std::min( min_size(cont) , min_size(conts...) );
	}


} // namespace ehanc

#endif
