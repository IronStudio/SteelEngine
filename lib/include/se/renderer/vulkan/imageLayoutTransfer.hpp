#pragma once

#include <vulkan/vulkan.h>



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


	void transferImageLayout(const se::renderer::vulkan::ImageLayoutTransferInfos &infos);


} // namespace se::renderer::vulkan
