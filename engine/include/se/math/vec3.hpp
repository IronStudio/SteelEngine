#pragma once

#include "vector.hpp"



namespace se
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	class Vec3 : public Vector<T, 3>
	{
		public:
			Vec3();
			Vec3(T fill);
			Vec3(T x, T y, T z);
			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Vec3(const se::Vec3<T2> &vector);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Vec3<T> &operator=(const se::Vec3<T2> &vector);

			template <typename T2>
			requires std::is_arithmetic_v<T2>
			const se::Vec3<T> &operator=(const se::Vector<T2, 3> &vector);


			T &x, &y, &z;
			T &r, &g, &b;
	};


	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Vector<T, 3> cross(const se::Vector<T, 3> &a, const se::Vector<T2, 3> &b);


	using Vec3i = se::Vec3<se::Int32>;
	using Vec3f = se::Vec3<se::Float32>;
	using Vec3f64 = se::Vec3<se::Float64>;

} // namespace se



#include "vec3.inl"