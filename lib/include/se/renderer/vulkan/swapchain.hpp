#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/math.hpp"
#include "se/types.hpp"



namespace se::renderer::vulkan {
	struct SwapchainInfos {
		VkDevice device;
		VkSurfaceKHR surface;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		se::Vec2i windowSize;
	};


	class SE_CORE Swapchain {
		public:
			Swapchain(const se::renderer::vulkan::SwapchainInfos &infos);
			~Swapchain();

			inline VkSwapchainKHR getSwapChain() const noexcept {return m_swapchain;}
			inline const VkSurfaceFormatKHR &getFormat() const noexcept {return m_chosenFormat;}


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
			static VkExtent2D s_chooseExtent(const VkSurfaceCapabilitiesKHR &capabilities, const se::Vec2i &windowSize);
			static se::Uint32 s_chooseImageCount(const VkSurfaceCapabilitiesKHR &capabilities);
			static std::vector<VkImageView> s_createImageView(VkDevice device, VkSwapchainKHR swapchain, VkFormat format);

			se::renderer::vulkan::SwapchainInfos m_infos;
			VkSwapchainKHR m_swapchain;
			VkPresentModeKHR m_chosenPresentMode;
			VkSurfaceFormatKHR m_chosenFormat;
			std::vector<VkImageView> m_imageViews;
	};

} // namespace se::renderer::vulkan
