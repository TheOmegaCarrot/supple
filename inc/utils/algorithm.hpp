#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

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

} // namespace ehanc

#endif
