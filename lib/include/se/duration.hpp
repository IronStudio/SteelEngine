#pragma once

#include <ostream>

#include "se/concepts.hpp"
#include "se/units.hpp"



namespace se {
	template <typename T, typename Ratio = se::Ratio<1, 1>>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	class Duration {
		public:
			inline Duration();
			~Duration() = default;

			inline explicit Duration(T val);

			template <typename T2, typename Ratio2>
			inline Duration(const se::Duration<T2, Ratio2> &duration);
			template <typename T2, typename Ratio2>
			inline const se::Duration<T, Ratio> &operator=(const se::Duration<T2, Ratio2> &duration);

			template <typename T2, typename Ratio2>
			inline const se::Duration<T, Ratio> &operator+=(const se::Duration<T2, Ratio2> &duration);
			template <typename T2, typename Ratio2>
			inline const se::Duration<T, Ratio> &operator-=(const se::Duration<T2, Ratio2> &duration);

			template <typename T2, typename Ratio2>
			inline se::Duration<T, Ratio> operator+(const se::Duration<T2, Ratio2> &duration);
			template <typename T2, typename Ratio2>
			inline se::Duration<T, Ratio> operator-(const se::Duration<T2, Ratio2> &duration);

			inline explicit operator T() const noexcept;

		private:
			T m_val;
	};


	template <typename T, typename Ratio>
	std::ostream &operator<<(std::ostream &stream, const se::Duration<T, Ratio> &duration);


	namespace literals {
		inline se::Duration<se::Float> operator ""_s(long double val);
		inline se::Duration<se::Float, se::Milli> operator ""_ms(long double val);
		inline se::Duration<se::Float, se::Micro> operator "" _μs(long double val);
		inline se::Duration<se::Float, se::Nano>  operator ""_ns(long double val);

		inline se::Duration<se::Float> operator ""_s(unsigned long long int val);
		inline se::Duration<se::Float, se::Milli> operator ""_ms(unsigned long long int val);
		inline se::Duration<se::Float, se::Micro> operator "" _μs(unsigned long long int val);
		inline se::Duration<se::Float, se::Nano>  operator ""_ns(unsigned long long int val);
	} // namespace literals
} // namespace se



#include "se/duration.inl"