#include "../essentials/assertion.hpp"
#include "heapAllocator.hpp"

#include <iostream>



namespace se
{
	template <typename T>
	HeapAllocator::Handle<T>::Handle() :
		m_ptr {nullptr},
		m_heapAllocator {nullptr},
		m_count {0}
	{

	}



	template <typename T>
	HeapAllocator::Handle<T>::Handle(const se::HeapAllocator *heapAllocator, void *ptr, se::Length count) :
		m_ptr {ptr},
		m_heapAllocator {heapAllocator},
		m_count {count}
	{

	}



	template <typename T>
	HeapAllocator::Handle<T>::Handle(const se::HeapAllocator::Handle<T> &handle) :
		m_ptr {handle.m_ptr},
		m_heapAllocator {handle.m_heapAllocator},
		m_count {handle.m_count}
	{

	}



	template <typename T>
	HeapAllocator::Handle<T>::Handle(se::HeapAllocator::Handle<T> &&handle) noexcept :
		m_ptr {handle.m_ptr},
		m_heapAllocator {handle.m_heapAllocator},
		m_count {handle.m_count}
	{
		handle.m_ptr = nullptr;
		handle.m_heapAllocator = nullptr;
		handle.m_count = 0;
	}



	template <typename T>
	const se::HeapAllocator::Handle<T> &HeapAllocator::Handle<T>::operator=(const se::HeapAllocator::Handle<T> &handle)
	{
		m_ptr = handle.m_ptr;
		m_heapAllocator = handle.m_heapAllocator;
		m_count = handle.m_count;
		return *this;
	}



	template <typename T>
	const se::HeapAllocator::Handle<T> &HeapAllocator::Handle<T>::operator=(se::HeapAllocator::Handle<T> &&handle) noexcept
	{
		m_ptr = handle.m_ptr;
		m_heapAllocator = handle.m_heapAllocator;
		m_count = handle.m_count;

		handle.m_ptr = nullptr;
		handle.m_heapAllocator = nullptr;
		handle.m_count = 0;

		return *this;
	}



	template <typename T>
	T &HeapAllocator::Handle<T>::operator*() const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't dereference an invalid heap handle");
		return *reinterpret_cast<T*> (reinterpret_cast<se::HeapAllocator::ReferenceTableElement*> (m_ptr)->ptr);
	}



	template <typename T>
	T *HeapAllocator::Handle<T>::getRawPointer() const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't get raw pointer of an invalid heap handle");
		return reinterpret_cast<T*> (reinterpret_cast<se::HeapAllocator::ReferenceTableElement*> (m_ptr)->ptr);
	}



	template <typename T>
	bool HeapAllocator::Handle<T>::isValid() const noexcept
	{
		if (m_heapAllocator == nullptr)
			return false;

		return m_heapAllocator->isHandleValid(*this);
	}






	template <typename T>
	bool HeapAllocator::isHandleValid(const se::HeapAllocator::Handle<T> &handle) const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't check validity of a handle on an invalid heap allocator");

		if (handle.m_heapAllocator != this)
			return false;

		if (handle.m_count == 0)
			return false;

		if (handle.m_ptr == nullptr)
			return false;

		if (handle.m_ptr < m_referenceTable)
			return false;

		if (handle.m_ptr >= m_referenceTable + m_referenceTableSize)
			return false;

		if (!reinterpret_cast<se::HeapAllocator::ReferenceTableElement*> (handle.m_ptr)->isUsed)
			return false;

		return true;
	}



	template <typename T>
	se::HeapAllocator::Handle<T> HeapAllocator::allocate(se::Length amount)
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't allocate memory on an invalid heap allocator");

		se::Size sizeToAllocate {sizeof(T) * amount};
		if (reinterpret_cast<se::Byte*> (m_nextBlockToAllocate) + sizeToAllocate > reinterpret_cast<se::Byte*> (m_heap) + m_size)
			throw SE_UnknownRuntimeError("Can't allocate enough on the given heap allocator");

		se::HeapAllocator::ReferenceTableElement *reference {nullptr};
		for (se::Length i {0}; i < m_referenceTableSize; ++i)
		{
			if (m_referenceTable[i].isUsed)
				continue;

			reference = m_referenceTable + i;
			reference->isUsed = true;
			reference->ptr = m_nextBlockToAllocate;
			reference->size = sizeToAllocate;
			break;
		}

		se::HeapAllocator::Handle<T> handle {this, reference, amount};
		m_nextBlockToAllocate = reinterpret_cast<se::Byte*> (m_nextBlockToAllocate) + sizeToAllocate;
		m_usage += sizeToAllocate;
		return handle;
	}



	template <typename T>
	void HeapAllocator::free(const se::HeapAllocator::Handle<T> &handle)
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't free memory on an invalid heap allocator");
		SE_UNKNOWN_ASSERT(handle.isValid(), "Can't free an invalid handle on a heap allocator");

		auto ptr {reinterpret_cast<se::HeapAllocator::ReferenceTableElement*> (handle.m_ptr)};
		m_usage -= ptr->size;
		ptr->ptr = nullptr;
		ptr->isUsed = false;
		ptr->size = 0;
	}



	inline bool HeapAllocator::isValid() const noexcept
	{
		return m_heap != nullptr
			&& m_referenceTable != nullptr
			&& m_nextBlockToAllocate != nullptr
			&& m_nextBlockToAllocate >= reinterpret_cast<se::Byte*> (m_heap)
			&& m_nextBlockToAllocate < reinterpret_cast<se::Byte*> (m_heap) + m_size
			&& m_size != 0
			&& m_usage <= m_size
			&& m_referenceTableSize != 0;
	}



	inline se::Size HeapAllocator::getSize() const noexcept
	{
		return m_size;
	}



	inline se::Size HeapAllocator::getUsage() const noexcept
	{
		return m_usage;
	}



} // namespace se
