#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"



namespace se::renderer::vulkan {
	struct ImageLayoutTransferInfos {
		VkCommandBuffer commandBuffer;
		VkImageLayout oldLayout;
		VkImageLayout newLayout;
		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;
		VkImageAspectFlags aspect;
		VkImage image;
		VkAccessFlags dstAccessMask;
		VkAccessFlags srcAccessMask;
	};


	SE_CORE void transferImageLayout(const se::renderer::vulkan::ImageLayoutTransferInfos &infos);


} // namespace se::renderer::vulkan
