#include "string.hpp"



namespace se
{
	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator+=(const se::Char<charset>::Type *str)
	{
		return *this += se::String<charset> (str);
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator+=(const std::string &str)
	{
		return *this += se::String<charset> (str);
	}



	template <se::Charset charset>
	bool String<charset>::operator==(const se::Char<charset>::Type *str) const noexcept
	{
		return *this == se::String<charset> (str);
	}



	template <se::Charset charset>
	bool String<charset>::operator==(const std::string &str) const noexcept
	{
		return *this == se::String<charset> (str);
	}



	template <se::Charset charset>
	bool String<charset>::isEmpty() const noexcept
	{
		return m_length == 0 || m_data == nullptr || m_sizeInBytes == 0;
	}



	template <se::Charset charset>
	const se::Char<charset>::Type *String<charset>::getCString() const noexcept
	{
		return m_data;
	}



	template <se::Charset charset>
	std::string String<charset>::getCppString() const noexcept
	{
		return std::string(m_data);
	}



	template <se::Charset charset>
	size_t String<charset>::getLength() const noexcept
	{
		return m_length;
	}



	template <se::Charset charset>
	size_t String<charset>::getSizeInBytes() const noexcept
	{
		return m_sizeInBytes;
	}



	template <se::Charset charset>
	const se::String<charset> &operator+(se::String<charset> a, const typename se::Char<charset>::Type *b)
	{
		return a += b;
	}



	template <se::Charset charset>
	const se::String<charset> &operator+(se::String<charset> a, const se::String<charset> &b)
	{
		return a += b;
	}



	template <se::Charset charset>
	const se::String<charset> &operator+(se::String<charset> a, const std::string &b)
	{
		return a += b;
	}



	template <se::Charset charset>
	std::ostream &operator<<(std::ostream &stream, const se::String<charset> &str) noexcept
	{
		if (str.isEmpty())
			return stream;
		return stream << str.getCString();
	}



} // namespace se
