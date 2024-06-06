#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/format.hpp"



namespace se::renderer::vulkan {
	SE_CORE VkFormat formatSeToVk(se::renderer::Format format);
	SE_CORE se::renderer::Format formatVkToSe(VkFormat format);
} // namespace se::renderer::vulkan
