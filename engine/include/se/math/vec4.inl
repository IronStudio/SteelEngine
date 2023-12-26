#include "vec4.hpp"



namespace se
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec4<T>::Vec4() :
		Vector<T, 4> (),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		w {this->m_datas[3]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]},
		a {this->m_datas[3]}
	{
		
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec4<T>::Vec4(T fill) :
		Vector<T, 4> (fill),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		w {this->m_datas[3]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]},
		a {this->m_datas[3]}
	{
		
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec4<T>::Vec4(T _x, T _y, T _z, T _w) :
		Vector<T, 4> (_x, _y, _z, _w),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		w {this->m_datas[3]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]},
		a {this->m_datas[3]}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Vec4<T>::Vec4(const se::Vec4<T2> &vector) :
		Vector<T, 4> (vector),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		z {this->m_datas[2]},
		w {this->m_datas[3]},
		r {this->m_datas[0]},
		g {this->m_datas[1]},
		b {this->m_datas[2]},
		a {this->m_datas[3]}
	{

	}



} // namespace se
