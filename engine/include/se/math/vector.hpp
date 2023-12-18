#pragma once

#include <array>
#include <ostream>
#include <type_traits>

#include "../types.hpp"



namespace se
{
	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	class Vector
	{
		public:
			Vector() = default;

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Vector(T2 fill);

			template <typename T2, se::Length D2>
			requires std::is_arithmetic_v<T2> && (D2 <= D)
			Vector(const std::array<T, D> &datas);

			template <typename T2, se::Length D2>
			requires std::is_arithmetic_v<T2> && (D2 <= D)
			Vector(const se::Vector<T2, D2> &vector);

			template <typename T2, typename ...Args>
			requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= (D - 1))
			Vector(T2 first, Args ...args);


			template <typename T2, se::Length D2>
			requires std::is_arithmetic_v<T2> && (D2 <= D)
			const se::Vector<T, D> &operator=(const se::Vector<T, D> &vector);

			inline T &operator[] (se::Length index);
			inline T operator[] (se::Length index) const;


			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Vector<T, D> &operator+=(const se::Vector<T2, D> &vector);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Vector<T, D> &operator-=(const se::Vector<T2, D> &vector);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Vector<T, D> &operator*=(const se::Vector<T2, D> &vector);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Vector<T, D> &operator*=(T2 scalar);

			T length() const noexcept;
			inline T length2() const noexcept;


			inline const std::array<T, D >= 4 ? D : 4> &getInternalArray() const noexcept;



		protected:
			std::array<T, D >= 4 ? D : 4> m_datas;
	};



	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	bool operator==(const se::Vector<T, D> &a, const se::Vector<T2, D> &b);


	template <typename T, typename T2, se::Length D>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	T dot(const se::Vector<T, D> &a, const se::Vector<T2, D> &b);


	template <typename T, se::Length D>
	requires std::is_arithmetic_v<T>
	std::ostream &operator<<(std::ostream &stream, const se::Vector<T, D> &vector);

	
} // namespace se



#include "vector.inl"