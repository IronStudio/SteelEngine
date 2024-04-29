#pragma once

#include <ostream>

#include "se/concepts.hpp"
#include "se/units.hpp"



namespace se {
	/**
	 * @brief Helper to store, convert and use time with different units
	 * @tparam T The type to use to store the time
	 * @tparam Ratio A `se::concepts::Ratio`-complient type
	 */
	template <typename T, typename Ratio = se::Ratio<1, 1>>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	class Duration final {
		public:
			inline Duration();
			~Duration() = default;

			/**
			 * @brief Initialized the duration with the value of `val`
			 * @param val The value of the duration, directly in the units you want
			 */
			inline explicit Duration(T val);

			/**
			 * @brief Conversion constructor
			 */
			template <typename T2, typename Ratio2>
			inline Duration(const se::Duration<T2, Ratio2> &duration);
			/**
			 * @brief Conversion operator
			 */
			template <typename T2, typename Ratio2>
			inline const se::Duration<T, Ratio> &operator=(const se::Duration<T2, Ratio2> &duration);

			/**
			 * @brief Add a duration to the current one (implicit cast)
			 */
			template <typename T2, typename Ratio2>
			inline const se::Duration<T, Ratio> &operator+=(const se::Duration<T2, Ratio2> &duration);
			/**
			 * @brief Substract a duration from the current one (implicit cast)
			 */
			template <typename T2, typename Ratio2>
			inline const se::Duration<T, Ratio> &operator-=(const se::Duration<T2, Ratio2> &duration);

			/**
			 * @brief Add two durations (implicit cast)
			 */
			template <typename T2, typename Ratio2>
			inline se::Duration<T, Ratio> operator+(const se::Duration<T2, Ratio2> &duration);
			/**
			 * @brief Substract two durations (implicit cast)
			 */
			template <typename T2, typename Ratio2>
			inline se::Duration<T, Ratio> operator-(const se::Duration<T2, Ratio2> &duration);

			/**
			 * @brief Get the value of the duration, in the duration's unit
			 */
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