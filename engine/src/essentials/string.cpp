#include <cstring>
#include <iostream>

#include "essentials/assertion.hpp"
#include "essentials/string.hpp"



namespace se
{
	size_t Char<se::Charset::UTF8>::size {sizeof(char)};
	size_t Char<se::Charset::UTF16>::size {sizeof(char)};
	size_t Char<se::Charset::UTF32>::size {sizeof(char)};



	template <se::Charset charset>
	String<charset>::String() :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0}
	{

	}



	template <se::Charset charset>
	String<charset>::String(se::Char<charset>::Type character) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0}
	{
		SE_UNKNOWN_ASSERT(character != 0, "Can't create string from only '\0'");

		m_sizeInBytes = 2;
		m_length = 1;
		m_data = static_cast<se::Char<charset>::Type*> (malloc(m_sizeInBytes));
		SE_UNKNOWN_ASSERT(m_data != nullptr, "String data wasn't allocated");
		m_data[0] = character;
		m_data[1] = 0;
	}



	template <se::Charset charset>
	String<charset>::String(const se::Char<charset>::Type *str) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0}
	{
		SE_UNKNOWN_ASSERT(str != nullptr, "Can't create string from empty string");

		this->p_allocateMemoryForArray(str);
		this->p_copyRawArray(str);
	}



	template <se::Charset charset>
	String<charset>::String(const se::String<charset> &str) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0}
	{
		SE_UNKNOWN_ASSERT(!str.isEmpty(), "Can't create string from empty string");

		this->p_allocateMemoryForArray(str.m_data);
		this->p_copyRawArray(str.m_data);
	}



	template <se::Charset charset>
	String<charset>::String(se::String<charset> &&str) :
		m_data {str.m_data},
		m_length {str.m_length},
		m_sizeInBytes {str.m_sizeInBytes}
	{
		SE_UNKNOWN_ASSERT(!str.isEmpty(), "Can't move empty string");

		str.m_data = nullptr;
		str.m_length = 0;
		str.m_sizeInBytes = 0;
	}



	template <se::Charset charset>
	String<charset>::String(const std::string &str) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0}
	{
		SE_UNKNOWN_ASSERT(!str.empty(), "Can't create string from empty string");

		const char *data {str.c_str()};
		this->p_allocateMemoryForArray(data);
		this->p_copyRawArray(data);
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator=(se::Char<charset>::Type character)
	{
		this->clear();
		SE_UNKNOWN_ASSERT(character != 0, "Can't create string from only '\0'");

		m_sizeInBytes = 2;
		m_length = 1;
		m_data = static_cast<se::Char<charset>::Type*> (malloc(m_sizeInBytes));
		SE_UNKNOWN_ASSERT(m_data != nullptr, "String data wasn't allocated");
		m_data[0] = character;
		m_data[1] = 0;

		return *this;
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator=(const se::Char<charset>::Type *str)
	{
		SE_UNKNOWN_ASSERT(str != nullptr, "Can't create string from empty string");

		this->clear();
		this->p_allocateMemoryForArray(str);
		this->p_copyRawArray(str);
		return *this;
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator=(const se::String<charset> &str)
	{
		SE_UNKNOWN_ASSERT(!str.isEmpty(), "Can't create string from empty string");

		this->clear();
		this->p_allocateMemoryForArray(str.m_data);
		this->p_copyRawArray(str.m_data);
		return *this;
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator=(se::String<charset> &&str)
	{
		SE_UNKNOWN_ASSERT(!str.isEmpty(), "Can't move empty string");

		this->clear();
		m_data = str.m_data;
		m_length = str.m_length;
		m_sizeInBytes = str.m_sizeInBytes;
		str.m_data = nullptr;
		str.m_length = 0;
		str.m_sizeInBytes = 0;
		return *this;
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator=(const std::string &str)
	{
		SE_UNKNOWN_ASSERT(!str.empty(), "Can't create string from empty string");

		this->clear();
		const char *data {str.c_str()};
		this->p_allocateMemoryForArray(data);
		this->p_copyRawArray(data);
		return *this;
	}



	template <se::Charset charset>
	String<charset>::~String()
	{
		this->clear();
	}



	template <se::Charset charset>
	const se::String<charset> &String<charset>::operator+=(const se::String<charset> &str)
	{
		if (str.isEmpty())
			return *this;

		if (this->isEmpty())
			return *this = str;

		typename se::Char<charset>::Type *tmp {static_cast<se::Char<charset>::Type*> (malloc(m_sizeInBytes))};
		SE_UNKNOWN_ASSERT(tmp != nullptr, "Can't allocate memory to concatenate string");

		memcpy(tmp, m_data, m_sizeInBytes);

		size_t oldSizeInBytes = m_sizeInBytes;
		m_sizeInBytes += str.m_sizeInBytes - 1;
		m_length += str.m_length;

		free(m_data);

		m_data = static_cast<se::Char<charset>::Type*> (malloc(m_sizeInBytes));
		SE_UNKNOWN_ASSERT(m_data != nullptr, "Can't allocate memory to concatenate string");
		memcpy(m_data, tmp, oldSizeInBytes - 1);
		memcpy(m_data + oldSizeInBytes - 1, str.m_data, str.m_sizeInBytes);

		free(tmp);

		return *this;
	}



	template <se::Charset charset>
	bool String<charset>::operator==(const se::String<charset> &str) const noexcept
	{
		if (m_length != str.m_length)
			return false;

		if (m_sizeInBytes != str.m_sizeInBytes)
			return false;

		for (size_t i {0}; i < m_sizeInBytes; ++i)
		{
			if (m_data[i] != str.m_data[i])
				return false;
		}

		return true;
	}



	template <se::Charset charset>
	se::Char<charset>::Type &String<charset>::operator[](size_t index)
	{
		SE_UNKNOWN_ASSERT(!this->isEmpty(), "Can't get character from empty string");
		SE_UNKNOWN_ASSERT(index < m_sizeInBytes, "Can't get a character at a position outside the string");

		return m_data[index];
	}



	template <se::Charset charset>
	se::Char<charset>::Type String<charset>::operator[](size_t index) const
	{
		SE_UNKNOWN_ASSERT(!this->isEmpty(), "Can't get character from empty string");
		SE_UNKNOWN_ASSERT(index < m_sizeInBytes, "Can't get a character at a position outside the string");

		return m_data[index];
	}



	template <se::Charset charset>
	void String<charset>::clear()
	{
		if (m_data != nullptr)
			free(m_data);

		m_length = 0;
		m_sizeInBytes = 0;
	}



	template <se::Charset charset>
	void String<charset>::p_allocateMemoryForArray(const se::Char<charset>::Type *str)
	{
		for (const typename se::Char<charset>::Type *currentChar {str}; *currentChar != 0; ++currentChar)
			++m_length;

		SE_UNKNOWN_ASSERT(m_length != 0, "Can't allocate memory for empty string");

		m_sizeInBytes = (m_length + 1) * se::Char<charset>::size;
		m_data = static_cast<se::Char<charset>::Type*> (malloc(m_sizeInBytes));
		SE_UNKNOWN_ASSERT(m_data != nullptr, "String data wasn't allocated");

		m_data[m_length] = 0;
	}



	template <>
	void String<se::Charset::UTF8>::p_copyRawArray(const se::Char<se::Charset::UTF8>::Type *str)
	{
		size_t loopIterationCount {m_length};
		for (size_t i {0}; i < loopIterationCount; ++i)
		{
			m_data[i] = str[i];
			if (!(m_data[i] & 0b10000000))
				continue;

			if (!(m_data[i] & 0b1000000))
				continue;

			if (m_data[i] & 0b100000)
				m_length -= 2;

			else if (m_data[i] & 0b10000)
				m_length -= 3;

			else
				--m_length;
		}
	}



} // namespace se


template class se::String<se::Charset::UTF8>;
//template class se::String<se::Charset::UTF16>;
//template class se::String<se::Charset::UTF32>;