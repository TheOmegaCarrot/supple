#ifndef EHANC_UTILS_ALGORITHM_HPP
#define EHANC_UTILS_ALGORITHM_HPP

#include <iterator>
#include <functional>

#include "utils/etc.hpp"

namespace ehanc {

	/* {{{ doc */
	/**
	 * @brief Applies `func` to each adjacent pair of elements.
	 *
	 * Example: Range is: {1, 2, 3}
	 *
	 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
	 *
	 * @tparam Itr Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @tparam BinaryFunc Function which takes two arguments, and
	 * for both must accept the type which the iterators point to.
	 *
	 * @param begin Iterator to the beginning of the range.
	 *
	 * @param end Iterator to the end of the range.
	 *
	 * @param func Binary function to apply to each adjacent pair.
	 */
	/* }}} */
	template<typename Itr, typename BinaryFunc>
	constexpr void for_each_pair(const Itr begin, const Itr end,
						BinaryFunc&& func)
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
	 * @tparam Itr Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @tparam BinaryFunc Function which takes two arguments, and
	 * for both must accept the type which the iterators point to.
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
						const int n, BinaryFunc&& func)
	noexcept(noexcept(func(*begin, *begin)))
	{
		int count{0};
		Itr leader{begin};
		++leader;
		Itr follower{begin};

		for (	; count != n  &&  leader != end
				; ++count, ++leader, ++follower )
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
	 * @tparam Itr1 Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @tparam Itr2 Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @tparam BinaryFunc Function which takes two arguments, and
	 * for both must accept the type which the iterators point to.
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
	constexpr void for_each_both( Itr1 begin1, const Itr1 end1,
						Itr2 begin2, const Itr2 end2,
						BinaryFunc&& func)
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
	 * @tparam Itr1 Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @tparam Itr2 Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @tparam BinaryFunc Function which takes two arguments, and
	 * for both must accept the type which the iterators point to.
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
	constexpr void for_each_both_n(	Itr1 begin1, const Itr1 end1,
							Itr2 begin2, const Itr2 end2,
							const int n, BinaryFunc&& func)
	noexcept(noexcept(func(*begin1, *begin2)))
	{
		for (	int count{0}
				; (count != n && begin1 != end1 && begin2 != end2)
				; ++count, ++begin1, ++begin2 )
		{
			func(*begin1, *begin2);
		}
	}

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
	 * @tparam VarFunc A function which acccepts the types of all the
	 * contaners in parameter order.
	 *
	 * @tparam Begins Sequence of iterators satisfying only a minimal
	 * inteface. Simply being incrementable is sufficient.
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
	constexpr void for_each_all_n(VarFunc&& func, const size_t n, Begins... begins)
	noexcept(noexcept(func(*begins...)))
	{
		for ( size_t i{0} ; i != n ; ++i ) {
			func(*begins...);
			(++begins,...);
		}
	}

	template<typename VarFunc, typename... Containers>
	constexpr void for_each_all_c(VarFunc&& func, const Containers&... containers)
	{
		for_each_all_n(std::forward<VarFunc>(func),
				ehanc::min_size(containers...), std::cbegin(containers)...);
	}

	template<typename VarFunc, typename... Containers>
	constexpr void for_each_all(VarFunc&& func, Containers&... containers)
	{
		for_each_all_n(std::forward<VarFunc>(func),
				min_size(containers...), std::begin(containers)...);
	}

	/* {{{ doc */
	/**
	 * @brief Computes distance between two iterators.
	 * It is assumed `begin` is before `end`, meaning that incrementing
	 * `begin` will eventually yield an iterator equal to `end`; if this
	 * is not the case, behavior is undefined.
	 *
	 * @tparam Itr Iterator type satisfying only a minimal interface.
	 * Simply being incrementable and equality-comparable is sufficient.
	 *
	 * @param begin Beginning iterator.
	 *
	 * @param end End iterator.
	 *
	 * @return Distance between begin and end. Distance being defined as
	 * the number of times `begin` must be incremented to equal `end`.
	 */
	/* }}} */
	template<typename Itr>
	[[nodiscard]] constexpr auto forward_distance(Itr begin, const Itr end)
			noexcept -> size_t
	{
		size_t count{0};
		while (begin != end) {
			++begin;
			++count;
		}
		return count;
	}

	/* {{{ doc */
	/**
	 * @brief Returns an iterator to the last element of the container.
	 * In short, `++ehanc::last(container) == std::end(container)`
	 *
	 * @tparam Iterable Type which provides iterators.
	 *
	 * @param container Container which supports at least forward iterators.
	 *
	 * @return Iterator to the last element of the container.
	 */
	/* }}} */
	template<typename Iterable>
	[[nodiscard]] constexpr auto last(const Iterable& container) noexcept
			-> decltype(std::begin(container))
	{
		auto begin{std::begin(container)};
		auto end{std::end(container)};

		if ( begin == end ) {
			return begin;
		}

		size_t distance{forward_distance(begin, end) - 1};
		for ( size_t i{0} ; i != distance ; ++i ) {
			++begin;
		}

		return begin;
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
	[[nodiscard]] constexpr auto clast(const Iterable& container) noexcept
			-> decltype(std::cbegin(container))
	{
		return static_cast<decltype(std::cbegin(container))>(last(container));
	}

} // namespace ehanc

#endif
