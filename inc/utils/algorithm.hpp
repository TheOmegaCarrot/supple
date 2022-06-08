#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <iterator>

namespace ehanc {

	/* {{{ doc */
	/**
	 * @brief Applies `func` to each adjacent pair of elements.
	 *
	 * Example: Range is: {1, 2, 3}
	 *
	 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
	 *
	 * @param begin Iterator to the beginning of the range.
	 *
	 * @param end Iterator to the end of the range.
	 *
	 * @param func Binary function to apply to each adjacent pair.
	 */
	/* }}} */
	template<typename Itr, typename BinaryFunc>
	void for_each_pair(
			const Itr begin, const Itr end, BinaryFunc func)
	{
		Itr leader = begin;
		++leader;
		Itr follower = begin;

		for ( ; leader != end; ++leader, ++follower ) {
			func(*leader, *follower);
		}
	}

	/* {{{ doc */
	/**
	 * @brief Returns an iterator to the last element of the container.
	 * In short, `++ehanc::last(container) == std::end(container)`
	 *
	 * @param container Container which supports at least forward iterators.
	 *
	 * @return Iterator to the last element of the container.
	 */
	/* }}} */
	template<typename Iterable>
	[[nodiscard]] auto last(Iterable container)
			-> decltype(std::begin(container))
	{
		auto end = std::end(container);
		auto leader = std::begin(container);
		auto follower = leader;

		if ( follower == end ) {
			return follower;
		}

		while (++leader != end) {
			++follower;
		}

		return follower;

	}

	/* {{{ doc */
	/**
	 * @brief Returns a const iterator to the last element of the container.
	 * In short, `++ehanc::clast(container) == std::end(container)`
	 *
	 * @param container Container which supports at least forward iterators.
	 *
	 * @return Const iterator to the last element of the container.
	 */
	/* }}} */
	template<typename Iterable>
	[[nodiscard]] auto clast(Iterable container)
			-> decltype(std::cbegin(container))
	{
		auto end = std::cend(container);
		auto leader = std::cbegin(container);
		auto follower = leader;

		if ( follower == end ) {
			return follower;
		}

		while (++leader != end) {
			++follower;
		}

		return follower;
	}

} // namespace ehanc

#endif
