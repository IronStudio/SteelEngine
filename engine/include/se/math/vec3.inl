#include "vec3.hpp"



namespace se
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec3<T>::Vec3() :
		Vector<T, 3> (),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]}
	{
		
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec3<T>::Vec3(T fill) :
		Vector<T, 3> (fill),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]}
	{
		
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec3<T>::Vec3(T _x, T _y, T _z) :
		Vector<T, 3> (_x, _y, _z),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Vec3<T>::Vec3(const se::Vec3<T2> &vector) :
		Vector<T, 3> (vector),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Vec3<T> &Vec3<T>::operator=(const se::Vec3<T2> &vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;

		return *this;
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Vec3<T> &Vec3<T>::operator=(const se::Vector<T2, 3> &vector)
	{
		x = vector[0];
		y = vector[1];
		z = vector[2];

		return *this;
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Vector<T, 3> cross(const se::Vector<T, 3> &a, const se::Vector<T2, 3> &b)
	{
		return se::Vector<T, 3> (
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		);
	}



} // namespace se
