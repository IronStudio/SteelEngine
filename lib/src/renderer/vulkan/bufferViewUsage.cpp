#include "se/renderer/vulkan/bufferViewUsage.hpp"

#include <map>

#include "se/assertion.hpp"



namespace se::renderer::vulkan {
	VkDescriptorType bufferUsageSeToVk(se::renderer::BufferViewUsage usage) {
		static const std::map<se::renderer::BufferViewUsage, VkDescriptorType> map {
			{se::renderer::BufferViewUsage::eUniform, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER},
			{se::renderer::BufferViewUsage::eStorage, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER},
		};

		auto it {map.find(usage)};
		SE_ASSERT(it != map.end(), "Can't find wanted attribute");
		return it->second;
	}


	se::renderer::BufferViewUsage bufferUsageVkToSe(VkDescriptorType usage) {
		static const std::map<VkDescriptorType, se::renderer::BufferViewUsage> map {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, se::renderer::BufferViewUsage::eUniform},
			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, se::renderer::BufferViewUsage::eStorage},
		};

		return map.find(usage)->second;
	}


} // namespace se::renderer::vulkan
