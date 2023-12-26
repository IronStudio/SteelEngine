#pragma once

#include "matrix.hpp"



namespace se
{
	/**
	 * @brief A class that respresent a square NxN matrix
	*/
	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T>
	class Matn : public se::Matrix<T, N, N>
	{
		public:
			Matn();

			/**
			 * @brief A constructor for a diagonal matrix (for examples the identity)
			 * @param diagonal The value to fill the diagonal with (1 for the identity matrix)
			*/
			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Matn(T2 diagonal);

			template <typename T2, typename ...Args>
			requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= (N * N - 1))
			Matn(T2 first, Args ...args);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Matn(const se::Matrix<T2, N, N> &matrix);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Matn<T, N> &operator=(const se::Matrix<T2, N, N> &matrix);
	};


	/**
	 * @brief Calculate the determinant of `matrx`
	 * @warning Avoid using this **as much as possible** since it is a really slow operation
	*/
	template <typename T, se::Length N>
	requires std::is_arithmetic_v<T> && (N <= 4)
	T det(const se::Matn<T, N> &matrix);


	template <typename T>
	using Mat2 = Matn<T, 2>;

	using Mat2i = Mat2<se::Int32>;
	using Mat2f = Mat2<se::Float32>;
	using Mat2f64 = Mat2<se::Float64>;


	template <typename T>
	using Mat3 = Matn<T, 3>;

	using Mat3i = Mat3<se::Int32>;
	using Mat3f = Mat3<se::Float32>;
	using Mat3f64 = Mat3<se::Float64>;


	template <typename T>
	using Mat4 = Matn<T, 4>;

	using Mat4i = Mat4<se::Int32>;
	using Mat4f = Mat4<se::Float32>;
	using Mat4f64 = Mat4<se::Float64>;

} // namespace se



#include "matn.inl"