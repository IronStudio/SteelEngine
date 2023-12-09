#include <cstring>
#include <iostream>

#include "essentials/assertion.hpp"
#include "essentials/string.hpp"



namespace se
{
	se::Size Char<se::Charset::UTF8>::size {sizeof(char)};
	se::Size Char<se::Charset::UTF16>::size {sizeof(char16_t)};
	se::Size Char<se::Charset::UTF32>::size {sizeof(char32_t)};



	template <se::Charset charset>
	String<charset>::String() :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0},
		m_capacity {0}
	{

	}



	template <se::Charset charset>
	String<charset>::String(se::Char<charset>::Type character) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0},
		m_capacity {0}
	{
		SE_UNKNOWN_ASSERT(character != 0, "Can't create string from only '\0'");

		m_sizeInBytes = 2;
		m_capacity = 2;
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
		m_sizeInBytes {0},
		m_capacity {0}
	{
		SE_UNKNOWN_ASSERT(str != nullptr, "Can't create string from empty string");

		this->p_allocateMemoryForArray(str);
		this->p_copyRawArray(str);
	}



	template <se::Charset charset>
	String<charset>::String(const se::String<charset> &str) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0},
		m_capacity {0}
	{
		SE_UNKNOWN_ASSERT(!str.isEmpty(), "Can't create string from empty string");

		this->p_allocateMemoryForArray(str.m_data);
		this->p_copyRawArray(str.m_data);
	}



	template <se::Charset charset>
	String<charset>::String(se::String<charset> &&str) noexcept :
		m_data {str.m_data},
		m_length {str.m_length},
		m_sizeInBytes {str.m_sizeInBytes},
		m_capacity {str.m_capacity}
	{
		str.m_data = nullptr;
		str.m_length = 0;
		str.m_sizeInBytes = 0;
		str.m_capacity = 0;
	}



	template <se::Charset charset>
	String<charset>::String(const std::string &str) :
		m_data {nullptr},
		m_length {0},
		m_sizeInBytes {0},
		m_capacity {0}
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
		m_capacity = 2;
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
	const se::String<charset> &String<charset>::operator=(se::String<charset> &&str) noexcept
	{
		this->clear();
		m_data = str.m_data;
		m_length = str.m_length;
		m_sizeInBytes = str.m_sizeInBytes;
		m_capacity = str.m_capacity;
		str.m_data = nullptr;
		str.m_length = 0;
		str.m_sizeInBytes = 0;
		str.m_capacity = 0;
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

		if (m_capacity >= m_sizeInBytes + str.m_sizeInBytes)
		{
			memcpy(m_data + m_sizeInBytes, str.m_data, str.m_sizeInBytes);
			return *this;
		}

		typename se::Char<charset>::Type *tmp {static_cast<se::Char<charset>::Type*> (malloc(m_sizeInBytes))};
		SE_UNKNOWN_ASSERT(tmp != nullptr, "Can't allocate memory to concatenate string");

		memcpy(tmp, m_data, m_sizeInBytes);

		se::Size oldSizeInBytes = m_sizeInBytes;
		m_sizeInBytes += str.m_sizeInBytes - 1;
		m_capacity = m_sizeInBytes;
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

		for (se::Size i {0}; i < m_sizeInBytes; ++i)
		{
			if (m_data[i] != str.m_data[i])
				return false;
		}

		return true;
	}



	template <se::Charset charset>
	se::Char<charset>::Type &String<charset>::operator[](se::Size index)
	{
		SE_UNKNOWN_ASSERT(!this->isEmpty(), "Can't get character from empty string");
		SE_UNKNOWN_ASSERT(index < m_sizeInBytes, "Can't get a character at a position outside the string");

		return m_data[index];
	}



	template <se::Charset charset>
	se::Char<charset>::Type String<charset>::operator[](se::Size index) const
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
	void String<charset>::reserve(se::Size size)
	{
		SE_UNKNOWN_ASSERT(size >= m_sizeInBytes, "Can't reserve a space smaller than the current string memory footprint");

		auto tmp {static_cast<se::Char<charset>::Type*> (malloc(size * se::Char<charset>::size))};
		SE_UNKNOWN_ASSERT(tmp != nullptr, "String data wasn't allocated");

		for (se::Size i {0}; i < m_sizeInBytes; ++i)
			tmp[i] = m_data[i];

		free(m_data);
		m_data = tmp;
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
		se::Size loopIterationCount {m_length};
		for (se::Size i {0}; i < loopIterationCount; ++i)
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



	template <se::Charset charset>
	se::String<charset> intToString(se::Int64 number, se::Uint base)
	{
		bool isNegative {number < 0};
		if (isNegative)
			number *= -1;
			
		auto result {uintToString<charset> (static_cast<se::Uint64> (number), base)};

		if (isNegative)
			result = std::move(se::String<charset> ('-') + result);

		return result;
	}



	template <se::Charset charset>
	se::String<charset> uintToString(se::Uint64 number, se::Uint base)
	{
		SE_UNKNOWN_ASSERT(base <= 16, "Can't convert given number to a base bigger than 16");
		SE_UNKNOWN_ASSERT(base > 1, "Can't convert given number to a base lower or equal to 1");

		static const se::String<charset> baseNumbers {
			"0123456789ABCDEF"
		};

		se::String<charset> result {};

		if (number == 0)
			return se::String<charset> ("0");


		while (number != 0)
		{
			result = std::move(se::String<charset> (baseNumbers[number % base]) + result);
			number = (se::Uint64)(number / base);
		}

		return result;
	}



} // namespace se


template class se::String<se::Charset::UTF8>;
//template class se::String<se::Charset::UTF16>;
//template class se::String<se::Charset::UTF32>;

template se::String<se::Charset::UTF8> se::intToString<se::Charset::UTF8> (se::Int64 number, se::Uint base);
//template se::String<se::Charset::UTF16> se::intToString<se::Charset::UTF16> (se::Int64 number, se::Uint base);
//template se::String<se::Charset::UTF32> se::intToString<se::Charset::UTF32> (se::Int64 number, se::Uint base);
template se::String<se::Charset::UTF8> se::uintToString<se::Charset::UTF8> (se::Uint64 number, se::Uint base);
//template se::String<se::Charset::UTF16> se::uintToString<se::Charset::UTF16> (se::Uint64 number, se::Uint base);
//template se::String<se::Charset::UTF32> se::uintToString<se::Charset::UTF32> (se::Uint64 number, se::Uint base);