#ifndef EHANC_UTILS_LAZY_HPP
#define EHANC_UTILS_LAZY_HPP

#include <functional>
#include <optional>

namespace ehanc {

	/* {{{ doc */
	/**
	 * @brief Lazy evaluator. Given a callable which takes no arguments,
	 * this class will seamlessly and implicitly convert to the result
	 * of calling the callable, but only call the callable a single time.
	 *
	 * @tparam RetType Return type of `m_func`. A `lazy` is implicitly
	 * convertible to the object of type `RetType` returned by `m_func`.
	 *
	 * @code
	 *
	 *	ehanc::lazy<int> example([](){return longComputation()});
	 *
	 *	// longComputation() only evaluated once,
	 *	// and its return value is stored.
	 *	use(example);
	 *	use(example);
	 *	use(example);
	 *
	 * @endcode
	 */
	/* }}} */
	template<typename RetType>
	class lazy
	{
		private:

			/* {{{ doc */
			/**
			 * The callable which takes no arguments and returns
			 * a useful value.
			 */
			/* }}} */
			std::function<RetType ()> m_func;

			/* {{{ doc */
			/**
			 * The stored result of calling `m_func`, if it has been
			 * needed.
			 */
			/* }}} */
			std::optional<RetType> m_value;

		public:

			/* {{{ doc */
			/**
			 * Default constructor deleted to prevent
			 * construction without a callable.
			 */
			/* }}} */
			lazy() = delete;

			/* {{{ doc */
			/**
			 * Enforce initialization with an appropriate callable.
			 */
			/* }}} */
			template<typename Func>
			constexpr lazy(Func&& func)
				noexcept
				: m_func{std::forward<Func>(func)}, m_value{}
			{  }

			/* {{{ doc */
			/**
			 * Straightforward to copy.
			 */
			/* }}} */
			constexpr lazy(const lazy& src)
				noexcept
				= default;

			/* {{{ doc */
			/**
			 * Straightforward to copy.
			 */
			/* }}} */
			constexpr lazy& operator= (const lazy& rhs)
				noexcept
				= default;

			/* {{{ doc */
			/**
			 * Straightforward to move.
			 */
			/* }}} */
			constexpr lazy(lazy&& src)
				noexcept
				= default;

			/* {{{ doc */
			/**
			 * Straightforward to move.
			 */
			/* }}} */
			constexpr lazy& operator= (lazy&& rhs)
				noexcept
				= default;

			/* {{{ doc */
			/**
			 * Straightforward to destruct.
			 */
			/* }}} */
			~lazy() noexcept = default;

			/* {{{ doc */
			/**
			 * @brief Allow for implicit conversion to the value returned
			 * by `m_func`, but only ever call `m_func` once.
			 */
			/* }}} */
			[[nodiscard]] constexpr operator RetType ()
				noexcept(noexcept(m_func()))
			{
				if (m_value) {
					return *m_value;
				} else {
					m_value = m_func();
					return *m_value;
				}
			}

	};

} // namespace ehanc

#endif
