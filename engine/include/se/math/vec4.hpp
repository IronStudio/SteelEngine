#pragma once

#include "vector.hpp"



namespace se
{
	/**
	 * @brief A class that provide access to `Vector<T, 4>` components with references
	*/
	template <typename T>
	requires std::is_arithmetic_v<T>
	class Vec4 : public Vector<T, 4>
	{
		public:
			Vec4();
			Vec4(T fill);
			Vec4(T x, T y, T z, T w);
			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Vec4(const se::Vec4<T2> &vector);


			T &x, &y, &z, &w;
			T &r, &g, &b, &a;
	};


	using Vec4i = se::Vec4<se::Int32>;
	using Vec4f = se::Vec4<se::Float32>;
	using Vec4f64 = se::Vec4<se::Float64>;

} // namespace se



#include "vec4.inl"