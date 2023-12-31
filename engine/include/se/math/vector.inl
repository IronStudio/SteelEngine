#include "vector.hpp"

#ifndef SE_NO_VECTORIZATION
	#include <smmintrin.h>
#endif

#include <algorithm>
#include <cmath>



namespace se
{
	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Vector<T, D>::Vector(T2 fill) :
		m_datas {}
	{
		std::fill(m_datas.begin(), m_datas.end(), static_cast<T> (fill));
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2, se::Length D2>
	requires std::is_arithmetic_v<T2> && (D2 <= D)
	Vector<T, D>::Vector(const std::array<T2, D2> &datas) :
		m_datas {}
	{
		std::copy(datas.begin(), datas.end(), m_datas.begin());
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2, se::Length D2>
	requires std::is_arithmetic_v<T2> && (D2 <= D)
	Vector<T, D>::Vector(const se::Vector<T2, D2> &vector) :
		m_datas {}
	{
		std::copy(vector.getInternalArray().begin(), vector.getInternalArray().end(), m_datas.begin());
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2, typename ...Args>
	requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= (D - 1))
	Vector<T, D>::Vector(T2 first, Args ...args) :
		m_datas {static_cast<T> (first), static_cast<T> (args)...}
	{

	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2, se::Length D2>
	requires std::is_arithmetic_v<T2> && (D2 <= D)
	const se::Vector<T, D> &Vector<T, D>::operator=(const se::Vector<T, D> &vector)
	{
		std::copy(vector.getInternalArray().begin(), vector.getInternalArray().end(), m_datas.begin());
		return *this;
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	T &Vector<T, D>::operator[] (se::Length index)
	{
		return m_datas[index];
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	T Vector<T, D>::operator[] (se::Length index) const
	{
		return m_datas[index];
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Vector<T, D> &Vector<T, D>::operator+=(const se::Vector<T2, D> &vector)
	{
		#ifndef SE_NO_VECTORIZATION
			if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && D <= 4)
			{
				__m128 dst {_mm_load_ps(m_datas.data())};
				__m128 src {_mm_load_ps(vector.getInternalArray().data())};
				dst = _mm_add_ps(dst, src);
				_mm_store_ps(m_datas.data(), dst);
				return *this;
			}

			else
			{
		#endif

		for (se::Length i {0}; i < D; ++i)
			m_datas[i] += vector[i];
		return *this;

		#ifndef SE_NO_VECTORIZATION
			}
		#endif
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Vector<T, D> &Vector<T, D>::operator-=(const se::Vector<T2, D> &vector)
	{
		#ifndef SE_NO_VECTORIZATION
			if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && D <= 4)
			{
				__m128 dst {_mm_load_ps(m_datas.data())};
				__m128 src {_mm_load_ps(vector.getInternalArray().data())};
				dst = _mm_sub_ps(dst, src);
				_mm_store_ps(m_datas.data(), dst);
				return *this;
			}

			else
			{
		#endif

		for (se::Length i {0}; i < D; ++i)
			m_datas[i] -= vector[i];
		return *this;

		#ifndef SE_NO_VECTORIZATION
			}
		#endif
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Vector<T, D> &Vector<T, D>::operator*=(const se::Vector<T2, D> &vector)
	{
		#ifndef SE_NO_VECTORIZATION
			if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && D <= 4)
			{
				__m128 dst {_mm_load_ps(m_datas.data())};
				__m128 src {_mm_load_ps(vector.getInternalArray().data())};
				dst = _mm_mul_ps(dst, src);
				_mm_store_ps(m_datas.data(), dst);
				return *this;
			}

			else
			{
		#endif

		for (se::Length i {0}; i < D; ++i)
			m_datas[i] *= vector[i];
		return *this;

		#ifndef SE_NO_VECTORIZATION
			}
		#endif
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Vector<T, D> &Vector<T, D>::operator*=(T2 scalar)
	{
		#ifndef SE_NO_VECTORIZATION
			if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && D <= 4)
			{
				__m128 dst {_mm_load_ps(m_datas.data())};
				__m128 src {_mm_set1_ps(scalar)};
				dst = _mm_mul_ps(dst, src);
				_mm_store_ps(m_datas.data(), dst);
				return *this;
			}

			else
			{
		#endif

		for (se::Length i {0}; i < D; ++i)
			m_datas[i] *= scalar;
		return *this;

		#ifndef SE_NO_VECTORIZATION
			}
		#endif
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	T Vector<T, D>::length() const noexcept
	{
		#ifndef SE_NO_VECTORIZATION
			if constexpr (std::is_same_v<T, se::Float32> && D <= 4)
			{
				__m128 dst {_mm_load_ps(m_datas.data())};
				dst = _mm_dp_ps(dst, dst, 0b11111111);
				dst = _mm_sqrt_ps(dst);
				float output[4];
				_mm_store_ps(output, dst);
				return output[0];
			}

			else
			{
		#endif

		return sqrtf(se::dot(*this, *this));

		#ifndef SE_NO_VECTORIZATION
			}
		#endif
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	T Vector<T, D>::length2() const noexcept
	{
		return se::dot(*this, *this);
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	const std::array<T, D >= 4 ? D : 4> &Vector<T, D>::getInternalArray() const noexcept
	{
		return m_datas;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	bool operator==(const se::Vector<T, D> &a, const se::Vector<T2, D> &b)
	{
		for (se::Length i {0}; i < D; ++i)
		{
			if (a.getInternalArray()[i] != b.getInternalArray()[i])
				return false;
		}

		return true;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Vector<T, D> operator+(se::Vector<T, D> a, const se::Vector<T2, D> b)
	{
		a += b;
		return a;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Vector<T, D> operator-(se::Vector<T, D> a, const se::Vector<T2, D> b)
	{
		a -= b;
		return a;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Vector<T, D> operator*(se::Vector<T, D> a, const se::Vector<T2, D> b)
	{
		a *= b;
		return a;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Vector<T, D> operator*(se::Vector<T, D> vector, T2 scalar)
	{
		vector *= scalar;
		return vector;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Vector<T, D> operator*(T2 scalar, se::Vector<T, D> vector)
	{
		vector *= scalar;
		return vector;
	}



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	T dot(const se::Vector<T, D> &a, const se::Vector<T2, D> &b)
	{
		#ifndef SE_NO_VECTORIZATION
			if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && D <= 4)
			{
				__m128 dst {_mm_load_ps(a.getInternalArray().data())};
				__m128 src {_mm_load_ps(b.getInternalArray().data())};
				dst = _mm_dp_ps(dst, src, 0b11111111);
				float output[4];
				_mm_store_ps(output, dst);
				return output[0];
			}

			else
			{
		#endif

		T result {};
		for (se::Length i {0}; i < D; ++i)
			result += a.getInternalArray()[i] * b.getInternalArray()[i];

		return result;

		#ifndef SE_NO_VECTORIZATION
			}
		#endif
	}



	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	std::ostream &operator<<(std::ostream &stream, const se::Vector<T, D> &vector)
	{
		stream << "[";
		for (se::Length i {0}; i < D - 1; ++i)
			stream << vector[i] << ", ";
		stream << vector[D - 1] << "]";
		return stream;
	}



} // namespace se
