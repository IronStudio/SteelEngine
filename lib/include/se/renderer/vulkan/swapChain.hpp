#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/types.hpp"



namespace se::renderer::vulkan {
	struct SwapChainInfos {
		VkDevice device;
		VkSurfaceKHR surface;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
	};


	class SE_CORE SwapChain {
		public:
			SwapChain(const se::renderer::vulkan::SwapChainInfos &infos);
			~SwapChain();

			inline VkSwapchainKHR getSwapChain() const noexcept {return m_swapchain;}


		private:
			struct FormatScoreCriterias {
				std::vector<VkSurfaceFormatKHR> formats;
				bool sRGB;
				bool adobeRGB;
				bool hdr;
			};

			static VkSurfaceFormatKHR s_chooseFormat(const FormatScoreCriterias &criterias);
			static se::Int32 s_scoreFormat(const VkSurfaceFormatKHR &format, const FormatScoreCriterias &criterias);
			static VkPresentModeKHR s_choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes);
			static VkExtent2D s_chooseExtent(const VkSurfaceCapabilitiesKHR &capabilities);

			se::renderer::vulkan::SwapChainInfos m_infos;
			VkSwapchainKHR m_swapchain;
			VkPresentModeKHR m_chosenPresentMode;
	};

} // namespace se::renderer::vulkan
