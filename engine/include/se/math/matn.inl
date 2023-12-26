#include "matn.hpp"



namespace se
{
	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T>
	Matn<T, N>::Matn() :
		se::Matrix<T, N, N> ()
	{

	}



	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Matn<T, N>::Matn(T2 diagonal) :
		se::Matrix<T, N, N> ()
	{
		for (se::Length i {0}; i < N; ++i)
			this->m_datas[i * N + i] = diagonal;
	}



	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T>
	template <typename T2, typename ...Args>
	requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= (N * N - 1))
	Matn<T, N>::Matn(T2 first, Args ...args) :
		se::Matrix<T, N, N> (first, args...)
	{

	}



	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Matn<T, N>::Matn(const se::Matrix<T2, N, N> &matrix) :
		se::Matrix<T, N, N> (matrix)
	{

	}



	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Matn<T, N> &Matn<T, N>::operator=(const se::Matrix<T2, N, N> &matrix)
	{
		return *this = matrix;
	}



	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T> && (N <= 4)
	T det(const se::Matn<T, N> &matrix)
	{
		if constexpr (N == 2)
		{
			return matrix.getInternalArray()[0] * matrix.getInternalArray()[3]
				- matrix.getInternalArray()[1] * matrix.getInternalArray()[2];
		}

		else if constexpr (N == 3)
		{
			return matrix.getInternalArray()[0] * matrix.getInternalArray()[4] * matrix.getInternalArray()[8]
				+ matrix.getInternalArray()[1] * matrix.getInternalArray()[5] * matrix.getInternalArray()[6]
				+ matrix.getInternalArray()[2] * matrix.getInternalArray()[3] * matrix.getInternalArray()[7]
				- matrix.getInternalArray()[0] * matrix.getInternalArray()[7] * matrix.getInternalArray()[5]
				- matrix.getInternalArray()[1] * matrix.getInternalArray()[3] * matrix.getInternalArray()[8]
				- matrix.getInternalArray()[2] * matrix.getInternalArray()[4] * matrix.getInternalArray()[6];
		}

		else if constexpr (N == 4)
		{
			return se::det(se::Matn<T, 3> (
				matrix.getInternalArray()[5], matrix.getInternalArray()[6], matrix.getInternalArray()[7],
				matrix.getInternalArray()[9], matrix.getInternalArray()[10], matrix.getInternalArray()[11],
				matrix.getInternalArray()[13], matrix.getInternalArray()[14], matrix.getInternalArray()[15]
			)) - se::det(se::Matn<T, 3> (
				matrix.getInternalArray()[4], matrix.getInternalArray()[6], matrix.getInternalArray()[7],
				matrix.getInternalArray()[8], matrix.getInternalArray()[10], matrix.getInternalArray()[11],
				matrix.getInternalArray()[12], matrix.getInternalArray()[14], matrix.getInternalArray()[15]
			)) + se::det(se::Matn<T, 3> (
				matrix.getInternalArray()[4], matrix.getInternalArray()[5], matrix.getInternalArray()[7],
				matrix.getInternalArray()[8], matrix.getInternalArray()[9], matrix.getInternalArray()[11],
				matrix.getInternalArray()[12], matrix.getInternalArray()[13], matrix.getInternalArray()[15]
			)) - se::det(se::Matn<T, 3> (
				matrix.getInternalArray()[4], matrix.getInternalArray()[5], matrix.getInternalArray()[6],
				matrix.getInternalArray()[8], matrix.getInternalArray()[9], matrix.getInternalArray()[10],
				matrix.getInternalArray()[12], matrix.getInternalArray()[13], matrix.getInternalArray()[14]
			));
		}
	}



} // namespace se
