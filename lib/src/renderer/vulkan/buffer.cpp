#include "se/renderer/vulkan/buffer.hpp"

#include <map>

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"



namespace se::renderer::vulkan {
	Buffer::Buffer(const se::renderer::BufferInfos &infos) :
		se::renderer::Buffer(infos),
		m_buffer {VK_NULL_HANDLE},
		m_allocatorHandle {}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		std::map<se::renderer::BufferUsage, VkBufferUsageFlags> bufferUsageMap {
			{se::renderer::BufferUsage::eTransferSrc, VK_BUFFER_USAGE_TRANSFER_SRC_BIT},
			{se::renderer::BufferUsage::eTransferDst, VK_BUFFER_USAGE_TRANSFER_DST_BIT},
			{se::renderer::BufferUsage::eUniform, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT},
			{se::renderer::BufferUsage::eStorage, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT},
			{se::renderer::BufferUsage::eIndex, VK_BUFFER_USAGE_INDEX_BUFFER_BIT},
			{se::renderer::BufferUsage::eVertex, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT},
			{se::renderer::BufferUsage::eIndirect, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT}
		};

		VkBufferUsageFlags usage {0};
		for (se::Count i {0}; i < bufferUsageMap.size(); ++i) {
			if (!(m_infos.usage & (se::renderer::BufferUsage)(1 << i)))
				continue;

			usage |= bufferUsageMap[(se::renderer::BufferUsage)(1 << i)];
		}

		VkBufferCreateInfo bufferCreateInfos {};
		bufferCreateInfos.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfos.queueFamilyIndexCount = 0;
		bufferCreateInfos.pQueueFamilyIndices = nullptr;
		bufferCreateInfos.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfos.size = m_infos.size;
		bufferCreateInfos.usage = usage;

		if (vkCreateBuffer(device, &bufferCreateInfos, nullptr, &m_buffer) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create buffer of size " + std::to_string(m_infos.size));

		VkMemoryRequirements memoryRequirements {};
		vkGetBufferMemoryRequirements(device, m_buffer, &memoryRequirements);

		se::renderer::VramAllocationInfos allocationInfos {};
		allocationInfos.alignement = memoryRequirements.alignment;
		allocationInfos.size = memoryRequirements.size;
		m_allocatorHandle = m_infos.allocator->allocate(allocationInfos);

		auto handle {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (m_allocatorHandle.get())};
		if (vkBindBufferMemory(device, m_buffer, handle->getMemory(), handle->getOffset()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't bind buffer to memory");
	}


	Buffer::~Buffer() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_buffer != VK_NULL_HANDLE)
			vkDestroyBuffer(device, m_buffer, nullptr);
	}




	BufferTransferor::BufferTransferor(const se::renderer::BufferTransferorInfos &infos) :
		se::renderer::BufferTransferor(infos),
		m_commandBuffer {VK_NULL_HANDLE}
	{

	}


	BufferTransferor::~BufferTransferor() {

	}


	void BufferTransferor::transfer(const se::renderer::BufferTransferInfos &infos) {

	}


	void BufferTransferor::s_loadCommandPool(const se::renderer::BufferTransferorInfos &infos) {
		VkCommandPoolCreateInfo commandPoolCreateInfos {};
		commandPoolCreateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfos.queueFamilyIndex = ;
		s_commandPool;
	}


	void BufferTransferor::s_unloadCommandPool() {

	}


	se::Count BufferTransferor::s_instanceCount {0};
	VkCommandPool BufferTransferor::s_commandPool {VK_NULL_HANDLE};

} // namespace se::renderer::vulkan
