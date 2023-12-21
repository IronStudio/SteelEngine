#include "operation.hpp"



namespace se
{
	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Vec4<T> operator*(se::Quaternion<T> quat, const se::Vec4<T2> &vector)
	{
		quat *= se::Quaternion<T> (vector);
		return se::Vec4<T> (quat[0], quat[1], quat[2], quat[3]);
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Vec4<T> operator*(const se::Vec4<T> &vector, const se::Quaternion<T2> &quat)
	{
		const se::Vec4<T> result = se::Quaternion<T> (vector) * quat;
		return se::Vec4<T> (result[0], result[1], result[2], result[3]);
	}



} // namespace se
