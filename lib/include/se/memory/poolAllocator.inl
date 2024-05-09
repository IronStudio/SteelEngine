#include "se/memory/poolAllocator.hpp"

#include <cstring>
#include <iomanip>
#include <sstream>

#include "se/assertion.hpp"
#include "se/exceptions.hpp"



namespace se::memory
{
	template <typename T>
	PoolHandle<T>::PoolHandle() :
		m_allocator {nullptr},
		m_index {0},
		m_count {0}
	{

	}



	template <typename T>
	PoolHandle<T>::PoolHandle(se::memory::PoolAllocator<T> *allocator, se::Count index, se::Count count) :
		m_allocator {allocator},
		m_index {index},
		m_count {count}
	{

	}



	template <typename T>
	PoolHandle<T>::PoolHandle(const se::memory::PoolHandle<T> &handle) :
		m_allocator {handle.m_allocator},
		m_index {handle.m_index},
		m_count {handle.m_count}
	{

	}



	template <typename T>
	const se::memory::PoolHandle<T> &PoolHandle<T>::operator=(const se::memory::PoolHandle<T> &handle) {
		m_allocator = handle.m_allocator;
		m_index = handle.m_index;
		m_count = handle.m_count;
		return *this;
	}



	template <typename T>
	T &PoolHandle<T>::operator*() const {
		SE_ASSERT(m_allocator != nullptr, "Can't dereference handle if not linked to allocator");
		return *m_allocator->m_getElementPointer(m_index);
	}



	template <typename T>
	T *PoolHandle<T>::operator->() const {
		SE_ASSERT(m_allocator != nullptr, "Can't dereference handle if not linked to allocator");
		return m_allocator->m_getElementPointer(m_index);
	}



	template <typename T>
	T &PoolHandle<T>::operator[](se::Size index) const {
		SE_ASSERT(index < m_count, "Can't access index out of range");
		return m_allocator->m_getElementPointer(m_index)[index];
	}



	template <typename T>
	bool PoolHandle<T>::operator==(const se::memory::PoolHandle<T> &handle) const {
		return m_allocator == handle.m_allocator && m_index == handle.m_index;
	}



	template <typename T>
	PoolHandle<T>::operator bool() const {
		return m_allocator != nullptr;
	}



	template <typename T>
	PoolHandle<T>::operator std::string() const {
		std::ostringstream stream {};
		stream << std::hex << std::setfill('0') << std::setw(16) << m_allocator->m_getElementPointer(m_index);
		std::string str {stream.str()};
		str.insert(str.begin() + 12, '-');
		str.insert(str.begin() + 8, '-');
		str.insert(str.begin() + 4, '-');
		return str;
	}



	template <typename T>
	PoolHandle<T>::operator T*() const {
		return m_allocator->m_getElementPointer(m_index);
	}



	template <typename T>
	se::Size PoolHandle<T>::getCount() const noexcept {
		return m_count;
	}



	template <typename T>
	std::ostream &operator<<(std::ostream &stream, const se::memory::PoolHandle<T> &handle) {
		stream << static_cast<std::string> (handle);
		return stream;
	}






	template <typename T>
	PoolAllocator<T>::PoolAllocator() :
		m_size {0},
		m_data {nullptr},
		m_usedSpace {nullptr}
	{

	}



	template <typename T>
	PoolAllocator<T>::PoolAllocator(se::Size size) :
		m_size {size},
		m_data {nullptr},
		m_usedSpace {nullptr}
	{
		m_data = reinterpret_cast<T*> (malloc(sizeof(T) * size));
		m_usedSpace = reinterpret_cast<bool*> (malloc(sizeof(bool) * size));
		memset(m_usedSpace, 0, sizeof(bool) * size);
	}



	template <typename T>
	PoolAllocator<T>::~PoolAllocator() {
		for (se::Size i {0}; i < m_size; ++i)
			m_usedSpace[i];

		if (m_data != nullptr) {
			::free(m_usedSpace);
			::free(m_data);
		}
	}



	template <typename T>
	PoolAllocator<T>::PoolAllocator(se::memory::PoolAllocator<T> &&allocator) noexcept :
		m_size {allocator.m_size},
		m_data {allocator.m_data},
		m_usedSpace {allocator.m_usedSpace}
	{
		allocator.m_size = 0;
		allocator.m_data = nullptr;
		allocator.m_usedSpace = nullptr;
	}



	template <typename T>
	const se::memory::PoolAllocator<T> &PoolAllocator<T>::operator=(se::memory::PoolAllocator<T> &&allocator) noexcept {
		m_size = allocator.m_size;
		m_data = allocator.m_data;
		m_usedSpace = allocator.m_usedSpace;
		allocator.m_size = 0;
		allocator.m_data = nullptr;
		allocator.m_usedSpace = nullptr;
		return *this;
	}



	template <typename T>
	template <bool throwOnAllocationFailure>
	se::memory::PoolAllocator<T>::Handle PoolAllocator<T>::allocate(se::Size count) {
		for (se::Size i {0}; i < m_size; ++i) {
			if (m_usedSpace[i])
				continue;

			if (m_size - i < count)
				break;

			bool isSpaceBigEnough {true};
			for (se::Size j {i + 1}; j < i + count; ++j) {
				if (!m_usedSpace[j])
					continue;

				isSpaceBigEnough = false;
			}

			if (!isSpaceBigEnough)
				continue;

			for (se::Size j {i}; j < i + count; ++j)
				m_usedSpace[j] = true;

			se::memory::PoolAllocator<T>::Handle handle {m_data + i, count};
			return handle;
		}

		if constexpr (throwOnAllocationFailure)
			throw se::exceptions::BadAllocation("Can't allocate a spot of the wanted size in PoolAllocator");

		return {};
	}



	template <typename T>
	void PoolAllocator<T>::free(const se::memory::PoolAllocator<T>::Handle &handle) {
		se::Size startPosition {static_cast<se::Size> (handle.m_ptr - m_data)};
		for (se::Size i {startPosition}; i < startPosition + handle.m_count; ++i)
			m_usedSpace[i] = false;
	}



	template <typename T>
	template <bool throwOnAllocationFailure>
	se::memory::PoolAllocator<T>::Handle PoolAllocator<T>::reallocate(
		const se::memory::PoolAllocator<T>::Handle &handle, se::Size count
	) {
		this->free(handle);
		se::memory::PoolAllocator<T>::Handle output {this->allocate<throwOnAllocationFailure> (handle.m_count + count)};
		for (se::Size i {0}; i < handle.m_count; ++i)
			output[i] = handle[i];
		return output;
	}



	template <typename T>
	void PoolAllocator<T>::resize(se::Size size) {
		T *newData {reinterpret_cast<T*> (malloc(sizeof(T) * size))};
		memcpy(newData, m_data, std::min(size, m_size));
		free(m_data);
		m_data = newData;
		m_size = size;
	}



	template <typename T>
	T *PoolAllocator<T>::m_getElementPointer(se::Count index) {
		return m_data + index;
	}



} // namespace se::memory
