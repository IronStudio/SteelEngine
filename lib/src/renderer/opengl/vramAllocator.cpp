#include "se/renderer/opengl/vramAllocator.hpp"

#include "se/assertion.hpp"



namespace se::renderer::opengl {
	VramAllocatorHandle::VramAllocatorHandle(se::renderer::VramAllocator *allocator, se::ByteCount start, se::ByteCount size) :
		se::renderer::VramAllocatorHandle(allocator),
		m_start {start},
		m_size {size}
	{

	}


	VramAllocatorHandle::~VramAllocatorHandle() {
		if (m_start >= 0 && m_size > 0)
			m_allocator->free(this);
	}


	bool VramAllocatorHandle::isValid() {
		return true;
	}




	VramAllocator::VramAllocator(const se::renderer::VramAllocatorInfos &infos) :
		se::renderer::VramAllocator(infos),
		m_allocationTable {},
		m_memory {}
	{
		GLenum usage {};
		if (m_infos.usageFrequency == se::renderer::VramUsageFrequency::eForgotten) {
			if (m_infos.usageNature == se::renderer::VramUsageNature::eCpuToGpu)
				usage = GL_STREAM_DRAW;
			else if (m_infos.usageNature == se::renderer::VramUsageNature::eGpuToCpu)
				usage = GL_STREAM_READ;
			else
				usage = GL_STREAM_COPY;
		}

		else if (m_infos.usageFrequency == se::renderer::VramUsageFrequency::eStatic) {
			if (m_infos.usageNature == se::renderer::VramUsageNature::eCpuToGpu)
				usage = GL_STATIC_DRAW;
			else if (m_infos.usageNature == se::renderer::VramUsageNature::eGpuToCpu)
				usage = GL_STATIC_READ;
			else
				usage = GL_STATIC_COPY;
		}

		else {
			if (m_infos.usageNature == se::renderer::VramUsageNature::eCpuToGpu)
				usage = GL_DYNAMIC_DRAW;
			else if (m_infos.usageNature == se::renderer::VramUsageNature::eGpuToCpu)
				usage = GL_DYNAMIC_READ;
			else
				usage = GL_DYNAMIC_COPY;
		}

		glCreateBuffers(1, &m_memory);
		glNamedBufferData(m_memory, m_infos.chunkSize, nullptr, usage);
	}


	VramAllocator::~VramAllocator() noexcept {
		if (m_memory != 0)
			glDeleteBuffers(1, &m_memory);
	}


	std::unique_ptr<VramAllocator::Handle> VramAllocator::allocate(const se::renderer::VramAllocationInfos &allocationInfos) {
		se::ByteCount start {0};
		se::ByteCount size {0};

		se::ByteCount lastEnd {0};
		se::ByteCount lastStart {m_infos.chunkSize};
		for (auto it {m_allocationTable.begin()}; it != m_allocationTable.end(); ++it) {
			if (lastStart - lastEnd >= allocationInfos.size) {
				start = lastEnd;
				size = allocationInfos.size;
				break;
			}

			lastStart = it->start;
			lastEnd = it->end;
		}

		if (m_infos.chunkSize - lastEnd >= allocationInfos.size) {
			start = lastEnd;
			size = allocationInfos.size;
		}

		return std::make_unique<se::renderer::opengl::VramAllocatorHandle> (this, start, size);
	}


	void VramAllocator::free(Handle *handle) {

	}


} // namespace se::renderer::opengl
