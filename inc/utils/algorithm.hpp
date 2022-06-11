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
	 * @brief Applies `func` to each corresponding pair of elements.
	 * Iteration ceases when either range runs out of members.
	 *
	 * Example: Range 1 is: {1, 2, 3}, Range 2 is: {4, 5, 6, 7}
	 *
	 * Calls to `func` will be:
	 *
	 * * `func(1, 4)`
	 * * `func(2, 5)`
	 * * `func(3, 6)`
	 *
	 * @param begin1 Iterator to the beginning of range 1
	 *
	 * @param end1 Iterator to the end of range 1
	 *
	 * @param begin2 Iterator to the beginning of range 2
	 *
	 * @param end2 Iterator to the end of range 2
	 *
	 * @param func Binary function to apply to each corresponding pair
	 */
	/* }}} */
	template<typename Itr1, typename Itr2, typename BinaryFunc>
	void for_each_both( Itr1 begin1, Itr1 end1,
						Itr2 begin2, Itr2 end2,
						BinaryFunc func)
	{
		for (	; (begin1 != end1 && begin2 != end2)
				; ++begin1, ++begin2 ) {
			func(*begin1, *begin2);
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
