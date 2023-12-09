#include "essentials/assertion.hpp"
#include "essentials/exception.hpp"
#include "memory/stackAllocator.hpp"



namespace se
{
	StackAllocator::StackAllocator(se::Size size) :
		m_stackBottom {nullptr},
		m_stackTop {nullptr},
		m_stackSize {0}
	{
		m_stackBottom = malloc(size);
		if (m_stackBottom == nullptr)
			throw SE_UnknownRuntimeError("Can't allocate memory for a stack allocator");

		m_stackTop = m_stackBottom;
		m_stackSize = size;
	}



	StackAllocator::~StackAllocator()
	{
		if (m_stackBottom == nullptr)
			::free(m_stackBottom);
	}



	//[[deprecated("Please avoid using raw pointer in general, because it can cause problem when defragmenting RAM and it isn't as safe as using handles")]]
	bool StackAllocator::isPointerValid(void *ptr) const
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't check validity of a handle on an invalid stack allocator");

		if (ptr < m_stackBottom)
			return false;

		if (ptr >= m_stackTop)
			return false;

		return true;
	}



} // namespace se
