#include "se/utils/string.hpp"

#include <cstdlib>
#include <cstring>

#include "se/assert.hpp"
#include "se/utils/iterator.hpp"



namespace se {
	template <se::StringEncoding encoding>
	StringIterator<encoding>::StringIterator() noexcept :
		m_pointer {nullptr},
		m_linkedContainer {nullptr}
	{

	}


	template <se::StringEncoding encoding>
	StringIterator<encoding>::StringIterator(_Char *pointer, const se::_String<encoding> *linkedContainer) noexcept :
		m_pointer {pointer},
		m_linkedContainer {linkedContainer}
	{

	}


	template <se::StringEncoding encoding>
	StringIterator<encoding>::StringIterator(const se::StringIterator<encoding> &iterator) :
		m_pointer {iterator.m_pointer},
		m_linkedContainer {iterator.m_linkedContainer}
	{

	}


	template <se::StringEncoding encoding>
	const se::StringIterator<encoding> &StringIterator<encoding>::operator=(const se::StringIterator<encoding> &iterator) {
		m_pointer = iterator.m_pointer;
		m_linkedContainer = iterator.m_linkedContainer;
		return *this;
	}


	template <se::StringEncoding encoding>
	se::StringIterator<encoding> StringIterator<encoding>::next() const noexcept {
		auto output {*this};
		return ++output;
	}


	template <se::StringEncoding encoding>
	const se::StringIterator<encoding> &StringIterator<encoding>::operator++() noexcept {
		++m_pointer;
		if (m_pointer > m_linkedContainer->getIterableData() + m_linkedContainer->getSize())
			m_pointer = m_linkedContainer->getIterableData() + m_linkedContainer->getSize();
		return *this;
	}


	template <se::StringEncoding encoding>
	se::StringIterator<encoding> StringIterator<encoding>::operator++(int) noexcept {
		auto output {*this};
		++(*this);
		return output;
	}


	template <se::StringEncoding encoding>
	se::StringIterator<encoding> StringIterator<encoding>::previous() const noexcept {
		auto output {*this};
		return --output;
	}


	template <se::StringEncoding encoding>
	const se::StringIterator<encoding> &StringIterator<encoding>::operator--() noexcept {
		--m_pointer;
		if (m_pointer < m_linkedContainer->getIterableData() - 1)
			m_pointer = m_linkedContainer->getIterableData() - 1;
		return *this;
	}


	template <se::StringEncoding encoding>
	se::StringIterator<encoding> StringIterator<encoding>::operator--(int) noexcept {
		auto output {*this};
		--(*this);
		return output;
	}


	template <se::StringEncoding encoding>
	bool StringIterator<encoding>::operator==(const se::StringIterator<encoding> &iterator) const noexcept {
		return m_pointer == iterator.m_pointer && m_linkedContainer == iterator.m_linkedContainer;
	}


	template <se::StringEncoding encoding>
	typename se::EncodingChar<encoding>::Char &StringIterator<encoding>::operator*() const noexcept {
		SE_ASSERT(m_pointer != nullptr, "Can't dereference null string iterator");
		return *m_pointer;
	}


	template <se::StringEncoding encoding>
	typename se::EncodingChar<encoding>::Char *StringIterator<encoding>::operator->() const noexcept {
		SE_ASSERT(m_pointer != nullptr, "Can't dereference null string iterator");
		return m_pointer;
	}


	template <se::StringEncoding encoding>
	const se::StringIterator<encoding> &StringIterator<encoding>::operator+=(se::Count offset) noexcept {
		SE_ASSERT(m_pointer != nullptr, "Can't offset null iterator");
		SE_ASSERT(m_pointer < m_linkedContainer->getIterableData() + m_linkedContainer->getSize(), "Can't offset end iterator");
		m_pointer += offset;
		SE_ASSERT(m_pointer <= m_linkedContainer->getIterableData() + m_linkedContainer->getSize(), "Can't offset iterator outside of the string");
		return *this;
	}


	template <se::StringEncoding encoding>
	const se::StringIterator<encoding> &StringIterator<encoding>::operator-=(se::Count offset) noexcept {
		SE_ASSERT(m_pointer != nullptr, "Can't negative offset null iterator");
		SE_ASSERT(m_pointer > m_linkedContainer->getIterableData() - 1, "Can't negative offset rend iterator");
		m_pointer -= offset;
		SE_ASSERT(m_pointer >= m_linkedContainer->getIterableData() - 1, "Can't negative offset iterator outside of the string");
		return *this;
	}


	template <se::StringEncoding encoding>
	se::StringIterator<encoding> StringIterator<encoding>::operator+(se::Count offset) const noexcept {
		auto output {*this};
		output += offset;
		return output;
	}


	template <se::StringEncoding encoding>
	se::StringIterator<encoding> StringIterator<encoding>::operator-(se::Count offset) const noexcept {
		auto output {*this};
		output -= offset;
		return output;
	}


	template <se::StringEncoding encoding>
	se::Count StringIterator<encoding>::operator-(const se::StringIterator<encoding> &iterator) const noexcept {
		SE_ASSERT(m_linkedContainer == iterator.m_linkedContainer, "Can't calculate distance between iterator of different object");
		if (m_pointer > iterator.m_pointer)
			return m_pointer - iterator.m_pointer;
		return iterator.m_pointer - m_pointer;
	}




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
		static_assert(se::iterator::RandomAccessIterator<Iterator>, "String iterator must be a valid RandomAccessIterator");

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
		memcpy(buffer + m_size, string.m_data, string.m_size * sizeof(_Char));

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
		memcpy(m_data + m_size, string.m_data, (string.m_size + 1) * sizeof(_Char));
		m_size = m_size + string.m_size;
		return *this;
	}


	template <se::StringEncoding encoding>
	_String<encoding>::_Char &_String<encoding>::operator[] (se::Count index) const noexcept {
		SE_ASSERT(m_data != nullptr && m_size != 0, "Can't access element of empty string");
		SE_ASSERT(index < m_size, "Can't access char outside of the string (the null-terminated char is inaccessable through this operator)");

		return m_data[index];
	}


	template <se::StringEncoding encoding>
	_String<encoding>::Iterator _String<encoding>::begin() const noexcept {
		return Iterator(m_data, this);
	}


	template <se::StringEncoding encoding>
	_String<encoding>::Iterator _String<encoding>::end() const noexcept {
		return Iterator(m_data + m_size, this);
	}


	template <se::StringEncoding encoding>
	_String<encoding>::Iterator _String<encoding>::rbegin() const noexcept {
		return Iterator(m_data + m_size - 1, this);
	}


	template <se::StringEncoding encoding>
	_String<encoding>::Iterator _String<encoding>::rend() const noexcept {
		return Iterator(m_data - 1, this);
	}


} // namespace se
