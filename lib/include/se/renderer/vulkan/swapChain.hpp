#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"



namespace se::renderer::vulkan {
	struct SwapChainInfos {
		VkDevice device;
		VkSurfaceKHR surface;
	};


	class SE_CORE SwapChain {
		public:
			SwapChain(const se::renderer::vulkan::SwapChainInfos &infos);
			~SwapChain();

			inline VkSwapchainKHR getSwapChain() const noexcept {return m_swapchain;}


		private:
			se::renderer::vulkan::SwapChainInfos m_infos;
			VkSwapchainKHR m_swapchain;
	};

} // namespace se::renderer::vulkan
