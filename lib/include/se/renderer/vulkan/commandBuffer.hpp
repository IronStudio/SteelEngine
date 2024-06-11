#pragma once

#include <map>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/vulkan/device.hpp"



namespace se::renderer::vulkan {
	struct CommandBufferInfos {
		se::renderer::Context *context;
		se::renderer::vulkan::QueueType queue;
	};

	class SE_CORE CommandBuffer final {
		public:
			CommandBuffer();
			CommandBuffer(const se::renderer::vulkan::CommandBufferInfos &infos);
			~CommandBuffer();

			CommandBuffer(CommandBuffer &&commandBuffer) noexcept;
			const CommandBuffer &operator=(CommandBuffer &&commandBuffer) noexcept;

			CommandBuffer(const CommandBuffer &) = delete;
			const CommandBuffer &operator=(const CommandBuffer &) = delete;

			inline const VkCommandBuffer &getCommandBuffer() const noexcept {return m_commandBuffer;}

		private:
			static void s_createCommandPool(const se::renderer::vulkan::CommandBufferInfos &infos);
			static void s_destroyCommandPool(const se::renderer::vulkan::CommandBufferInfos &infos);

			static std::map<se::renderer::vulkan::QueueType, VkCommandPool> s_commandPools;
			static std::map<se::renderer::vulkan::QueueType, se::Count> s_commandPoolCounts;

			se::renderer::vulkan::CommandBufferInfos m_infos;
			VkCommandBuffer m_commandBuffer;
	};

} // namespace se::renderer::vulkan
