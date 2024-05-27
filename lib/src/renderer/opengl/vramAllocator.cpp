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




	VramAllocator::VramAllocator(const se::renderer::VramAllocatorInfos &infos) :
		se::renderer::VramAllocator(infos),
		m_allocationTable {},
		m_memory {}
	{
		SE_ASSERT(m_infos.usage != se::renderer::VramUsageMask(), "You must specify the buffer usage !");

		GLenum usage {};
		if ((m_infos.usage & se::renderer::VramUsage::eRead) && (m_infos.usage & se::renderer::VramUsage::eWrite))
			usage = GL_READ_WRITE;
		else if (m_infos.usage & se::renderer::VramUsage::eRead)
			usage = GL_READ_ONLY;
		else if (m_infos.usage & se::renderer::VramUsage::eWrite)
			usage = GL_WRITE_ONLY;


		glGenBuffers(1, &m_memory);
		//glBindBuffer(GL_TEXTURE_BUFFER, m_memory);
		glNamedBufferData(m_memory, m_infos.chunkSize, nullptr, GL_READ_WRITE);
	}


	VramAllocator::~VramAllocator() noexcept {
		if (m_memory != 0)
			glDeleteBuffers(1, &m_memory);
	}


	std::unique_ptr<VramAllocator::Handle> VramAllocator::allocate(const se::renderer::VramAllocationInfos &allocationInfos) {
		se::ByteCount start {};
		se::ByteCount size {};

		se::ByteCount lastEnd {0};
		se::ByteCount lastStart {m_infos.chunkSize};
		for (auto it {m_allocationTable.begin()}; it != m_allocationTable.end(); ++it) {
			lastStart = it->start;
			//if (lastStart - lastEnd >= allocationInfos.size)
		}

		return std::make_unique<se::renderer::opengl::VramAllocatorHandle> (this, start, size);
	}


	void VramAllocator::free(Handle *handle) {

	}


} // namespace se::renderer::opengl
