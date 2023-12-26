#pragma once

#include "matn.hpp"
#include "vec4.hpp"
#include "quaternion.hpp"



namespace se
{
	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Vec4<T> operator*(se::Quaternion<T> quat, const se::Vec4<T2> &vector);

	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Vec4<T> operator*(const se::Vec4<T> &vector, const se::Quaternion<T2> &quat);


	
} // namespace se
