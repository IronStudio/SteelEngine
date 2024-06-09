#include "se/renderer/vulkan/buffer.hpp"

#include <map>

#include "se/assertion.hpp"
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


	void Buffer::write(const se::renderer::BufferWriteInfos &writeInfos) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		SE_ASSERT(m_infos.allocator->getInfos().usageNature == se::renderer::VramUsageNature::eCpuToGpu, "Can't write to buffer that is not cpu->gpu");
		SE_ASSERT(writeInfos.value.size() * sizeof(se::Byte) + writeInfos.offset <= m_infos.size, "Can't write data that goes outside of the buffer");

		VkDeviceMemory memory {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (m_allocatorHandle.get())->getMemory()};
		se::ByteCount offset {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (m_allocatorHandle.get())->getOffset()};
		offset += writeInfos.offset;

		void *data {};
		if (vkMapMemory(device, memory, offset, m_infos.size, 0, &data) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't map buffer memory");

		memcpy(data, writeInfos.value.data(), writeInfos.value.size() * sizeof(se::Byte));
		vkUnmapMemory(device, memory);
	}


	void Buffer::write(const se::renderer::BufferWriteAttributeInfos &writeInfos) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		SE_ASSERT(m_infos.allocator->getInfos().usageNature == se::renderer::VramUsageNature::eCpuToGpu, "Can't write to buffer that is not cpu->gpu");
		SE_ASSERT(
			writeInfos.attributeBufferView->getTotalSize() * sizeof(se::Byte) + writeInfos.offset <= m_infos.size,
			"Can't write data that goes outside of the buffer"
		);

		VkDeviceMemory memory {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (m_allocatorHandle.get())->getMemory()};
		se::ByteCount offset {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (m_allocatorHandle.get())->getOffset()};
		offset += writeInfos.offset;

		void *data {};
		if (vkMapMemory(device, memory, offset, m_infos.size, 0, &data) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't map buffer memory");

		for (const auto &attribute : writeInfos.attributes) {
			memcpy(
				(se::Byte*)data + writeInfos.attributeBufferView->getAttribute(attribute.name).offset,
				attribute.value.data(),
				attribute.value.size() * sizeof(se::Byte)
			);
		}

		vkUnmapMemory(device, memory);
	}




	BufferTransferor::BufferTransferor(const se::renderer::BufferTransferorInfos &infos) :
		se::renderer::BufferTransferor(infos),
		m_commandBuffer {VK_NULL_HANDLE},
		m_inUseFences {},
		m_freeFences {},
		m_fenceMutex {}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		s_loadCommandPool(m_infos);

		VkCommandBufferAllocateInfo commandBufferAllocateInfos {};
		commandBufferAllocateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfos.commandBufferCount = 1;
		commandBufferAllocateInfos.commandPool = s_commandPool;
		commandBufferAllocateInfos.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfos, &m_commandBuffer) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't allocate memory for transfer command buffer");
	}


	BufferTransferor::~BufferTransferor() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
		m_fenceMutex.lock();
		for (const auto &fence : m_freeFences)
			vkDestroyFence(device, fence, nullptr);
		if (!m_inUseFences.empty())
			(void)vkWaitForFences(device, m_inUseFences.size(), m_inUseFences.data(), VK_TRUE, UINT64_MAX);
		for (const auto &fence : m_inUseFences)
			vkDestroyFence(device, fence, nullptr);
		m_fenceMutex.unlock();

		if (m_commandBuffer != VK_NULL_HANDLE)
			vkFreeCommandBuffers(device, s_commandPool, 1, &m_commandBuffer);
		s_unloadCommandPool(m_infos);
	}


	void BufferTransferor::transfer(const se::renderer::BufferTransferInfos &infos) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		VkCommandBufferBeginInfo commandBufferBeginInfos {};
		commandBufferBeginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfos.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		commandBufferBeginInfos.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfos) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't begin transfer command buffer");

		VkBufferCopy bufferCopy {};
		bufferCopy.srcOffset = infos.srcOffset;
		bufferCopy.dstOffset = infos.dstOffset;
		bufferCopy.size = infos.size;

		VkBuffer srcBuffer {reinterpret_cast<se::renderer::vulkan::Buffer*> (infos.source)->getBuffer()};
		VkBuffer dstBuffer {reinterpret_cast<se::renderer::vulkan::Buffer*> (infos.destination)->getBuffer()};

		vkCmdCopyBuffer(m_commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);

		if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't end command buffer");

		VkSubmitInfo submitInfos {};
		submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfos.commandBufferCount = 1;
		submitInfos.pCommandBuffers = &m_commandBuffer;
		submitInfos.waitSemaphoreCount = 0;
		submitInfos.signalSemaphoreCount = 0;

		se::Count queueFamilyIndex {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)
			->getDevice()->getQueueFamilyIndices().find(se::renderer::vulkan::QueueType::eTransfer)->second};
		VkQueue queue {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getQueues()
			.find(se::renderer::vulkan::QueueType::eTransfer)->second.find(queueFamilyIndex)->second[0]};

		VkFence fence {VK_NULL_HANDLE};
		std::lock_guard _ {m_fenceMutex};
		if (m_freeFences.empty()) {
			m_inUseFences.push_back(VK_NULL_HANDLE);
			VkFenceCreateInfo fenceCreateInfos {};
			fenceCreateInfos.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfos.flags = 0;
			if (vkCreateFence(device, &fenceCreateInfos, nullptr, &fence) != VK_SUCCESS)
				throw se::exceptions::RuntimeError("Can't create new fences to synchronise transfer");

			*m_inUseFences.rbegin() = fence;
		}

		else {
			m_inUseFences.push_back(*m_freeFences.begin());
			m_freeFences.erase(m_freeFences.begin());
			fence = *m_inUseFences.rbegin();
		}

		if (vkQueueSubmit(queue, 1, &submitInfos, fence) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't submit transfer to queue");
	}


	void BufferTransferor::sync() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		std::lock_guard _ {m_fenceMutex};
		if (vkWaitForFences(device, m_inUseFences.size(), m_inUseFences.data(), VK_TRUE, UINT64_MAX) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't wait for transfer fences");
		m_freeFences.insert(m_freeFences.end(), m_inUseFences.begin(), m_inUseFences.end());
		if (vkResetFences(device, m_inUseFences.size(), m_inUseFences.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't reset transfer fences");
		m_inUseFences.clear();
	}


	void BufferTransferor::s_loadCommandPool(const se::renderer::BufferTransferorInfos &infos) {
		++s_instanceCount;
		if (s_instanceCount > 1)
			return;

		se::Count queueFamilyIndex {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)
			->getDevice()->getQueueFamilyIndices().find(se::renderer::vulkan::QueueType::eTransfer)->second};
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getDevice()};

		VkCommandPoolCreateInfo commandPoolCreateInfos {};
		commandPoolCreateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfos.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfos.queueFamilyIndex = queueFamilyIndex;
		if (vkCreateCommandPool(device, &commandPoolCreateInfos, nullptr, &s_commandPool) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create command pool for buffer transfer");
	}


	void BufferTransferor::s_unloadCommandPool(const se::renderer::BufferTransferorInfos &infos) {
		--s_instanceCount;
		if (s_instanceCount > 0)
			return;

		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getDevice()};
		vkDestroyCommandPool(device, s_commandPool, nullptr);
	}


	se::Count BufferTransferor::s_instanceCount {0};
	VkCommandPool BufferTransferor::s_commandPool {VK_NULL_HANDLE};

} // namespace se::renderer::vulkan
