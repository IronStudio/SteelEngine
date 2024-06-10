#include "se/duration.hpp"



namespace se {
	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	Duration<T, Ratio>::Duration() :
		m_val {}
	{

	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	Duration<T, Ratio>::Duration(T val) :
		m_val {val}
	{

	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename T2, typename Ratio2>
	Duration<T, Ratio>::Duration(const se::Duration<T2, Ratio2> &duration) :
		m_val {}
	{
		*this = duration;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename T2, typename Ratio2>
	const se::Duration<T, Ratio> &Duration<T, Ratio>::operator=(const se::Duration<T2, Ratio2> &duration) {
		if constexpr (std::is_same_v<Ratio, Ratio2>)
			m_val = static_cast<T> (static_cast<T2> (duration));
		else
			m_val = static_cast<T> (static_cast<T2> (duration) * Ratio2::ratio / Ratio::ratio);
		return *this;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename T2, typename Ratio2>
	const se::Duration<T, Ratio> &Duration<T, Ratio>::operator+=(const se::Duration<T2, Ratio2> &duration) {
		se::Duration<T, Ratio> converted {duration};
		m_val += converted.m_val;
		return *this;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename T2, typename Ratio2>
	const se::Duration<T, Ratio> &Duration<T, Ratio>::operator-=(const se::Duration<T2, Ratio2> &duration) {
		se::Duration<T, Ratio> converted {duration};
		m_val -= converted.m_val;
		return *this;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename T2, typename Ratio2>
	se::Duration<T, Ratio> Duration<T, Ratio>::operator+(const se::Duration<T2, Ratio2> &duration) {
		se::Duration<T, Ratio> result {*this};
		result += duration;
		return result;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename T2, typename Ratio2>
	se::Duration<T, Ratio> Duration<T, Ratio>::operator-(const se::Duration<T2, Ratio2> &duration) {
		se::Duration<T, Ratio> result {*this};
		result -= duration;
		return result;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	Duration<T, Ratio>::operator T() const noexcept {
		return m_val;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename Ratio2>
	bool Duration<T, Ratio>::operator==(const se::Duration<T, Ratio2> &duration) const {
		se::Duration<T, Ratio> converted {duration};
		return m_val == converted.m_val;
	}



	template <typename T, typename Ratio>
	SE_REQUIRES(se::concepts::Number<T> && se::concepts::Ratio<Ratio>)
	template <typename Ratio2>
	auto Duration<T, Ratio>::operator<=>(const se::Duration<T, Ratio2> &duration) const {
		se::Duration<T, Ratio> converted {duration};
		return m_val <=> converted.m_val;
	}



	template <typename T, typename Ratio>
	std::ostream &operator<<(std::ostream &stream, const se::Duration<T, Ratio> &duration) {
		stream << static_cast<T> (duration) << " " << Ratio::prefix << "s";
		return stream;
	}



	namespace literals {
		se::Duration<se::Float> operator ""_s(long double val) {
			return se::Duration<se::Float> (val);
		}



		se::Duration<se::Float, se::Milli> operator ""_ms(long double val) {
			return se::Duration<se::Float, se::Milli> (val);
		}



		se::Duration<se::Float, se::Micro> operator "" _μs(long double val) {
			return se::Duration<se::Float, se::Micro> (val);
		}



		se::Duration<se::Float, se::Nano> operator ""_ns(long double val) {
			return se::Duration<se::Float, se::Nano> (val);
		}



		se::Duration<se::Float> operator ""_s(unsigned long long int val) {
			return se::Duration<se::Float> (static_cast<se::Float> (val));
		}



		se::Duration<se::Float, se::Milli> operator ""_ms(unsigned long long int val) {
			return se::Duration<se::Float, se::Milli> (static_cast<se::Float> (val));
		}



		se::Duration<se::Float, se::Micro> operator "" _μs(unsigned long long int val) {
			return se::Duration<se::Float, se::Micro> (static_cast<se::Float> (val));
		}



		se::Duration<se::Float, se::Nano>  operator ""_ns(unsigned long long int val) {
			return se::Duration<se::Float, se::Nano> (static_cast<se::Float> (val));
		}
	} // namespace literals
} // namespace se
