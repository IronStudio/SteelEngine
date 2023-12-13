#include <cmath>

#include "essentials/exception.hpp"
#include "memory/heapAllocator.hpp"



namespace se
{
	HeapAllocator::HeapAllocator(se::Size size, se::Float ratio) :
		m_heap {nullptr},
		m_nextBlockToAllocate {nullptr},
		m_referenceTable {nullptr},
		m_size {size},
		m_usage {0},
		m_referenceTableSize {0}
	{
		SE_UNKNOWN_ASSERT(size != 0, "Can't create an empty heap allocator");
		SE_UNKNOWN_ASSERT(ratio > 0 && ratio <= 1, "Ratio must be a number between 0 and 1");


		m_heap = malloc(m_size);
		if (m_heap == nullptr)
			throw SE_UnknownRuntimeError("Can't allocate memory for a heap allocator");

		m_referenceTableSize = (se::Length)ceilf((se::Float)m_size * ratio);
		m_referenceTable = reinterpret_cast<se::HeapAllocator::ReferenceTableElement*> (malloc(
			m_referenceTableSize * sizeof(se::HeapAllocator::ReferenceTableElement)));
		if (m_referenceTable == nullptr)
			throw SE_UnknownRuntimeError("Can't allocate memory for heap allocator reference table");

		m_nextBlockToAllocate = m_heap;
	}



	HeapAllocator::~HeapAllocator()
	{
		if (m_referenceTable != nullptr)
			::free(m_referenceTable);

		if (m_heap != nullptr)
			::free(m_heap);

		m_referenceTable = nullptr;
		m_heap = nullptr;
		m_nextBlockToAllocate = nullptr;
		m_size = 0;
		m_usage = 0;
		m_referenceTableSize = 0;
	}



	void HeapAllocator::defragment(se::Length maxChunkCount)
	{
		SE_UNKNOWN_ASSERT(this->isValid(), "Can't defragment invalid heap allocator");
	}



	se::Size HeapAllocator::getFragmentationSpaceSize() const noexcept
	{
		void *highestAddress {nullptr};

		for (se::Length i {0}; i < m_referenceTableSize; ++i)
		{
			if (!m_referenceTable[i].isUsed)
				continue;

			if (reinterpret_cast<se::Byte*> (m_referenceTable[i].ptr) + m_referenceTable[i].size > highestAddress)
				highestAddress = reinterpret_cast<se::Byte*> (m_referenceTable[i].ptr) + m_referenceTable[i].size;
		}

		se::Size sizeUsed {static_cast<se::Size> (reinterpret_cast<se::Byte*> (highestAddress) - reinterpret_cast<se::Byte*> (m_heap))};
		return sizeUsed - m_usage;
	}



} // namespace se
