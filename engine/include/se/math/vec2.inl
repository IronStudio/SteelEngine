#include "vec2.hpp"



namespace se
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec2<T>::Vec2() :
		Vector<T, 2> (),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		u {this->m_datas[0]},
		v {this->m_datas[1]}
	{
		
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec2<T>::Vec2(T fill) :
		Vector<T, 2> (fill),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		u {this->m_datas[0]},
		v {this->m_datas[1]}
	{
		
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	Vec2<T>::Vec2(T _x, T _y) :
		Vector<T, 2> (_x, _y),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		u {this->m_datas[0]},
		v {this->m_datas[1]}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Vec2<T>::Vec2(const se::Vec2<T2> &vector) :
		Vector<T, 2> (vector),
		x {this->m_datas[0]},
		y {this->m_datas[1]},
		u {this->m_datas[0]},
		v {this->m_datas[1]}
	{

	}



} // namespace se
