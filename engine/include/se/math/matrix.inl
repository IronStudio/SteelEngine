#include "matrix.hpp"

#include <smmintrin.h>

#include <cstring>



namespace se
{
	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	Matrix<T, R, C>::Matrix() :
		m_datas {}
	{

	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Matrix<T, R, C>::Matrix(T2 fill) :
		m_datas {}
	{
		for (se::Length i {0}; i < R * C; ++i)
			m_datas[i] = fill;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2, se::Length S>
	requires std::is_arithmetic_v<T2> && (S <= R * C)
	Matrix<T, R, C>::Matrix(const std::array<T2, S> &datas) :
		m_datas {}
	{
		for (se::Length i {0}; i < S; ++i)
			m_datas[i] = datas[i];
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2, typename ...Args>
	requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= (R * C - 1))
	Matrix<T, R, C>::Matrix(T2 first, Args ...args) :
		m_datas {static_cast<T> (first), static_cast<T> (args)...}
	{
		
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Matrix<T, R, C>::Matrix(const se::Matrix<T2, R, C> &matrix) :
		m_datas {}
	{
		for (se::Length i {0}; i < R * C; ++i)
			m_datas[i] = matrix.getInternalArray()[i];
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Matrix<T, R, C> &Matrix<T, R, C>::operator=(const se::Matrix<T2, R, C> &matrix)
	{
		for (se::Length i {0}; i < R * C; ++i)
			m_datas[i] = matrix.getInternalArray()[i];
		return *this;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	std::span<T, C> Matrix<T, R, C>::operator[] (se::Length index)
	{
		return std::span<T, C> (std::span<T, R * C> (m_datas).subspan(index * C, C));
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	const se::Vector<T, C> Matrix<T, R, C>::operator[] (se::Length index) const
	{
		se::Vector<T, C> vector {};
		for (se::Length i {0}; i < C; ++i)
			vector[i] = m_datas[index * C + i];
		return vector;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Matrix<T, R, C> &Matrix<T, R, C>::operator+=(const se::Matrix<T2, R, C> &matrix)
	{
		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && R == 4 && R == C)
		{
			for (se::Length i {0}; i < R; ++i)
			{
				__m128 dst {_mm_load_ps(m_datas.data() + i * R)};
				__m128 src {_mm_load_ps(matrix.getInternalArray().data() + i * R)};
				dst = _mm_add_ps(dst, src);
				_mm_store_ps(m_datas.data() + i * R, dst);
			}

			return *this;
		}

		for (se::Length i {0}; i < R * C; ++i)
			m_datas[i] += matrix.getInternalArray()[i];

		return *this;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Matrix<T, R, C> &Matrix<T, R, C>::operator-=(const se::Matrix<T2, R, C> &matrix)
	{
		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && R == 4 && R == C)
		{
			for (se::Length i {0}; i < R; ++i)
			{
				__m128 dst {_mm_load_ps(m_datas.data() + i * R)};
				__m128 src {_mm_load_ps(matrix.getInternalArray().data() + i * R)};
				dst = _mm_sub_ps(dst, src);
				_mm_store_ps(m_datas.data() + i * R, dst);
			}

			return *this;
		}

		for (se::Length i {0}; i < R * C; ++i)
			m_datas[i] -= matrix.getInternalArray()[i];

		return *this;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Matrix<T, R, C> &Matrix<T, R, C>::operator*=(T2 scalar)
	{
		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && R == 4 && R == C)
		{
			for (se::Length i {0}; i < R; ++i)
			{
				__m128 dst {_mm_load_ps(m_datas.data() + i * R)};
				__m128 src {_mm_set_ps1(scalar)};
				dst = _mm_mul_ps(dst, src);
				_mm_store_ps(m_datas.data() + i * R, dst);
			}

			return *this;
		}

		for (se::Length i {0}; i < R * C; ++i)
			m_datas[i] *= scalar;

		return *this;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	inline const std::array<T, R * C> &Matrix<T, R, C>::getInternalArray() const noexcept
	{
		return m_datas;
	}



	template <typename T, se::Length R, se::Length C>
	std::ostream &operator<<(std::ostream &stream, const se::Matrix<T, R, C> &matrix)
	{
		for (se::Length i {0}; i < R; ++i)
		{
			for (se::Length j {0}; j < C; ++j)
			{
				stream << matrix.getInternalArray()[i * C + j];
				if (i != R - 1 || j != C - 1)
					stream << ", ";
			}

			if (i != R - 1)
				stream << "\n";
		}

		return stream;
	}



	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator+(se::Matrix<T, R, C> a, const se::Matrix<T2, R, C> &b)
	{
		a += b;
		return a;
	}



	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator-(se::Matrix<T, R, C> a, const se::Matrix<T2, R, C> &b)
	{
		a -= b;
		return b;
	}



	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator*(se::Matrix<T, R, C> matrix, T2 scalar)
	{
		matrix *= scalar;
		return scalar;
	}



	template <typename T, typename T2, se::Length R, se::Length S, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator*(const se::Matrix<T, R, S> &a, const se::Matrix<T2, S, C> &b)
	{
		se::Matrix<T, R, C> result {};

		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2> && R == 4 && R == C && R == S)
		{
			auto bColumnMajor {se::transpose(b)};

			for (se::Length i {0}; i < R; ++i)
			{
				__m128 aRow {_mm_load_ps(a.getInternalArray().data() + i * R)};

				for (se::Length j {0}; j < R; ++j)
				{
					__m128 bRow {_mm_load_ps(bColumnMajor.getInternalArray().data() + j * R)};
					__m128 res {_mm_dp_ps(aRow, bRow, 0b11111111)};
					float output[4] {};
					_mm_store_ps(output, res);
					result[i][j] = output[0];
				}
			}

			return result;
		}

		
		for (se::Length i {0}; i < R; ++i)
		{
			for (se::Length j {0}; j < C; ++j)
			{
				T output {0};

				for (se::Length k {0}; k < S; ++k)
					output += a.getInternalArray()[i * S + k] * b.getInternalArray()[k * C + j];

				result[i][j] = output;
			}
		}

		return result;
	}



	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	bool operator==(const se::Matrix<T, R, C> &a, const se::Matrix<T2, R, C> &b)
	{
		for (se::Length i {0}; i < R * C; ++i)
		{
			if (a.getInternalArray()[i] != b.getInternalArray()[i])
				return false;
		}

		return true;
	}



	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	se::Matrix<T, C, R> transpose(const se::Matrix<T, R, C> &matrix)
	{
		se::Matrix<T, C, R> result {};
		for (se::Length i {0}; i < R; ++i)
		{
			for (se::Length j {0}; j < C; ++j)
				result[j][i] = matrix.getInternalArray()[i * C + j];
		}

		return result;
	}



} // namespace se
