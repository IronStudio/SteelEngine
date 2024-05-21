#include "se/renderer/vulkan/swapChain.hpp"

#include "se/exceptions.hpp"



namespace se::renderer::vulkan {
	SwapChain::SwapChain(const se::renderer::vulkan::SwapChainInfos &infos) :
		m_infos {infos},
		m_swapchain {VK_NULL_HANDLE}
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfos {};
		swapchainCreateInfos.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfos.surface = m_infos.surface;
		
		
		if (vkCreateSwapchainKHR(m_infos.device, &swapchainCreateInfos, nullptr, &m_swapchain) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create a vulkan swapchain");
	}


	SwapChain::~SwapChain() {

	}


} // namespace se::renderer::vulkan
