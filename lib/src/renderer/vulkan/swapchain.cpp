#include "se/renderer/vulkan/swapchain.hpp"

#include <algorithm>

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/logger.hpp"



namespace se::renderer::vulkan {
	Swapchain::Swapchain(const se::renderer::vulkan::SwapchainInfos &infos) :
		m_infos {infos},
		m_swapchain {VK_NULL_HANDLE},
		m_chosenPresentMode {},
		m_chosenFormat {},
		m_chosenExtent {},
		m_images {},
		m_imageViews {}
	{
		FormatScoreCriterias formatScoreCriterias {};
		formatScoreCriterias.sRGB = true;
		formatScoreCriterias.adobeRGB = false;
		formatScoreCriterias.hdr = false;
		formatScoreCriterias.formats = m_infos.formats;
		m_chosenFormat = s_chooseFormat(formatScoreCriterias);

		m_chosenPresentMode = s_choosePresentMode(m_infos.presentModes);
		m_chosenExtent = s_chooseExtent(m_infos.surfaceCapabilities, m_infos.windowSize);
		se::Uint32 chosenImageCount {s_chooseImageCount(m_infos.surfaceCapabilities)};

		VkSwapchainCreateInfoKHR swapchainCreateInfos {};
		swapchainCreateInfos.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfos.surface = m_infos.surface;
		swapchainCreateInfos.minImageCount = chosenImageCount;
		swapchainCreateInfos.imageFormat = m_chosenFormat.format;
		swapchainCreateInfos.imageColorSpace = m_chosenFormat.colorSpace;
		swapchainCreateInfos.imageExtent = m_chosenExtent;
		swapchainCreateInfos.imageArrayLayers = 1;
		swapchainCreateInfos.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapchainCreateInfos.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfos.queueFamilyIndexCount = 0;
		swapchainCreateInfos.pQueueFamilyIndices = nullptr;
		swapchainCreateInfos.presentMode = m_chosenPresentMode;
		swapchainCreateInfos.preTransform = m_infos.surfaceCapabilities.currentTransform;
		swapchainCreateInfos.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfos.clipped = VK_TRUE;
		swapchainCreateInfos.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_infos.device, &swapchainCreateInfos, nullptr, &m_swapchain) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create a vulkan swapchain");

