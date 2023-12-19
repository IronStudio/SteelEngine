#include "quaternion.hpp"

#include <smmintrin.h>



namespace se
{
	template <typename T>
	requires std::is_arithmetic_v<T>
	Quaternion<T>::Quaternion() :
		m_datas {}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Quaternion<T>::Quaternion(T2 fill) :
		m_datas {static_cast<T> (fill), static_cast<T> (fill), static_cast<T> (fill), static_cast<T> (fill)}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2, typename ...Args>
	requires std::is_arithmetic_v<T2> && (std::is_arithmetic_v<Args> && ...) && (sizeof...(Args) <= 3)
	Quaternion<T>::Quaternion(T2 first, Args ...args) :
		m_datas {static_cast<T> (first), static_cast<T> (args)...}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2, typename T3>
	requires std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>
	Quaternion<T>::Quaternion(const se::Vector<T2, 3> &vector, T3 real) :
		m_datas {static_cast<T> (vector[0]), static_cast<T> (vector[1]), static_cast<T> (vector[2]), static_cast<T> (real)}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Quaternion<T>::Quaternion(const se::Vec4<T2> &vector) :
		m_datas {static_cast<T> (vector[0]), static_cast<T> (vector[1]), static_cast<T> (vector[2]), static_cast<T> (vector[3])}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	Quaternion<T>::Quaternion(const se::Quaternion<T2> &quaternion) :
		m_datas {static_cast<T> (quaternion[0]), static_cast<T> (quaternion[1]),
			static_cast<T> (quaternion[2]), static_cast<T> (quaternion[3])}
	{

	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Quaternion<T> &Quaternion<T>::operator=(const se::Quaternion<T2> &quaternion)
	{
		m_datas[0] = static_cast<T> (quaternion[0]);
		m_datas[1] = static_cast<T> (quaternion[1]);
		m_datas[2] = static_cast<T> (quaternion[2]);
		m_datas[3] = static_cast<T> (quaternion[3]);
		return *this;
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Quaternion<T> &Quaternion<T>::operator*=(const se::Quaternion<T2> &quaternion)
	{
		auto thisVec3 {this->getVectorPart()};
		auto quatVec3 {quaternion.getVectorPart()};

		thisVec3 = quaternion.getScalarPart() * thisVec3 + this->getScalarPart() * quatVec3
			+ se::cross(this->getVectorPart(), quaternion.getVectorPart());
		
		m_datas[0] = thisVec3.x;
		m_datas[1] = thisVec3.y;
		m_datas[2] = thisVec3.z;
		m_datas[3] = this->getScalarPart() * quaternion.getScalarPart() - se::dot(thisVec3, quatVec3);

		return *this;
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Quaternion<T> &Quaternion<T>::operator*=(T2 scalar)
	{
		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2>)
		{
			__m128 dst {_mm_load_ps(m_datas.data())};
			__m128 src {_mm_set1_ps(scalar)};
			dst = _mm_mul_ps(dst, src);
			_mm_store_ps(m_datas.data(), dst);
			return *this;
		}

		else
		{
			m_datas[0] *= scalar;
			m_datas[1] *= scalar;
			m_datas[2] *= scalar;
			m_datas[3] *= scalar;
			return *this;
		}
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Quaternion<T> &Quaternion<T>::operator+=(const se::Quaternion<T2> &quaternion)
	{
		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2>)
		{
			__m128 dst {_mm_load_ps(m_datas.data())};
			__m128 src {_mm_load_ps(quaternion.getInternalDatas().data())};
			dst = _mm_add_ps(dst, src);
			_mm_store_ps(m_datas.data(), dst);
			return *this;
		}

		else
		{
			for (se::Length i {0}; i < 4; ++i)
				m_datas[i] += quaternion.getInternalDatas()[i];
		}
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	template <typename T2>
	requires std::is_arithmetic_v<T2>
	const se::Quaternion<T> &Quaternion<T>::operator-=(const se::Quaternion<T2> &quaternion)
	{
		if constexpr (std::is_same_v<T, se::Float32> && std::is_same_v<T, T2>)
		{
			__m128 dst {_mm_load_ps(m_datas.data())};
			__m128 src {_mm_load_ps(quaternion.getInternalDatas().data())};
			dst = _mm_sub_ps(dst, src);
			_mm_store_ps(m_datas.data(), dst);
			return *this;
		}

		else
		{
			for (se::Length i {0}; i < 4; ++i)
				m_datas[i] -= quaternion.getInternalDatas()[i];
		}
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	T &Quaternion<T>::operator[] (se::Length index)
	{
		return m_datas[index];
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	T Quaternion<T>::operator[] (se::Length index) const
	{
		return m_datas[index];
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	T Quaternion<T>::length() const
	{
		if constexpr (std::is_same_v<T, se::Float32>)
		{
			__m128 dst {_mm_load_ps(m_datas.data())};
			dst = _mm_dp_ps(dst, dst, 0b11111111);
			dst = _mm_sqrt_ps(dst);
			T output[4] {};
			_mm_store_ps(output, dst);
			return output[0];
		}

		else
			return sqrt(this->length2());
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	T Quaternion<T>::length2() const
	{
		if constexpr (std::is_same_v<T, se::Float32>)
		{
			__m128 dst {_mm_load_ps(m_datas.data())};
			dst = _mm_dp_ps(dst, dst, 0b11111111);
			T output[4] {};
			_mm_store_ps(output, dst);
			return output[0];
		}

		else
			return m_datas[0] * m_datas[0] + m_datas[1] * m_datas[1] + m_datas[2] * m_datas[2] + m_datas[3] * m_datas[3];
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	void Quaternion<T>::normalize()
	{
		*this *= (static_cast<T> (1) / this->length());
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	se::Vec3<T> Quaternion<T>::getVectorPart() const noexcept
	{
		return se::Vec3<T> (m_datas[0], m_datas[1], m_datas[2]);
	}


	
	template <typename T>
	requires std::is_arithmetic_v<T>
	T Quaternion<T>::getScalarPart() const noexcept
	{
		return m_datas[3];
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	const std::array<T, 4> &Quaternion<T>::getInternalDatas() const noexcept
	{
		return m_datas;
	}


	
	template <typename T>
	requires std::is_arithmetic_v<T>
	std::ostream &operator<<(std::ostream &stream, const se::Quaternion<T> &quaternion)
	{
		stream << "(" << quaternion[0] << ", " << quaternion[1] << ", " << quaternion[2] << ", " << quaternion[3] << ")";
		return stream;
	}
	


	template <typename T>
	requires std::is_arithmetic_v<T>
	se::Quaternion<T> conjugate(const se::Quaternion<T> &quaternion)
	{
		return se::Quaternion<T> (quaternion.getVectorPart() * (-1), quaternion.getScalarPart());
	}



	template <typename T>
	requires std::is_arithmetic_v<T>
	se::Quaternion<T> inverse(const se::Quaternion<T> &quaternion)
	{
		return se::conjugate(quaternion) / quaternion.length2();
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Quaternion<T> operator*(se::Quaternion<T> a, const se::Quaternion<T2> &b)
	{
		return a *= b;
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Quaternion<T> operator*(se::Quaternion<T> quaternion, T2 scalar)
	{
		return quaternion *= scalar;
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Quaternion<T> operator*(T2 scalar, se::Quaternion<T> quaternion)
	{
		return quaternion *= scalar;
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Quaternion<T> operator+(se::Quaternion<T> a, const se::Quaternion<T2> &b)
	{
		return a += b;
	}



	template <typename T, typename T2>
	requires std::is_arithmetic_v<T> && std::is_arithmetic_v<T2>
	se::Quaternion<T> operator-(se::Quaternion<T> a, const se::Quaternion<T2> &b)
	{
		return a -= b;
	}



} // namespace se
