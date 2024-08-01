#include "se/utils/string.hpp"

#include <cstdlib>
#include <cstring>

#include "se/assert.hpp"



namespace se {
	template <se::StringEncoding encoding>
	String<encoding>::String() noexcept :
		m_data {nullptr},
		m_size {0}
	{

	}


	template <se::StringEncoding encoding>
	String<encoding>::String(const _Char *const str) noexcept :
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
	String<encoding>::String(const _Char *const str, const se::Count size) noexcept :
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
	String<encoding>::~String() {
		if (m_data != nullptr)
			free(m_data);
		m_data = nullptr;
		m_size = 0;
	}


	template <se::StringEncoding encoding>
	String<encoding>::String(const se::String<encoding> &string) noexcept :
		m_data {nullptr},
		m_size {0}
	{
		*this = string;
	}


	template <se::StringEncoding encoding>
	const se::String<encoding> &String<encoding>::operator=(const se::String<encoding> &string) noexcept {
		this->~String();

		if (string.m_data == nullptr || string.m_size == 0)
			return *this;

		m_size = string.m_size;
		se::ByteCount totalByteSize {sizeof(_Char) * (m_size + 1)};
		m_data = reinterpret_cast<_Char*> (malloc(totalByteSize));
		memcpy(m_data, string.m_data, totalByteSize);

		return *this;
	}


	template <se::StringEncoding encoding>
	String<encoding>::String(se::String<encoding> &&string) noexcept :
		m_data {string.m_data},
		m_size {string.m_size}
	{
		string.m_data = nullptr;
		string.m_size = 0;
	}


	template <se::StringEncoding encoding>
	const se::String<encoding> &String<encoding>::operator=(se::String<encoding> &&string) noexcept {
		m_data = string.m_data;
		m_size = string.m_size;

		string.m_data = nullptr;
		string.m_size = 0;

		return *this;
	}


	template <se::StringEncoding encoding>
	se::String<encoding> String<encoding>::operator+(const se::String<encoding> &string) {
		if (m_data == nullptr || m_size == 0) {
			if (string.m_data == nullptr || string.m_size == 0)
				return se::String<encoding> ();
			return string;
		}

		if (string.m_data == nullptr || string.m_size == 0)
			return *this;

		se::Count totalSize {m_size + string.m_size};
		_Char *buffer {reinterpret_cast<_Char*> (malloc(totalSize * sizeof(_Char)))};
		memcpy(buffer, m_data, m_size * sizeof(_Char));
		memcpy(buffer + m_size * sizeof(_Char), string.m_data, string.m_size * sizeof(_Char));

		se::String<encoding> output {buffer, totalSize};
		free(buffer);
		return output;
	}


	template <se::StringEncoding encoding>
	const se::String<encoding> &String<encoding>::operator+=(const se::String<encoding> &string) {
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


} // namespace se
