#include "se/renderer/vulkan/renderPass.hpp"

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"



namespace se::renderer::vulkan {
	RenderPass::RenderPass(const se::renderer::RenderPassInfos &infos) :
		se::renderer::RenderPass(infos),
		m_renderPass {VK_NULL_HANDLE}
	{
		const auto &swapchain {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getSwapchain()};
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		VkAttachmentDescription attachment {};
		attachment.format = swapchain->getFormat().format;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference attachmentReference {};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &attachmentReference;

		VkRenderPassCreateInfo renderPassCreateInfos {};
		renderPassCreateInfos.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfos.attachmentCount = 1;
		renderPassCreateInfos.pAttachments = &attachment;
		renderPassCreateInfos.dependencyCount = 0;
		renderPassCreateInfos.pDependencies = nullptr;
		renderPassCreateInfos.subpassCount = 1;
		renderPassCreateInfos.pSubpasses = &subpassDescription;

		if (vkCreateRenderPass(device, &renderPassCreateInfos, nullptr, &m_renderPass) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create render pass");
	}


	RenderPass::~RenderPass() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_renderPass != VK_NULL_HANDLE)
			vkDestroyRenderPass(device, m_renderPass, nullptr);
	}


} // namespace se::renderer::vulkan
