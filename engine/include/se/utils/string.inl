#include "se/utils/string.hpp"

#include <cstdlib>
#include <cstring>

#include "se/assert.hpp"



namespace se {
	template <se::StringEncoding encoding>
	_String<encoding>::_String() noexcept :
		m_data {nullptr},
		m_size {0}
	{

	}


	template <se::StringEncoding encoding>
	_String<encoding>::_String(const _Char *const str) noexcept :
		m_data {nullptr},
		m_size {0}
	{
		SE_ASSERT(str != nullptr, "Can't create string from nullptr pointer string");

		for (const _Char *it {str}; *it != '\0'; ++it) ++m_size;
		se::ByteCount totalByteSize {sizeof(_Char) * (1 + m_size)};
		m_data = reinterpret_cast<_Char*> (malloc(totalByteSize));
		memcpy(m_data, str, totalByteSize);
	}


	template <se::StringEncoding encoding>
	_String<encoding>::_String(const _Char *const str, const se::Count size) noexcept :
		m_data {nullptr},
		m_size {size}
	{
		SE_ASSERT(str != nullptr, "Can't create string from nullptr pointer string");
		SE_ASSERT(size != 0, "Can't create string from null-sized pointer string");

		se::ByteCount totalByteSize {sizeof(_Char) * m_size};
		m_data = reinterpret_cast<_Char*> (malloc(totalByteSize + sizeof(_Char)));
		memcpy(m_data, str, totalByteSize);
		m_data[totalByteSize] = '\0';
	}


	template <se::StringEncoding encoding>
	_String<encoding>::~_String() {
		if (m_data != nullptr)
			free(m_data);
		m_data = nullptr;
		m_size = 0;
	}


	template <se::StringEncoding encoding>
	_String<encoding>::_String(const se::_String<encoding> &string) noexcept :
		m_data {nullptr},
		m_size {0}
	{
		*this = string;
	}


	template <se::StringEncoding encoding>
	const se::_String<encoding> &_String<encoding>::operator=(const se::_String<encoding> &string) noexcept {
		this->~_String();

		if (string.m_data == nullptr || string.m_size == 0)
			return *this;

		m_size = string.m_size;
		se::ByteCount totalByteSize {sizeof(_Char) * (m_size + 1)};
		m_data = reinterpret_cast<_Char*> (malloc(totalByteSize));
		memcpy(m_data, string.m_data, totalByteSize);

		return *this;
	}


	template <se::StringEncoding encoding>
	_String<encoding>::_String(se::_String<encoding> &&string) noexcept :
		m_data {string.m_data},
		m_size {string.m_size}
	{
		string.m_data = nullptr;
		string.m_size = 0;
	}


	template <se::StringEncoding encoding>
	const se::_String<encoding> &_String<encoding>::operator=(se::_String<encoding> &&string) noexcept {
		m_data = string.m_data;
		m_size = string.m_size;

		string.m_data = nullptr;
		string.m_size = 0;

		return *this;
	}


	template <se::StringEncoding encoding>
	se::_String<encoding> _String<encoding>::operator+(const se::_String<encoding> &string) {
		if (m_data == nullptr || m_size == 0) {
			if (string.m_data == nullptr || string.m_size == 0)
				return se::_String<encoding> ();
			return string;
		}

		if (string.m_data == nullptr || string.m_size == 0)
			return *this;

		se::Count totalSize {m_size + string.m_size};
		_Char *buffer {reinterpret_cast<_Char*> (malloc(totalSize * sizeof(_Char)))};
		memcpy(buffer, m_data, m_size * sizeof(_Char));
		memcpy(buffer + m_size * sizeof(_Char), string.m_data, string.m_size * sizeof(_Char));

		se::_String<encoding> output {buffer, totalSize};
		free(buffer);
		return output;
	}


	template <se::StringEncoding encoding>
	const se::_String<encoding> &_String<encoding>::operator+=(const se::_String<encoding> &string) {
		if (m_data == nullptr || m_size == 0) {
			*this = string;
			return *this;
		}

		if (string.m_data == nullptr || string.m_size == 0)
			return *this;

		m_data = reinterpret_cast<_Char*> (realloc(m_data, (m_size + string.m_size + 1) * sizeof(_Char)));
		memcpy(m_data + m_size * sizeof(_Char), string.m_data, (string.m_size + 1) * sizeof(_Char));
		return *this;
	}


	template <se::StringEncoding encoding>
	_String<encoding>::_Char &_String<encoding>::operator[] (se::Count index) const noexcept {
		SE_ASSERT(m_data != nullptr && m_size != 0, "Can't access element of empty string");
		SE_ASSERT(index < m_size, "Can't access char outside of the string (the null-terminated char is inaccessable through this operator)");

		return m_data[index];
	}


} // namespace se
