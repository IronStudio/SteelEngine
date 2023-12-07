#include "../essentials/assertion.hpp"
#include "stackAllocator.hpp"



namespace se
{
	template <typename T>
	StackAllocator::Handle<T>::Handle() :
		m_ptr {nullptr},
		m_stackAllocator {nullptr}
	{

	}



	template <typename T>
	StackAllocator::Handle<T>::Handle(const se::StackAllocator *stackAllocator, void *ptr) :
		m_ptr {ptr},
		m_stackAllocator {stackAllocator}
	{

	}



	template <typename T>
	StackAllocator::Handle<T>::Handle(const se::StackAllocator::Handle<T> &handle) :
		m_ptr {handle.m_ptr},
		m_stackAllocator {handle.m_stackAllocator}
	{

	}



	template <typename T>
	StackAllocator::Handle<T>::Handle(se::StackAllocator::Handle<T> &&handle) noexcept :
		m_ptr {handle.m_ptr},
		m_stackAllocator {handle.m_stackAllocator}
	{
		handle.m_ptr = nullptr;
		handle.m_stackAllocator = nullptr;
	}



	template <typename T>
	const se::StackAllocator::Handle<T> &StackAllocator::Handle<T>::operator=(const se::StackAllocator::Handle<T> &handle)
	{
		m_ptr = handle.m_ptr;
		m_stackAllocator = handle.m_stackAllocator;
		return *this;
	}



	template <typename T>
	const se::StackAllocator::Handle<T> &StackAllocator::Handle<T>::operator=(se::StackAllocator::Handle<T> &&handle) noexcept
	{
		m_ptr = handle.m_ptr;
		m_stackAllocator = handle.m_stackAllocator;
		handle.m_ptr = nullptr;
		handle.m_stackAllocator = nullptr;
		return *this;
	}



	template <typename T>
	T &StackAllocator::Handle<T>::operator*() const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't dereference an invalid pointer to a stack allocated value");
		return *static_cast<T*> (m_ptr);
	}



	template <typename T>
	//[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
	T *StackAllocator::Handle<T>::getRawPointer() const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't get an invalid pointer to a stack allocated value");
		return static_cast<T*> (*m_ptr);
	}



	template <typename T>
	bool StackAllocator::Handle<T>::isValid() const noexcept
	{
		if (m_stackAllocator == nullptr)
			return false;
		return m_stackAllocator->isHandleValid(*this);
	}



	template <typename T>
	bool StackAllocator::isHandleValid(const se::StackAllocator::Handle<T> &handle) const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't check validity of a handle on an invalid stack allocator");
		SE_UNKNOWN_ASSERT(handle.m_stackAllocator == this, "Can't check validity of a handle of an other stack allocator");

		if (handle.m_ptr < m_stackBottom)
			return false;

		if (handle.m_ptr >= m_stackTop)
			return false;

		if (reinterpret_cast<size_t> (handle.m_ptr) + sizeof(T) - 1 >= reinterpret_cast<size_t> (m_stackTop))
			return false;

		return true;
	}



	se::StackAllocator::Point StackAllocator::getStackTop() const noexcept
	{
		return se::StackAllocator::Point(this, m_stackTop);
	}



	template <typename T>
	se::StackAllocator::Handle<T> StackAllocator::allocate(size_t amount)
	{
		se::StackAllocator::Handle<T> handle {this, m_stackTop};
		m_stackTop = reinterpret_cast<void*> (reinterpret_cast<size_t> (m_stackTop) + sizeof(T) * amount);
		if (reinterpret_cast<size_t> (m_stackTop) > reinterpret_cast<size_t> (m_stackBottom) + m_stackSize)
			throw SE_UnknownRuntimeError("No more memory available on stack allocator");
		return handle;
	}



	void StackAllocator::free(const se::StackAllocator::Point &point)
	{
		m_stackTop = point.m_ptr;
	}



	bool StackAllocator::isValid() const noexcept
	{
		return m_stackBottom != nullptr && m_stackTop != nullptr && m_stackSize != 0;
	}



	size_t StackAllocator::getSize() const noexcept
	{
		return m_stackSize;
	}



	size_t StackAllocator::getUsage() const noexcept
	{
		if (!this->isValid())
			return 0;
		return reinterpret_cast<size_t> (m_stackTop) - reinterpret_cast<size_t> (m_stackBottom);
	}



} // namespace se
