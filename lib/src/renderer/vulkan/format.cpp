#include "se/renderer/vulkan/format.hpp"

#include <map>

#include "se/assertion.hpp"



namespace se::renderer::vulkan {
	VkFormat formatSeToVk(se::renderer::Format format) {
		static const std::map<se::renderer::Format, VkFormat> formatMap {
			{se::renderer::Format::eNone,    VK_FORMAT_UNDEFINED},
			{se::renderer::Format::eR8,      VK_FORMAT_R8_SRGB},
			{se::renderer::Format::eRG8,     VK_FORMAT_R8G8_SRGB},
			{se::renderer::Format::eRGB32f,  VK_FORMAT_R32G32B32_SFLOAT},
			{se::renderer::Format::eRGB8,    VK_FORMAT_R8G8B8_SRGB},
			{se::renderer::Format::eRGBA32f, VK_FORMAT_R32G32B32A32_SFLOAT},
			{se::renderer::Format::eRGBA8,   VK_FORMAT_R8G8B8A8_SRGB},
			{se::renderer::Format::eBGR8,    VK_FORMAT_B8G8R8_SRGB},
			{se::renderer::Format::eBGRA8,   VK_FORMAT_B8G8R8A8_SRGB}
		};

		auto it {formatMap.find(format)};
		SE_ASSERT(it != formatMap.end(), "Can't find SE format to convert to Vk");
		return it->second;
	}


	se::renderer::Format formatVkToSe(VkFormat format) {
		static const std::map<VkFormat, se::renderer::Format> formatMap {
			{VK_FORMAT_UNDEFINED,           se::renderer::Format::eNone},
			{VK_FORMAT_R8_SRGB,             se::renderer::Format::eR8},
			{VK_FORMAT_R8G8_SRGB,           se::renderer::Format::eRG8},
			{VK_FORMAT_R32G32B32_SFLOAT,    se::renderer::Format::eRGB32f},
			{VK_FORMAT_R8G8B8_SRGB,         se::renderer::Format::eRGB8},
			{VK_FORMAT_R32G32B32A32_SFLOAT, se::renderer::Format::eRGBA32f},
			{VK_FORMAT_R8G8B8A8_SRGB,       se::renderer::Format::eRGBA8},
			{VK_FORMAT_B8G8R8_SRGB,         se::renderer::Format::eBGR8},
			{VK_FORMAT_B8G8R8A8_SRGB,       se::renderer::Format::eBGRA8}
		};

		auto it {formatMap.find(format)};
		SE_ASSERT(it != formatMap.end(), "Can't find Vk format to convert to SE");
		return it->second;
	}


} // namespace se::renderer::vulkan
