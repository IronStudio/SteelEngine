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



} // namespace se
