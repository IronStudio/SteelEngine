#include "se/renderer/vulkan/depthBuffer.hpp"

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/format.hpp"
#include "se/renderer/vulkan/vramAllocator.hpp"



namespace se::renderer::vulkan {
	DepthBuffer::DepthBuffer(const se::renderer::DepthBufferInfos &infos) :
		se::renderer::DepthBuffer(infos),
		m_image {VK_NULL_HANDLE},
		m_imageView {VK_NULL_HANDLE},
		m_memoryHandle {nullptr}
	{
		SE_ASSERT(m_infos.format == se::renderer::Format::eD16 || m_infos.format == se::renderer::Format::eD32, "Invalid format for depth buffer");

		VkFormat format {se::renderer::vulkan::formatSeToVk(m_infos.format)};
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		VkImageCreateInfo imageCreateInfos {};
		imageCreateInfos.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfos.extent = {(se::Uint)m_infos.size.x, (se::Uint)m_infos.size.y, 1};
		imageCreateInfos.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfos.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageCreateInfos.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfos.arrayLayers = 1;
		imageCreateInfos.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfos.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfos.format = format;
		imageCreateInfos.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfos.mipLevels = 1;

		if (vkCreateImage(device, &imageCreateInfos, nullptr, &m_image) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create image for depth buffer");


		VkMemoryRequirements memoryRequirements {};
		vkGetImageMemoryRequirements(device, m_image, &memoryRequirements);

		se::renderer::VramAllocationInfos allocationInfos {};
		allocationInfos.size = memoryRequirements.size;
		allocationInfos.alignement = memoryRequirements.alignment;
		m_memoryHandle = m_infos.allocator->allocate(allocationInfos);

		auto memoryHandle {reinterpret_cast<se::renderer::vulkan::VramAllocatorHandle*> (m_memoryHandle.get())};
		if (vkBindImageMemory(device, m_image, memoryHandle->getMemory(), memoryHandle->getOffset()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't bind memory to depth buffer image");


		VkImageViewCreateInfo imageViewCreateInfos {};
		imageViewCreateInfos.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfos.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfos.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfos.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfos.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfos.format = format;
		imageViewCreateInfos.image = m_image;
		imageViewCreateInfos.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageViewCreateInfos.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfos.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfos.subresourceRange.layerCount = 1;
		imageViewCreateInfos.subresourceRange.levelCount = 1;
		imageViewCreateInfos.viewType = VK_IMAGE_VIEW_TYPE_2D;

		if (vkCreateImageView(device, &imageViewCreateInfos, nullptr, &m_imageView) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create image view for depth buffer");		
	}


	DepthBuffer::~DepthBuffer() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_imageView != VK_NULL_HANDLE)
			vkDestroyImageView(device, m_imageView, nullptr);

		m_infos.allocator->free(m_memoryHandle.get());

		if (m_image != VK_NULL_HANDLE)
			vkDestroyImage(device, m_image, nullptr);
	}


} // namespace se::renderer::vulkan
