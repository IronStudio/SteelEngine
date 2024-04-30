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
		m_ptr {nullptr},
		m_count {0}
	{

	}



	template <typename T>
	PoolHandle<T>::PoolHandle(T *ptr, se::Size count) :
		m_ptr {ptr},
		m_count {count}
	{

	}



	template <typename T>
	PoolHandle<T>::PoolHandle(const se::memory::PoolHandle<T> &handle) :
		m_ptr {handle.m_ptr},
		m_count {handle.m_count}
	{

	}



	template <typename T>
	const se::memory::PoolHandle<T> &PoolHandle<T>::operator=(const se::memory::PoolHandle<T> &handle) {
		m_ptr = handle.m_ptr;
		m_count = handle.m_count;
		return *this;
	}



	template <typename T>
	T &PoolHandle<T>::operator*() const {
		SE_ASSERT(m_ptr != nullptr, "Can't dereference handle if set to nullptr");
		return *m_ptr;
	}



	template <typename T>
	T *PoolHandle<T>::operator->() const {
		SE_ASSERT(m_ptr != nullptr, "Can't dereference handle if set to nullptr");
		return m_ptr;
	}



	template <typename T>
	T &PoolHandle<T>::operator[](se::Size index) const {
		SE_ASSERT(index < m_count, "Can't access index out of range");
		return m_ptr[index];
	}



	template <typename T>
	bool PoolHandle<T>::operator==(const se::memory::PoolHandle<T> &handle) const {
		return m_ptr == handle.m_ptr;
	}



	template <typename T>
	PoolHandle<T>::operator bool() const {
		return m_ptr != nullptr;
	}



	template <typename T>
	PoolHandle<T>::operator std::string() const {
		std::ostringstream stream {};
		stream << std::hex << std::setfill('0') << std::setw(16) << m_ptr;
		std::string str {stream.str()};
		str.insert(str.begin() + 12, '-');
		str.insert(str.begin() + 8, '-');
		str.insert(str.begin() + 4, '-');
		return str;
	}



	template <typename T>
	PoolHandle<T>::operator T*() const {
		return m_ptr;
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






	template <typename T, bool A>
	PoolAllocator<T, A>::PoolAllocator() :
		m_size {0},
		m_data {nullptr},
		m_usedSpace {nullptr}
	{

	}



	template <typename T, bool A>
	PoolAllocator<T, A>::PoolAllocator(se::Size size) :
		m_size {size},
		m_data {nullptr},
		m_usedSpace {nullptr}
	{
		m_data = reinterpret_cast<T*> (malloc(sizeof(T) * size));
		m_usedSpace = reinterpret_cast<bool*> (malloc(sizeof(bool) * size));
		memset(m_usedSpace, 0, sizeof(bool) * size);
	}



	template <typename T, bool A>
	PoolAllocator<T, A>::~PoolAllocator() {
		for (se::Size i {0}; i < m_size; ++i)
			m_usedSpace[i];

		if (m_data != nullptr) {
			::free(m_usedSpace);
			::free(m_data);
		}
	}



	template <typename T, bool A>
	PoolAllocator<T, A>::PoolAllocator(se::memory::PoolAllocator<T, A> &&allocator) noexcept :
		m_size {allocator.m_size},
		m_data {allocator.m_data},
		m_usedSpace {allocator.m_usedSpace}
	{
		allocator.m_size = 0;
		allocator.m_data = nullptr;
		allocator.m_usedSpace = nullptr;
	}



	template <typename T, bool A>
	const se::memory::PoolAllocator<T, A> &PoolAllocator<T, A>::operator=(se::memory::PoolAllocator<T, A> &&allocator) noexcept {
		m_size = allocator.m_size;
		m_data = allocator.m_data;
		m_usedSpace = allocator.m_usedSpace;
		allocator.m_size = 0;
		allocator.m_data = nullptr;
		allocator.m_usedSpace = nullptr;
		return *this;
	}



	template <typename T, bool A>
	se::memory::PoolAllocator<T, A>::Handle PoolAllocator<T, A>::allocate(se::Size count) {
		for (se::Size i {0}; i < m_size; ++i) {
			if (m_usedSpace[i])
				continue;

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

			se::memory::PoolAllocator<T, A>::Handle handle {m_data + i, count};
			return handle;
		}

		if constexpr (A)
			throw se::exceptions::BadAllocation("Can't allocate a spot of the wanted size in PoolAllocator");

		return {};
	}



	template <typename T, bool A>
	void PoolAllocator<T, A>::free(const se::memory::PoolAllocator<T, A>::Handle &handle) {
		se::Size startPosition {static_cast<se::Size> (handle.m_ptr - m_data)};
		for (se::Size i {startPosition}; i < startPosition + handle.m_count; ++i)
			m_usedSpace[i] = false;
	}



	template <typename T, bool A>
	se::memory::PoolAllocator<T, A>::Handle PoolAllocator<T, A>::reallocate(
		const se::memory::PoolAllocator<T, A>::Handle &handle, se::Size count
	) {
		this->free(handle);
		se::memory::PoolAllocator<T, A>::Handle output {this->allocate(handle.m_count + count)};
		for (se::Size i {0}; i < handle.m_count; ++i)
			output[i] = handle[i];
		return output;
	}



} // namespace se::memory
