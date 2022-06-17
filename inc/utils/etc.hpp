#ifndef EHANC_UTILS_ETC_HPP
#define EHANC_UTILS_ETC_HPP

#include <cstddef>
#include <algorithm>

namespace ehanc {

	/* {{{ doc */
	/**
	 * @brief Checks if all values in `cont` pass some predicate `func`.
	 *
	 * @tparam Container Container type which provides iterators.
	 *
	 * @tparam Predicate Unary function which takes the type contained in
	 * `cont` and returns a bool.
	 *
	 * @param cont Container providing values to check.
	 *
	 * @param pred Condition to check all values in `cont` against.
	 */
	/* }}} */
	template<typename Container, typename Predicate>
	bool all_pass(const Container& cont, Predicate pred) {
		bool check{true};
		std::for_each(cont.cbegin(), cont.cend(),
				[&pred, &check](const auto& i) {
					check = check && pred(i);
				});
		return check;
	}

	/* {{{ doc */
	/**
	 * @brief Checks if all values in iterator range
	 * pass some predicate `func`.
	 *
	 * @tparam Itr Iterator type.
	 *
	 * @tparam Predicate Unary function which takes the type pointed to by
	 * the iterators and returns a bool.
	 *
	 * @param begin Begin iterator of range to check.
	 *
	 * @param end End iterator of range to check.
	 *
	 * @param pred Condition to check all values in cont against.
	 */
	/* }}} */
	template<typename Itr, typename Predicate>
	bool all_pass(Itr begin, const Itr end, Predicate pred)
	{
		bool check{true};
		std::for_each(begin, end, [&pred, &check](const auto& i) {
					check = check && pred(i);
				});
		return check;
	}

	/* {{{ doc */
	/**
	 * @brief Base case of recursive overload.
	 *
	 * @tparam Container Container which provides a `.size()`
	 * member function.
	 *
	 * @param cont Container whose size to query.
	 *
	 * @return Size of container `cont`.
	 */
	/* }}} */
	template<typename Container>
	constexpr std::size_t min_size(const Container& cont) noexcept
	{
		return cont.size();
	}

	/* {{{ doc */
	/**
	 * @brief Returns size of smallest container provided.
	 * Calls size method internally.
	 *
	 * @tparam Container Container which provides a `.size()`
	 * member function.
	 *
	 * @tparam Containers Sequence of container types which all provide
	 * a `.size()` member function. Types may be mixed, so long as all
	 * `.size()` member functions return comparable types which can be
	 * passed to `std::min`.
	 *
	 * @param cont First container.
	 *
	 * @param conts Pack of remaining containers.
	 *
	 * @return Size of smallest container.
	 */
	/* }}} */
	template<typename Container, typename... Containers>
	constexpr std::size_t min_size(const Container& cont,
			const Containers&... conts) noexcept
	{
		return std::min( min_size(cont) , min_size(conts...) );
	}

	/* {{{ doc */
	/**
	 * @brief Metafunction to determine the type resulting from
	 * addition of the two parameter types.
	 */
	/* }}} */
	template<typename T, typename U>
	struct sum_type {
		using type = decltype(std::declval<T>() + std::declval<U>());
	};

	/* {{{ doc */
	/**
	 * @brief Helper variable template to make using the `sum_type`
	 * metafunction less verbose and cumbersome.
	 */
	/* }}} */
	template<typename T, typename U>
	using sum_type_t = typename sum_type<T, U>::type;

	/* {{{ doc */
	/**
	 * @brief Metafunction to determine if two types have the
	 * same size on the stack. Third template parameter must
	 * be defaulted for intended behavior.
	 *
	 */
	/* }}} */
	template<typename T, typename U,
		typename B = std::conditional_t<sizeof(T) == sizeof(U),
			std::true_type, std::false_type>>
	struct are_same_size : B {};

	/* {{{ doc */
	/**
	 * @brief Helper variable template to make using the `are_same_size`
	 * metafunction less verbose and cumbersome.
	 */
	/* }}} */
	template<typename T, typename U>
	constexpr inline const bool are_same_size_v
		= are_same_size<T, U>::value;

} // namespace ehanc

namespace ehanc::literals {

	inline namespace size_t_literal {

		/* {{{ doc */
		/**
		 * @brief Makes it possible to declare a `std::size_t` literal.
		 *
		 * @param i Integer literal to be used as a `std::size_t`
		 */
		/* }}} */
		[[nodiscard]] constexpr auto operator""_z(unsigned long long i)
			noexcept -> std::size_t
			{
				return static_cast<std::size_t>(i);
			}

	} // namespace ehanc::literals::size_t_literal

} // namespace ehanc::literals

#endif
