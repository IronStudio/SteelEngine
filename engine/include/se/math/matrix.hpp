#pragma once

#include <array>
#include <ostream>
#include <span>
#include <type_traits>

#include "../types.hpp"
#include "vector.hpp"



namespace se
{
	/**
	 * @brief A class that represents a RxC matrix (R row and C column)
	 * @warning Matrix are stored in a row-major way
	*/
	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	class Matrix
	{
		public:
			Matrix();

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Matrix(T2 fill);

			/**
			 * @brief Copy `S` element of the given array into the beginning of the matrix, and fill the rest with 0
			 * @warning `datas` must be row-major
			*/
			template <typename T2, se::Length S>
			requires std::is_arithmetic_v<T2> && (S <= R * C)
			Matrix(const std::array<T2, S> &datas);

			template <typename T2, typename ...Args>
			requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= (R * C - 1))
			Matrix(T2 first, Args ...args);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Matrix(const se::Matrix<T2, R, C> &matrix);


			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Matrix<T, R, C> &operator=(const se::Matrix<T2, R, C> &matrix);


			/**
			 * @brief Access the `index`th row
			*/
			std::span<T, C> operator[] (se::Length index);
			/**
			 * @brief Access the `index`th row
			*/
			const se::Vector<T, C> operator[] (se::Length index) const;

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Matrix<T, R, C> &operator+=(const se::Matrix<T2, R, C> &matrix);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Matrix<T, R, C> &operator-=(const se::Matrix<T2, R, C> &matrix);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Matrix<T, R, C> &operator*=(T2 scalar);


			inline const std::array<T, R * C> &getInternalArray() const noexcept;


		protected:
			std::array<T, R * C> m_datas;
	};


	template <typename T, se::Length R, se::Length C>
	std::ostream &operator<<(std::ostream &stream, const se::Matrix<T, R, C> &matrix);

	
	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator+(se::Matrix<T, R, C> a, const se::Matrix<T2, R, C> &b);

	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator-(se::Matrix<T, R, C> a, const se::Matrix<T2, R, C> &b);

	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator*(se::Matrix<T, R, C> matrix, T2 scalar);


	template <typename T, typename T2, se::Length R, se::Length S, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Matrix<T, R, C> operator*(const se::Matrix<T, R, S> &a, const se::Matrix<T2, S, C> &b);

	template <typename T, typename T2, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	bool operator==(const se::Matrix<T, R, C> &a, const se::Matrix<T2, R, C> &b);


	template <typename T, se::Length R, se::Length C>
	requires std::is_arithmetic_v<T>
	se::Matrix<T, C, R> transpose(const se::Matrix<T, R, C> &matrix);


} // namespace se



#include "matrix.inl"