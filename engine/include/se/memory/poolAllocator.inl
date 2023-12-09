#include "../essentials/exception.hpp"
#include "poolAllocator.hpp"



namespace se
{
	template <typename T>
	PoolAllocator<T>::Handle::Handle() :
		m_ptr {nullptr},
		m_poolAllocator {nullptr}
	{

	}



	template <typename T>
	PoolAllocator<T>::Handle::Handle(const se::PoolAllocator<T> *poolAllocator, T *ptr) :
		m_ptr {ptr},
		m_poolAllocator {poolAllocator}
	{

	}



	template <typename T>
	PoolAllocator<T>::Handle::Handle(const se::PoolAllocator<T>::Handle &handle) :
		m_ptr {handle.m_ptr},
		m_poolAllocator {handle.m_poolAllocator}
	{

	}



	template <typename T>
	PoolAllocator<T>::Handle::Handle(se::PoolAllocator<T>::Handle &&handle) noexcept :
		m_ptr {handle.m_ptr},
		m_poolAllocator {handle.m_poolAllocator}
	{
		handle.m_ptr = nullptr;
		handle.m_poolAllocator = nullptr;
	}



	template <typename T>
	const se::PoolAllocator<T>::Handle &PoolAllocator<T>::Handle::operator=(const se::PoolAllocator<T>::Handle &handle)
	{
		m_ptr = handle.m_ptr;
		m_poolAllocator = handle.m_poolAllocator;
		return *this;
	}



	template <typename T>
	const se::PoolAllocator<T>::Handle &PoolAllocator<T>::Handle::operator=(se::PoolAllocator<T>::Handle &&handle) noexcept
	{
		m_ptr = handle.m_ptr;
		m_poolAllocator = handle.m_poolAllocator;
		handle.m_ptr = nullptr;
		handle.m_poolAllocator = nullptr;
		return *this;
	}



	template <typename T>
	T &PoolAllocator<T>::Handle::operator*() const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't dereference an invalid pointer to a pool allocated value");
		return *m_ptr;
	}



	//[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
	template <typename T>
	T *PoolAllocator<T>::Handle::getRawPointer() const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't get an invalid pointer to a pool allocated value");
		return m_ptr;
	}



	template <typename T>
	bool PoolAllocator<T>::Handle::isValid() const noexcept
	{
		if (m_poolAllocator == nullptr)
			return false;
		return m_poolAllocator->isHandleValid(*this);
	}






	template <typename T>
	PoolAllocator<T>::PoolAllocator(se::Size size) :
		m_data {nullptr},
		m_size {0},
		m_usage {0}
	{
		m_data = reinterpret_cast<PoolAllocator<T>::Data*> (malloc(sizeof(PoolAllocator<T>::Data) * size));
		if (m_data == nullptr)
			throw SE_UnknownRuntimeError("Can't allocate memory for pool allocator");

		m_size = size;
	}



	template <typename T>
	PoolAllocator<T>::~PoolAllocator()
	{
		if (m_data != nullptr)
			::free(m_data);
	}



	template <typename T>
	bool PoolAllocator<T>::isHandleValid(const se::PoolAllocator<T>::Handle &handle) const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't check validity of a handle on an invalid stack allocator");
		SE_UNKNOWN_ASSERT(handle.m_poolAllocator == this, "Can't check validity of a handle of an other stack allocator");

		if (reinterpret_cast<se::Size> (handle.m_ptr) < reinterpret_cast<se::Size>(m_data))
			return false;

		if (reinterpret_cast<se::Size> (handle.m_ptr) >= reinterpret_cast<se::Size> (m_data) + m_size * sizeof(se::PoolAllocator<T>::Data))
			return false;

		if (reinterpret_cast<se::Size> (handle.m_ptr) - 1 >= reinterpret_cast<se::Size> (m_data) + (m_size - 1) * sizeof(se::PoolAllocator<T>::Data))
			return false;

		if (!reinterpret_cast<se::PoolAllocator<T>::Data*> (handle.m_ptr)->isInUse)
			return false;

		return true;
	}



	template <typename T>
	se::PoolAllocator<T>::Handle PoolAllocator<T>::allocate()
	{
		for (se::Size i {0}; i < m_size; ++i)
		{
			if (!m_data[i].isInUse)
			{
				m_data[i].isInUse = true;
				++m_usage;
				return se::PoolAllocator<T>::Handle(this, reinterpret_cast<T*> (m_data + i));
			}
		}

		throw SE_UnknownRuntimeError("Can't allocate memory from pool allocator because it is full");
	}



	template <typename T>
	void PoolAllocator<T>::free(const se::PoolAllocator<T>::Handle &handle)
	{
		SE_UNKNOWN_ASSERT(handle.isValid(), "Can't free invalid handle");

		--m_usage;
		reinterpret_cast<se::PoolAllocator<T>::Data*> (handle.m_ptr)->isInUse = false;
	}



	template <typename T>
	bool PoolAllocator<T>::isValid() const noexcept
	{
		return m_data != nullptr && m_size != 0;
	}



	template <typename T>
	se::Size PoolAllocator<T>::getSize() const noexcept
	{
		return m_size;
	}



	template <typename T>
	se::Size PoolAllocator<T>::getUsage() const noexcept
	{
		return m_usage;
	}



} // namespace se
