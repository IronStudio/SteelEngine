#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/format.hpp"



namespace se::renderer::vulkan {
	VkFormat formatSeToVk(se::renderer::Format format);
	se::renderer::Format formatVkToSe(VkFormat format);
} // namespace se::renderer::vulkan
