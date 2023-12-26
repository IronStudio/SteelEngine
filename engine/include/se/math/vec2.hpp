#pragma once

#include "vector.hpp"



namespace se
{
	/**
	 * @brief A class that provide access to `Vector<T, 2>` components with references
	*/
	template <typename T>
	requires std::is_arithmetic_v<T>
	class Vec2 : public Vector<T, 2>
	{
		public:
			Vec2();
			Vec2(T fill);
			Vec2(T x, T y);
			template <typename T2>
			requires std::is_arithmetic_v<T2>
			Vec2(const se::Vec2<T2> &vector);


			T &x, &y;
			T &u, &v;
	};


	using Vec2i = se::Vec2<se::Int32>;
	using Vec2f = se::Vec2<se::Float32>;
	using Vec2f64 = se::Vec2<se::Float64>;

} // namespace se



#include "vec2.inl"