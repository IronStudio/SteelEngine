#include "se/renderer/vulkan/imageLayoutTransfer.hpp"



namespace se::renderer::vulkan {
	void transferImageLayout(const se::renderer::vulkan::ImageLayoutTransferInfos &infos) {
		VkImageMemoryBarrier imageMemoryBarrier {};
		imageMemoryBarrier.srcAccessMask = infos.srcAccessMask;
		imageMemoryBarrier.dstAccessMask = infos.dstAccessMask;
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.oldLayout = infos.oldLayout;
		imageMemoryBarrier.newLayout = infos.newLayout;
		imageMemoryBarrier.image = infos.image;
		imageMemoryBarrier.subresourceRange.aspectMask = infos.aspect;
		imageMemoryBarrier.subresourceRange.levelCount = 1;
		imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(
			infos.commandBuffer,
			infos.srcStage,
			infos.dstStage,
			0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier
		);
	}


} // namespace se::renderer::vulkan
