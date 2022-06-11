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

			lazy(std::function<RetType ()> func)
				: m_func{func}, m_value{}
			{  }

			lazy(const lazy& src) = default;

			lazy& operator= (const lazy& rhs) = default;

			lazy(lazy&& src) = default;

			lazy& operator= (lazy&& rhs) = default;

			~lazy() = default;

			operator RetType ()
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
