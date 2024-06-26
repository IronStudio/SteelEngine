#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/bufferViewUsage.hpp"



namespace se::renderer::vulkan {
	VkDescriptorType bufferUsageSeToVk(se::renderer::BufferViewUsage usage);
	se::renderer::BufferViewUsage bufferUsageVkToSe(VkDescriptorType usage);
} // namespace se::renderer::vulkan
