#pragma once

#include "../types.hpp"
#include "vec3.hpp"
#include "vec4.hpp"



namespace se
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	class Quaternion
	{
		public:
			Quaternion();

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Quaternion(T2 fill);

			template <typename T2, typename ...Args>
			requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= 3)
			Quaternion(T2 first, Args ...args);

			template <typename T2, typename T3>
			requires std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>
			Quaternion(const se::Vector<T2, 3> &vector, T3 real);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Quaternion(const se::Vec4<T2> &vector);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Quaternion(const se::Quaternion<T2> &quaternion);


			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Quaternion<T> &operator=(const se::Quaternion<T2> &quaternion);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Quaternion<T> &operator*=(const se::Quaternion<T2> &quaternion);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Quaternion<T> &operator*=(T2 scalar);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Quaternion<T> &operator+=(const se::Quaternion<T2> &quaternion);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Quaternion<T> &operator-=(const se::Quaternion<T2> &quaternion);


			inline T &operator[] (se::Length index);
			inline T operator[] (se::Length index) const;

			T length() const;
			T length2() const;
			void normalize();

			inline se::Vec3<T> getVectorPart() const noexcept;
			inline T getScalarPart() const noexcept;
			inline const std::array<T, 4> &getInternalDatas() const noexcept;


		private:
			std::array<T, 4> m_datas;
	};


	template <typename T>
	requires std::is_arithmetic_v<T>
	std::ostream &operator<<(std::ostream &stream, const se::Quaternion<T> &quaternion);

	template <typename T>
	requires std::is_arithmetic_v<T>
	se::Quaternion<T> conjugate(const se::Quaternion<T> &quaternion);

	template <typename T>
	requires std::is_arithmetic_v<T>
	se::Quaternion<T> inverse(const se::Quaternion<T> &quaternion);


	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Quaternion<T> operator*(se::Quaternion<T> a, const se::Quaternion<T2> &b);

	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Quaternion<T> operator*(se::Quaternion<T> quaternion, T2 scalar);

	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Quaternion<T> operator*(T2 scalar, se::Quaternion<T> quaternion);

	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Quaternion<T> operator+(se::Quaternion<T> a, const se::Quaternion<T2> &b);

	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	inline se::Quaternion<T> operator-(se::Quaternion<T> a, const se::Quaternion<T2> &b);



} // namespace se



#include "quaternion.inl"