#ifndef LAZY_HPP
#define LAZY_HPP

#include <functional>
#include <optional>

namespace ehanc {

	template<typename RetType>
	class lazy
	{
		private:

			std::function<RetType ()> m_func;
			std::optional<RetType> m_value{};

		public:

			lazy() = delete;

			constexpr lazy(std::function<RetType ()> func)
				noexcept(noexcept(func()))
				: m_func{func}, m_value{}
			{  }

			constexpr lazy(const lazy& src)
				noexcept(noexcept(src.func()))
				= default;

			constexpr lazy& operator= (const lazy& rhs)
				noexcept(noexcept(rhs.func()))
				= default;

			constexpr lazy(lazy&& src)
				noexcept(noexcept(src.func()))
				= default;

			constexpr lazy& operator= (lazy&& rhs)
				noexcept(noexcept(rhs.func()))
				= default;

			~lazy() noexcept(noexcept(m_func())) = default;

			constexpr operator RetType () noexcept(noexcept(m_func()))
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
