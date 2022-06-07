#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace ehanc {

	/* {{{ doc */
	/**
	 * @brief Applies `func` to each adjacent pair of elements.
	 *
	 * @param begin Iterator to the beginning of the range.
	 *
	 * @param end Iterator to the end of the range.
	 *
	 * @param func Binary function to apply to each pair.
	 *
	 * @code {
	 * std::array<int,3> example = {1, 2, 3};
	 * for_each_pair(example.cbegin(), example.cend(),
	 * 		[](const int i, const int j){
	 *			std::cout << i + j << '\t';
	 * 		}); // will print "3		5"
	 * @}
	 */
	/* }}} */
	template<typename Itr, typename BinaryFunc>
		void for_each_pair(const Itr begin, const Itr end, BinaryFunc func)
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
