#include "se/renderer/vulkan/commandBuffer.hpp"

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"



namespace se::renderer::vulkan {

	CommandBuffer::CommandBuffer() :
		m_infos {},
		m_commandBuffer {VK_NULL_HANDLE}
	{

	}


	CommandBuffer::CommandBuffer(const se::renderer::vulkan::CommandBufferInfos &infos) :
		m_infos {infos},
		m_commandBuffer {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
		s_createCommandPool(m_infos);

		VkCommandBufferAllocateInfo commandBufferAllocateInfos {};
		commandBufferAllocateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfos.commandBufferCount = 1;
		commandBufferAllocateInfos.commandPool = s_commandPools[m_infos.queue];
		commandBufferAllocateInfos.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfos, &m_commandBuffer) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't allocate command buffer");
	}


	CommandBuffer::~CommandBuffer() {
		s_destroyCommandPool(m_infos);
	}


	CommandBuffer::CommandBuffer(CommandBuffer &&commandBuffer) noexcept :
		m_infos {commandBuffer.m_infos},
		m_commandBuffer {commandBuffer.m_commandBuffer}
	{
		commandBuffer.m_commandBuffer = VK_NULL_HANDLE;
	}


	const CommandBuffer &CommandBuffer::operator=(CommandBuffer &&commandBuffer) noexcept {
		m_infos = commandBuffer.m_infos;
		m_commandBuffer = commandBuffer.m_commandBuffer;
		commandBuffer.m_commandBuffer = VK_NULL_HANDLE;
		return *this;
	}


	void CommandBuffer::s_createCommandPool(const se::renderer::vulkan::CommandBufferInfos &infos) {
		if (s_commandPools.find(infos.queue) != s_commandPools.end() && s_commandPoolCounts[infos.queue] != 0) {
			++s_commandPoolCounts[infos.queue];
			return;
		}

		auto &device {*reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()};

		VkCommandPoolCreateInfo commandPoolCreateInfos {};
		commandPoolCreateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfos.queueFamilyIndex = device.getQueueFamilyIndices().find(infos.queue)->second;
		commandPoolCreateInfos.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VkCommandPool commandPool {VK_NULL_HANDLE};
		if (vkCreateCommandPool(device.getDevice(), &commandPoolCreateInfos, nullptr, &commandPool) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create command pool");

		s_commandPoolCounts[infos.queue] = 1;
		s_commandPools[infos.queue] = commandPool;
	}


	void CommandBuffer::s_destroyCommandPool(const se::renderer::vulkan::CommandBufferInfos &infos) {
		--s_commandPoolCounts[infos.queue];
		if (s_commandPoolCounts[infos.queue] != 0)
			return;

		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (infos.context)->getDevice()->getDevice()};
		vkDestroyCommandPool(device, s_commandPools[infos.queue], nullptr);
	}



	std::map<se::renderer::vulkan::QueueType, VkCommandPool> CommandBuffer::s_commandPools {};
	std::map<se::renderer::vulkan::QueueType, se::Count> CommandBuffer::s_commandPoolCounts {};

} // namespace se::renderer::vulkan