		m_imageViews = s_createImageView(m_infos.device, m_swapchain, m_chosenFormat.format, m_images);
		SE_LOGGER.log({se::LogSeverity::eInfo}, "Swapchain image views count : {}", m_imageViews.size());
	}


	Swapchain::~Swapchain() {
		for (const auto &view : m_imageViews)
			vkDestroyImageView(m_infos.device, view, nullptr);

		if (m_swapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_infos.device, m_swapchain, nullptr);
	}


	VkSurfaceFormatKHR Swapchain::s_chooseFormat(const FormatScoreCriterias &criterias) {
		SE_ASSERT(
			criterias.sRGB && !criterias.adobeRGB
			|| !criterias.sRGB && criterias.adobeRGB
			|| !criterias.sRGB && !criterias.adobeRGB,
			"criterias.sRGB and criterias.adobeRGB are exclusive. You can have only one of them (or none)"
		);

		VkSurfaceFormatKHR bestFormat {};
		se::Int32 bestScore {-1};

		for (const auto &format : criterias.formats) {
			se::Int32 score {s_scoreFormat(format, criterias)};
			if (score < 0)
				continue;

			if (score <= bestScore)
				continue;

			bestScore = score;
			bestFormat = format;
		}

		if (bestScore < 0)
			throw se::exceptions::RuntimeError("Can't find a surface format suitable for the swapchain");

		return bestFormat;
	}


	se::Int32 Swapchain::s_scoreFormat(const VkSurfaceFormatKHR &format, const FormatScoreCriterias &criterias) {
		static const std::vector<VkFormat> sRGBformats {
			VK_FORMAT_R8G8B8A8_SRGB,
			VK_FORMAT_R8G8B8_SRGB,
			VK_FORMAT_B8G8R8A8_SRGB,
			VK_FORMAT_B8G8R8_SRGB,
		};

		static const std::vector<VkFormat> hdrFormats {
			VK_FORMAT_R16G16B16_UNORM,
			VK_FORMAT_R16G16B16_SNORM,
			VK_FORMAT_R16G16B16_USCALED,
			VK_FORMAT_R16G16B16_SSCALED,
			VK_FORMAT_R16G16B16_UINT,
			VK_FORMAT_R16G16B16_SINT,
			VK_FORMAT_R16G16B16_SFLOAT,
			VK_FORMAT_R16G16B16A16_UNORM,
			VK_FORMAT_R16G16B16A16_SNORM,
			VK_FORMAT_R16G16B16A16_USCALED,
			VK_FORMAT_R16G16B16A16_SSCALED,
			VK_FORMAT_R16G16B16A16_UINT,
			VK_FORMAT_R16G16B16A16_SINT,
			VK_FORMAT_R16G16B16A16_SFLOAT,
			VK_FORMAT_R32G32B32_UINT,
			VK_FORMAT_R32G32B32_SINT,
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_UINT,
			VK_FORMAT_R32G32B32A32_SINT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R64G64B64_UINT,
			VK_FORMAT_R64G64B64_SINT,
			VK_FORMAT_R64G64B64_SFLOAT,
			VK_FORMAT_R64G64B64A64_UINT,
			VK_FORMAT_R64G64B64A64_SINT,
			VK_FORMAT_R64G64B64A64_SFLOAT
		};


		se::Int32 score {0};
		std::vector<VkFormat>::const_iterator it;

		if (criterias.sRGB
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
			&& (it = std::find(sRGBformats.begin(), sRGBformats.end(), format.format)) != sRGBformats.end())
			score += 100 + sRGBformats.end() - it;

		if (criterias.adobeRGB
			&& (format.colorSpace == VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT || format.colorSpace == VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT))
			score += 100;

		if (criterias.hdr && (it = std::find(hdrFormats.begin(), hdrFormats.end(), format.format)) == hdrFormats.end())
			return -1;
		else if (criterias.hdr)
			score += hdrFormats.end() - it;

		return score;
	}


	VkPresentModeKHR Swapchain::s_choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes) {
		for (const auto &presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return VK_PRESENT_MODE_MAILBOX_KHR;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}


	VkExtent2D Swapchain::s_chooseExtent(const VkSurfaceCapabilitiesKHR &capabilities, const se::Vec2i &windowSize) {
		if (capabilities.currentExtent.width != std::numeric_limits<se::Uint32>::max())
			return capabilities.currentExtent;
		
		VkExtent2D extent {};
		extent.width = std::clamp((se::Uint32)windowSize.x, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.width = std::clamp((se::Uint32)windowSize.y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return extent;
	}


	se::Uint32 Swapchain::s_chooseImageCount(const VkSurfaceCapabilitiesKHR &capabilities) {
		se::Uint32 imageCount {capabilities.minImageCount + 1};
		if (capabilities.maxImageCount != 0 && imageCount > capabilities.maxImageCount)
			return capabilities.maxImageCount;
		return imageCount;
	}


	std::vector<VkImageView> Swapchain::s_createImageView(VkDevice device, VkSwapchainKHR swapchain, VkFormat format, std::vector<VkImage> &images) {
		se::Uint32 imageCount {};
		if (vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get swapchain images count");

		images.resize(imageCount);
		if (vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't get swapchain images");

		std::vector<VkImageView> imageViews {};
		imageViews.reserve(imageCount);

		VkImageViewCreateInfo createInfos {};
		createInfos.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfos.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfos.components.r = VK_COMPONENT_SWIZZLE_R;
		createInfos.components.g = VK_COMPONENT_SWIZZLE_G;
		createInfos.components.b = VK_COMPONENT_SWIZZLE_B;
		createInfos.components.a = VK_COMPONENT_SWIZZLE_A;
		createInfos.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfos.subresourceRange.baseMipLevel = 0;
		createInfos.subresourceRange.levelCount = 1;
		createInfos.subresourceRange.baseArrayLayer = 0;
		createInfos.subresourceRange.layerCount = 1;
		createInfos.format = format;

		for (const auto &image : images) {
			VkImageView view {};
			createInfos.image = image;

			if (vkCreateImageView(device, &createInfos, nullptr, &view) != VK_SUCCESS)
				throw se::exceptions::RuntimeError("Can't create an image view for the swapchain");

			imageViews.push_back(view);
		}

		return imageViews;
	}


} // namespace se::renderer::vulkan
