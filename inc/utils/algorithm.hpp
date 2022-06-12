#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <iterator>
#include <functional>

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
	constexpr void for_each_pair( const Itr begin, const Itr end,
						BinaryFunc func)
	noexcept(noexcept(func(*begin, *begin)))
	{
		Itr leader{begin};
		++leader;
		Itr follower{begin};

		for ( ; leader != end; ++leader, ++follower ) {
			func(*leader, *follower);
		}
	}

	/* {{{ doc */
	/**
	 * @brief Applies `func` to each adjacent pair of elements.
	 * Iteration ceases when the range runs out of adjacent pairs,
	 * or when `n` iterations have occurred; whichever comes first.
	 *
	 * Example: Range is: {1, 2, 3}
	 *
	 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
	 *
	 * @param begin Iterator to the beginning of the range.
	 *
	 * @param end Iterator to the end of the range.
	 *
	 * @param n Maximum number of calls to make.
	 *
	 * @param func Binary function to apply to each adjacent pair.
	 */
	/* }}} */
	template<typename Itr, typename BinaryFunc>
	constexpr void for_each_pair_n( const Itr begin, const Itr end,
						int n, BinaryFunc func)
	noexcept(noexcept(func(*begin, *begin)))
	{
		int count{0};
		Itr leader{begin};
		++leader;
		Itr follower{begin};

		for (	; count != n  &&  leader != end
				; ++n, ++leader, ++follower )
		{
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
	constexpr void for_each_both( Itr1 begin1, Itr1 end1,
						Itr2 begin2, Itr2 end2,
						BinaryFunc func)
	noexcept(noexcept(func(*begin1, *begin2)))
	{
		for (	; (begin1 != end1 && begin2 != end2)
				; ++begin1, ++begin2 ) {
			func(*begin1, *begin2);
		}
	}

	/* {{{ doc */
	/**
	 * @brief Applies `func` to each corresponding pair of elements.
	 * Iteration ceases when either range runs out of members,
	 * or when `n` iterations have occurred; whichever comes first.
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
	 * @param n Maximum number of iterations.
	 *
	 * @param func Binary function to apply to each corresponding pair
	 */
	/* }}} */
	template<typename Itr1, typename Itr2, typename BinaryFunc>
	constexpr void for_each_both_n(	Itr1 begin1, Itr1 end1,
							Itr2 begin2, Itr2 end2,
							int n, BinaryFunc func)
	noexcept(noexcept(func(*begin1, *begin2)))
	{
		for (	int count{0}
				; (count != n && begin1 != end1 && begin2 != end2)
				; ++count, ++begin1, ++begin2 )
		{
			func(*begin1, *begin2);
		}
	}

	// WIP:
	/* template<typename VarFunc, typename... Containers> */
	/* constexpr void for_each_all(VarFunc& func, Containers&... containers) */
	/* { */
	/* 	std::vector<const_iterator> its; */
	/* 	for (	(..., its.push_back(std::cbegin(containers))), bool check{false} */
	/* 			; all_pass(its, [](const auto& it) { return it != }) */
	/* 			; std::for_each(its.begin(), its.end() [](auto& it) {++it}) ) */
	/* } */

	/* {{{ doc */
	/**
	 * @brief Applies `func` to members of
	 * all containers in parameter order.
	 * Iteration ceases when after `n` iterations.
	 * If `n` is greater than the size of the smallest container,
	 * behavior is undefined.
	 *
	 * Example: Range1: {1, 2, 3}, Range2: {'a', 'b', 'c'}
	 *
	 * `func` must have accept `int, char` as parameters.
	 *
	 * @param func A function which accepts the types of all the containers
	 * in parameter order.
	 *
	 * @param n Maximum number of iterations. Must not be greater
	 * than the size of the smallest container.
	 *
	 * @param begins Iterators to containers to be iterated over.
	 * Smallest distance from any to its corresponding end must not be less
	 * than `n`, or behavior will be undefined.
	 */
	/* }}} */
	template<typename VarFunc, typename... Begins>
	constexpr void for_each_all_n(VarFunc&& func, int n, Begins... begins)
	{
		for ( int i{0} ; i != n ; ++i ) {
			func((*begins)...);
			((++begins),...);
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
	[[nodiscard]] constexpr auto last(Iterable container) noexcept
			-> decltype(std::begin(container))
	{
		auto end{std::end(container)};
		auto leader{std::begin(container)};
		auto follower{leader};

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
	[[nodiscard]] constexpr auto clast(Iterable container) noexcept
			-> decltype(std::cbegin(container))
	{
		auto end{std::cend(container)};
		auto leader{std::cbegin(container)};
		auto follower{leader};

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
