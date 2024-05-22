#include "se/renderer/vulkan/swapChain.hpp"

#include "se/assertion.hpp"
#include "se/exceptions.hpp"



namespace se::renderer::vulkan {
	SwapChain::SwapChain(const se::renderer::vulkan::SwapChainInfos &infos) :
		m_infos {infos},
		m_swapchain {VK_NULL_HANDLE},
		m_chosenPresentMode {}
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfos {};
		swapchainCreateInfos.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfos.surface = m_infos.surface;

		FormatScoreCriterias formatScoreCriterias {};
		formatScoreCriterias.sRGB = true;
		formatScoreCriterias.adobeRGB = false;
		formatScoreCriterias.hdr = false;
		formatScoreCriterias.formats = m_infos.formats;
		VkSurfaceFormatKHR choosenFormat {s_chooseFormat(formatScoreCriterias)};
		//surfaceCapabilities;

		m_chosenPresentMode = s_choosePresentMode(m_infos.presentModes);

		if (vkCreateSwapchainKHR(m_infos.device, &swapchainCreateInfos, nullptr, &m_swapchain) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create a vulkan swapchain");
	}


	SwapChain::~SwapChain() {

	}


	VkSurfaceFormatKHR SwapChain::s_chooseFormat(const FormatScoreCriterias &criterias) {
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


	se::Int32 SwapChain::s_scoreFormat(const VkSurfaceFormatKHR &format, const FormatScoreCriterias &criterias) {
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


	VkPresentModeKHR SwapChain::s_choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes) {
		for (const auto &presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return VK_PRESENT_MODE_MAILBOX_KHR;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}


	VkExtent2D SwapChain::s_chooseExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
		
	}


} // namespace se::renderer::vulkan
